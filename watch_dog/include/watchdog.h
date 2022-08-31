/*****************************************************************************
 *	FILENAME:	watchdog.h    AUTHOR: Liad Oz	 TEAM: Yellow    LAB: OL100	 *
 *																			 *
 *	REVIEWER:																 *
 *																			 *
 *	PURPOSE:    API for watchdog lib.										 *
 *																			 *
 *****************************************************************************/

#ifndef __ILRD_WATCHDOG_H__
#define __ILRD_WATCHDOG_H__

#include <stddef.h> /* size_t */
#include <time.h> /* time_t */
/*****************************************************************************/
/*
 * call back function delivered by the user returns 0 or 1, 
 * 0 on return indicates everything is OK. 
 * 1 indicates wd should reboot user program.
 */
typedef int(*wd_call_back)(void *arg); 
/*****************************************************************************/
/*
 * Description: the function initialize WD and protecting the critical section 
 * that follows until DNR is called, returns 0 on success otherwise failure.
 * Return value: 
 * Time Complexity: 
 */
int MMI(int argc, char **argv, time_t interval, int grace);
/*****************************************************************************/
/*
 * Description: the function cease to protect the critical section 
 * and destroy WD. return 0 on success other wise failure.
 * Return value: 
 * Time Complexity: 
 */
int DNR(void);
/*****************************************************************************/
#endif