/*****************************************************************************
 *	FILENAME:	watchdog.c    AUTHOR: Liad Oz	 TEAM: Yellow    LAB: OL100	 *
 *																			 *
 *	REVIEWER:	Elizabeta V.												 *
 *																			 *
 *	PURPOSE:    Implementing for watchdog lib.							     *
 *                                                                           *
 *****************************************************************************/

#define _POSIX_C_SOURCE (200112L) /* environment vars */

#include <assert.h> /* asserts */
#include <stdio.h> /* printf */
#include <stdlib.h> /* dynamic allocations */
#include <unistd.h> /* time_t */
#include <signal.h> /* signals */
#include <semaphore.h> /* sem */
#include <fcntl.h> /* sem O_CREAT */
#include <pthread.h> /* threads */
#include <sys/types.h> /* general types */
#include <sys/wait.h> /* waitpid */
#include <sys/stat.h> /* premissions */

#include "scheduler.h"
#include "watchdog_util.h"
#include "watchdog.h"

#define ENV_BUFF_SIZE 100
#define WD_PATH "./watchdog_main_DEBUG.out"
/*****************************************************************************/

/* container for watchdog configuration */
typedef struct wd_config
{
    time_t interval;
    int grace;
    pid_t wd_pid;
    pthread_t tid;
} wd_config_t;

/* environment */
static int WDSetLongToEnv(char *env_def, long val);
static void WDImportEnv(wd_config_t *config);
static pthread_t g_tid = {0}; 

/* flags */
static int g_sched_to_stop = 0; /* two copies for both processes */
static sig_atomic_t g_spare_on_wd_count = 0;
static sig_atomic_t g_spare_on_ua_count = 0;

/* process/threads synchronization */
static void HandShakeDog(void);
static void HandShakeUser(void);
static void EndSem(void);
static sem_t *g_wd_sem = NULL; /* sync #1: handshake before running sched */
static sem_t *g_ua_sem = NULL; 
static sem_t g_mmi_sem = {0}; /* sync #2: prevent from user (main-thread) 
                                finish MMI before initing sched (sub-thread) */

/*********************************** api *************************************/
int MMI(int argc, char **argv, time_t interval, int grace)
{
    wd_status_t status = 0;
 
    (void)argc;

    /* init unnamed semaphore */
    sem_init(&g_mmi_sem, 0, 0);

    /* set environment vars: interval & grace */
    if (WD_SUCCESS != WDSetLongToEnv("WD_INTERVAL", interval) ||
        WD_SUCCESS != WDSetLongToEnv("WD_GRACE", grace))
    {
        return WD_ENV_ERR;
    }

    /* define SIGUSER1 handler */
    SetSignalHandler(SIGUSR1, UAPreventWatchDogForking);

    /* spawn watchdog process */
    status = UASpawnWatchDog(argv);

    /* init scheduler thread */
    status = pthread_create(&g_tid, NULL, UAInitScheduler, (void *)argv);
    if (WD_SUCCESS != status)
    {
        return WD_THREAD_CREATE_ERR;
    }

    sem_wait(&g_mmi_sem);
    
    return WD_SUCCESS;
}

int DNR(void)
{
    wd_config_t config = {0};

    WDImportEnv(&config);

    /* flag to end and exit user app gracefully */
    g_sched_to_stop = 1;

    /* send WD signal to clean up */
    kill(config.wd_pid, SIGUSR2);
    
    /* zombie clean */
    waitpid(config.wd_pid, NULL, WNOHANG);

    /* pthread destroy */
    pthread_join(g_tid, NULL);

    /* end semaphore */
    EndSem();

    return WD_SUCCESS;
}
/*********************************** utils ***********************************/
void SetSignalHandler(int signal, 
                void (*handler)(int signum, siginfo_t *info, void *context))
{
    struct sigaction sa_signal = {0};

    sa_signal.sa_flags = SA_SIGINFO;
    sa_signal.sa_sigaction = handler;

    sigaction(signal, &sa_signal, NULL);
}

static int WDSetLongToEnv(char *env_def, long val)
{
    char temp[ENV_BUFF_SIZE] = {0};
    int status = 0;

    sprintf(temp, "%ld", val);
    
    status = setenv(env_def, temp, 1);
    if (WD_SUCCESS != status)
    {
        return WD_ENV_ERR;
    }

    return WD_SUCCESS;
}

static void WDImportEnv(wd_config_t *config)
{
    config->interval = atoi(getenv("WD_INTERVAL"));
    config->grace = atoi(getenv("WD_GRACE"));
    config->wd_pid = atoi(getenv("WD_PID"));
}

/* UA & WD task #3 */
int GracefulExit(void *param)
{
    scheduler_t *sched = (scheduler_t *)param;

    if (1 == g_sched_to_stop)
    {
        SchedulerStop(sched);
    }

    return WD_SUCCESS;
}
/************************************ UA *************************************/
wd_status_t UASpawnWatchDog(char **argv)
{ 
    pid_t wd_pid = fork();
    if (0 > wd_pid)
    {
        return WD_FORK_ERR; 
    }

    /* set current pid of watchdog */ 
    WDSetLongToEnv("WD_PID", wd_pid);

    /* case parent */
    if (0 < wd_pid)
    {
        /* do nothing */
    }
    /* case child */
    else
    {
        /* execute WD */
        execv(WD_PATH, argv);

        /* the execvp function returns only if an error occurs */
        return WD_EXEC_ERR;
    }

    return WD_SUCCESS;
}

void *UAInitScheduler(void *param)
{
    char **argv = (char **)param;
    scheduler_t *sched = NULL;
    wd_config_t config = {0};

    /* create scheduler */
    sched = SchedulerCreate();
    if (NULL == sched)
    {
        return NULL;
    }

    /* import updated Environment vars */
    WDImportEnv(&config);

    /* task 1 - signal to watch dog that user app is alive */
    SchedulerAddTask(sched, 
                    UANotifyImAlive, 
                    NULL,
                    config.interval,
                    NULL);

    /* task 2 - WD spawn (case needed) */ 
    SchedulerAddTask(sched, 
                    UAResurrectDog, 
                    NULL, 
                    config.interval, 
                    argv); 
                      
    /* task 3 - scheduler stop task (case needed) */ 
    SchedulerAddTask(sched, 
                    GracefulExit, 
                    NULL, 
                    config.interval, 
                    sched); 

    /* sync #1: user post to watchdog ready for running sched */
    HandShakeUser();

    /* sync #2: user sub-thread post to user main-thread */
    sem_post(&g_mmi_sem);
    
    /* sub-thread will enter infinity loop */
    SchedulerRun(sched);

    /* destroy sched */
    SchedulerDestroy(sched);

    return NULL;
}

/* UA SIGUSER1 handler */
void UAPreventWatchDogForking(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)info;
    (void)context;

    /* restart wd grace counter */
    g_spare_on_wd_count = 0;
    
    puts("\033[34;1m I AM UA: received signal don't fork WD");
}

/* UA task #1 */
int UANotifyImAlive(void *param)
{
    wd_config_t config = {0};

    (void)param;
    
    WDImportEnv(&config);

    kill(config.wd_pid, SIGUSR1);

    return WD_SUCCESS;
}

/* UA task #2 */
int UAResurrectDog(void *param)
{
    char **argv = (char **)param;
    wd_config_t config = {0};

    WDImportEnv(&config);
    
    puts("\033[34;1m -> entered resurrect");
    
    if (g_spare_on_wd_count >= config.grace)
    {
        puts(" (*) perform resurrect");
        
        /* confirm death of WD */
        kill(config.wd_pid, SIGKILL);
   
        /* WD zombie clean */
        waitpid(config.wd_pid, NULL, WNOHANG);
    
        /* restart grace */
        g_spare_on_wd_count = 0;
        
        /* spawn new watch dog */
        UASpawnWatchDog(argv);
    }
    else
    {
        ++g_spare_on_wd_count;
    }

    return WD_SUCCESS;
}
/************************************* WD ************************************/
void *WDInitScheduler(void *param)
{
    char **argv = (char **)param;
    scheduler_t *sched = NULL;
    wd_config_t config = {0};

    WDImportEnv(&config);

    /* create scheduler */
    sched = SchedulerCreate();
    if (NULL == sched)
    {
        return NULL;
    }

    /* task 1 - signal to user app that watchdog is alive */
    SchedulerAddTask(sched, 
                    WDNotifyImAlive, 
                    NULL, 
                    config.interval, 
                    NULL);

    /* task 2 - WD spawn (case needed) */ 
    SchedulerAddTask(sched, 
                    WDResurrectUserApp, 
                    NULL, 
                    config.interval, 
                    argv); 
                    
    /* task 3 - scheduler stop task (case needed) */ 
    SchedulerAddTask(sched, 
                    GracefulExit, 
                    NULL, 
                    config.interval,
                    sched); 

    
    /* sync #1: watchdog post to user ready for running sched */
    HandShakeDog();
                  
    /* sub-thread will enter infinity schedulering loop */
    SchedulerRun(sched);

    /* destroy sched */
    SchedulerDestroy(sched);

    return NULL;
}

/* WD SIGUSR1 handler */
void WDPreventUserAppForking(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)info;
    (void)context;

    /* restart ua grace counter */
    g_spare_on_ua_count = 0;
        
    puts("\033[33;1m I AM WD: received signal don't fork app");
}

/* WD SIGUSR2 handler */
void WDUpdateCleanUp(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)info;
    (void)context;

    /* stop scheduler */
    g_sched_to_stop = 1;
}

/* WD task #1 */
int WDNotifyImAlive(void *param)
{
    pid_t pid = (long)getppid();

    (void)param;
    
    kill(pid, SIGUSR1);

    return WD_SUCCESS;
}

/* WD task #2 */ 
int WDResurrectUserApp(void *param)
{
    char **argv = (char **)param;
    wd_config_t config = {0};

    WDImportEnv(&config);

    puts("\033[33;1m -> entered resurrect");
    
    if (g_spare_on_ua_count >= config.grace)
    {
        puts(" (*) perform resurrect");

        /* TODO: confirm UA death */
    
        /* spawn new user app */
        execv(argv[0], argv);
    }
    else
    {
        ++g_spare_on_ua_count;
    }

    return WD_SUCCESS;
}
/*********************************** sync ************************************/
static void HandShakeDog(void)
{
    g_ua_sem = sem_open("/WD_SEM_UA", O_CREAT, S_IRWXU, 0);
    if (SEM_FAILED == g_ua_sem)
    {
        perror("sem_open() failed!\n");      
    }
       
    g_wd_sem = sem_open("/WD_SEM", O_CREAT, S_IRWXU, 0);
    if (SEM_FAILED == g_wd_sem )
    {
        perror("sem_open() failed!\n");      
    }

    sem_post(g_ua_sem);
    sem_wait(g_wd_sem);
}

static void HandShakeUser(void)
{
    g_ua_sem = sem_open("/WD_SEM_UA", O_CREAT, S_IRWXU, 0);
    if (SEM_FAILED == g_ua_sem)
    {
        perror("sem_open() failed!\n");      
    }

    g_wd_sem = sem_open("/WD_SEM", O_CREAT, S_IRWXU, 0);
    if (SEM_FAILED == g_wd_sem)
    {
        perror("sem_open() failed!\n");      
    }

    sem_post(g_wd_sem);
    sem_wait(g_ua_sem);  
}

static void EndSem(void)
{
    sem_close(g_ua_sem);
    sem_close(g_wd_sem);

    sem_unlink("/WD_SEM_UA");
    sem_unlink("/WD_SEM");

    sem_destroy(&g_mmi_sem);
}
/*****************************************************************************/