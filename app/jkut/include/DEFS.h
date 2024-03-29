// Copyright 28-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions and constants.

#ifndef DEFS_H
  #define DEFS_H

  #define GC_THREADS
  #include "kut/DEFS.h"
  #include "exp.h"

  #define TESTING

  #ifdef TESTING

    /// Utility to show expression type error messages.
     #define TEST_TOKEN_TYPE_ERROR(fn_is, expected, exp) { \
        char *__expected = expected; \
        Token *__exp = exp; \
        if (!fn_is(this)) \
          EXC_ILLEGAL_ARGUMENT( \
            "Bad token type", __expected, token_type_to_str(__exp) \
          ); \
      }

    /// Utility to show expression type error messages.
     #define TEST_EXP_TYPE_ERROR(fn_is, expected, exp) { \
        char *__expected = expected; \
        Exp *__exp = exp; \
        if (!fn_is(this)) \
          EXC_ILLEGAL_ARGUMENT( \
            "Bad expression type", __expected, exp_type_to_str(__exp) \
          ); \
      }

    /// Utility to show statemtne type error messages.
    #define TEST_STAT_TYPE_ERROR(fn_is, expected, stat) { \
        char *__expected = expected; \
        Stat *__stat = stat; \
        if (!fn_is(this)) \
          EXC_ILLEGAL_ARGUMENT( \
            "Bad statement type", __expected, stat_type_to_str(__stat) \
          ); \
      }

  #else
    #define TEST_TOKEN_TYPE_ERROR(fn_is, expected, exp)

    #define TEST_EXP_TYPE_ERROR(fn_is, expected, exp)

    #define TEST_STAT_TYPE_ERROR(fn_is, expected, stat)
  #endif

  /// Check a bfunction parameters.
  ///   fn_id: Function identifier (e.g. "sys.print")
  ///   n    : Expecter parameters number. (e.g. 1)
  ///   array: Arr<Exp> to check.
  #define CHECK_PARS(fn_id, n, array) if (n != arr_size(array)) EXC_GENERIC(str_f(\
      "'%s' bad parameters number:\n  expected: %d\n    actual: %d", \
      fn_id, n, arr_size(array) \
    ))

#endif
