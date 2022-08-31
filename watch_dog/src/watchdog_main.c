/*****************************************************************************
 *	FILENAME:	watchdog_main.c  AUTHOR: Liad Oz  TEAM: Yellow   LAB: OL100	 *
 *																			 *
 *	REVIEWER:	Elizabeta V.												 *
 *																			 *
 *	PURPOSE:    This is the watchdog app.                                    *
 *																			 *
 *****************************************************************************/

#include <assert.h> /* asserts */
#include <stdio.h> /* printf */
#include <stdlib.h> /* dynamic allocations */
#include <unistd.h> /* time_t */
#include <signal.h> /* signals */
#include <pthread.h> /* threads */
#include <sys/wait.h> /* waitpid */

#include "scheduler.h" /* task scheduler */
#include "watchdog_util.h"
#include "watchdog.h"
/*****************************************************************************/
int main(int argc, char *argv[])
{
    (void)argc;

    /* define WD SIGUSER1/2 handlers */
    SetSignalHandler(SIGUSR1, WDPreventUserAppForking);
    SetSignalHandler(SIGUSR2, WDUpdateCleanUp);
    
    /* init and start scheduler */
    WDInitScheduler((void *)argv);

	return 0;
}
/*****************************************************************************/