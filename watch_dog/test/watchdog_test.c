/*****************************************************************************
 *	FILENAME:	watchdog_test.c   AUTHOR: Liad Oz	TEAM: Yellow  LAB: OL100 *
 *																			 *
 *	REVIEWER:																 *
 *																			 *
 *	PURPOSE:    Testing for watchdog lib.							 	 	 *
 *																			 *
 *****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* dynamic allocations */
#include <unistd.h> /* sleep */

#include "watchdog.h"
/*****************************************************************************/
int main(int argc, char *argv[])
{	
	time_t interval = 3;
	int i = 50;

	MMI(argc, argv, interval, 3);
	
	while (i)
    {
        sleep(1);
		--i;
    }

	DNR();

	return 0;
}
/*****************************************************************************/