// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TP_RS_H
  #define TP_RS_H

/// Structure for results of reading one line.

/// Reading states.
enum St {CODE, COMMENT, SINGLE, DOUBLE, BACK};

#include "dmc/std.h"

/*.-.*/

///
typedef struct rs_Rs Rs;

///
Rs *rs_new(enum St state, char *read, char *rest);

///
enum St rs_state(Rs *this);

///
char *rs_read(Rs *this);

///
char *rs_rest(Rs *this);

/*.-.*/

#endif
