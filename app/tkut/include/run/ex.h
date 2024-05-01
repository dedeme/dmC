// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression data. Immutable.

#ifndef RUN_EX_H
  #define RUN_EX_H

#include "run/rn_code.h"

///
typedef enum run_ex_type ExT;

///
typedef struct run_ex Ex;

enum run_ex_type {
  EX_STRING
};

///
struct run_ex {
  int nline;
  int nchar;
  ExT type;
  void *value;
};

/// Constructor.
Ex *ex_mk_string (int nline, int nchar, char *value);

/// Retrive 'this' value.
char *ex_string (Ex *this);

/// Run 'this'
Ex *exp_solve(RnCode *code, Ex *this);

///
char *ex_to_js (Ex *this);

///
Ex *ex_from_js (char *js);

#endif
