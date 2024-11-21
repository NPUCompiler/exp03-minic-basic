///
/// @file getopt-port.cpp
/// @brief getopt函数的多平台支持头文件
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#ifndef _USUAL_GETOPT_H_
#define _USUAL_GETOPT_H_

#ifndef NEED_USUAL_GETOPT
#if !defined(_MSC_VER)
#define NEED_USUAL_GETOPT
#endif
#endif

#ifdef NEED_USUAL_GETOPT

#include <getopt.h>

#else /* NEED_USUAL_GETOPT */

#ifdef __cplusplus
extern "C" {
#endif

/** argument to current option, or NULL if it has none */
extern char * optarg;
/** Current position in arg string.  Starts from 1.
 * Setting to 0 resets state.
 */
extern int optind;
/** whether getopt() should print error messages on problems.  Default: 1. */
extern int opterr;
/** Option char which caused error */
extern int optopt;

/** long option takes no argument */
#define no_argument 0
/** long option requires argument */
#define required_argument 1
/** long option has optional argument */
#define optional_argument 2

/** Long option description */
struct option {
    /** name of long option */
    const char * name;

    /**
     * whether option takes an argument.
     * One of no_argument, required_argument, and optional_argument.
     */
    int has_arg;

    /** if not NULL, set *flag to val when option found */
    int * flag;

    /** if flag not NULL, value to set *flag to; else return value */
    int val;
};

/** Compat: getopt */
int getopt(int argc, char * argv[], const char * options);

/** Compat: getopt_long */
int getopt_long(int argc, char * argv[], const char * options, const struct option * longopts, int * longindex);

/** Compat: getopt_long_only */
int getopt_long_only(int nargc, char * argv[], const char * options, const struct option * long_options, int * idx);

#ifdef __cplusplus
}
#endif

#endif /* NEED_USUAL_GETOPT */

#endif /* !_USUAL_GETOPT_H_ */
