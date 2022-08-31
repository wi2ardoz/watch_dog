/*****************************************************************************
 *	FILENAME:	utl.h			AUTHOR: Liad Oz			TEAM: Blue			 *
 *																			 *
 *	PURPOSE:	This is utils lib.										     *
 *																			 *
 *****************************************************************************/

#ifndef __ILRD_UTILS_H__
#define __ILRD_UTILS_H__
/***************************** ignore gcc errors *****************************/
#define UNUSED(x) (void)x
/******************************** stdout colors ******************************/
#define DEF         "\033[0m"              /* Default */
#define DEFAULT     "\x1b[0;0m"            /* Default */
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define PURPLE      "\033[35;1m"           /* Purple */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
/************************************ math ***********************************/
#define ABS(num) ( ( num >= 0 ) ? num : -num )
#define MIN(a,b) ( ( a < b ) ? a : b )
#define MAX(a,b) ( ( a > b ) ? a : b )
/************************************ size ***********************************/
#define WORD_SIZE sizeof(size_t)
/************************************ word ***********************************/
#define WORD_REMAINDER(size) ( size % WORD_SIZE )
#define WORD_REMAINDER_DIFF(size) ( WORD_SIZE - WORD_REMAINDER(size) )
/******************************* unit testing ********************************/
#define TEST(name, real, expected) \
    do { \
        ++g_test_counter; \
        ((real) == (expected)) ? \
        ++g_success_counter : \
        printf(RED "%s, Failed on line " BOLDRED "%d" DEF RED ", Real: " \
                    BOLDRED "%ld" DEF RED ", Expected: " BOLDRED "%ld\n" DEF, \
                            name, __LINE__, (long) (real), (long) (expected)); \
    } while (0)

#define TESTC(name, real, expected) \
    do { \
        ++g_test_counter; \
        ((real) == (expected)) ? \
        ++g_success_counter : \
        printf(RED "%s, Failed on line " BOLDRED "%d" DEF RED ", Real: " \
                    BOLDRED "%ld" DEF RED ", Expected: " BOLDRED "%ld\n" DEF, \
                            name, __LINE__, (long) (real), (long) (expected)); \
    } while (0)

#define PASS \
    do { \
        if (g_test_counter != g_success_counter) \
        { \
            printf(RED "Success Rate " BOLDRED "%f%%\n" DEF, \
                (((float) g_success_counter / (float) g_test_counter) * 100)); \
            printf(RED "Failed " BOLDRED "%d/%d" RED" tests\n\n" DEF, \
                        g_test_counter - g_success_counter, g_test_counter); \
        } \
    } while (0)

#define PRINT_TEST(a, b, format) \
	do { \
        if ((a) == (b)) \
        { \
            puts(GREEN "PASS" DEFAULT); \
        } \
        else \
        { \
            printf(RED "FAILED! Expected "format", not "format"\n" DEFAULT, \
                                                                    (a), (b)); \
        } \
    } while (0) 
/*****************************************************************************/
#endif