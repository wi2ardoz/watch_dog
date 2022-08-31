/*****************************************************************************
 *	FILENAME:	watchdog_util.h   AUTHOR: Liad Oz  TEAM: Yellow  LAB: OL100  *
 *																			 *
 *	REVIEWER:																 *
 *																			 *
 *	PURPOSE:    API for watchdog_util lib.									 *
 *																			 *
 *****************************************************************************/

#ifndef __ILRD_WATCHDOG_UTIL_H__
#define __ILRD_WATCHDOG_UTIL_H__

#include <stddef.h> /* size_t */
/*****************************************************************************/
typedef enum wd_status
{
    WD_KILL_ERR = -2,
    WD_FORK_ERR = -1,
    WD_SUCCESS = 0,
    WD_EXEC_ERR = 1,
    WD_SCHEDULER_ERR = 2,
    WD_THREAD_CREATE_ERR = 3,
    WD_ENV_ERR = 4
} wd_status_t;

typedef enum wd_bool
{
    WD_FALSE = 0,
    WD_TRUE = 1
} wd_bool_t;

/* funcs */
void *WDInitScheduler(void *param);
void *UAInitScheduler(void *param);
wd_status_t UASpawnWatchDog(char **argv);

/* tasks & handlers */
int WDNotifyImAlive(void *param);
int UANotifyImAlive(void *param);
int UAResurrectDog(void *param);
int WDResurrectUserApp(void *param);
int GracefulExit(void *param);
void UAPreventWatchDogForking(int signum, siginfo_t *info, void *context);
void WDPreventUserAppForking(int signum, siginfo_t *info, void *context);
void WDUpdateCleanUp(int signum, siginfo_t *info, void *context);

/* utils */
void SetSignalHandler(int signal, 
                void (*handler)(int signum, siginfo_t *info, void *ucontext));
/*****************************************************************************/
#endif