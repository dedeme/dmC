// Copyright 26-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef QQUOTES_H
  #define QQUOTES_H

/// Queue of quotes

#include "dmc/std.h"

///
typedef struct qquotes_Qquotes Qquotes;

/// Values initialized to -3
Qquotes *qquotes_new (int days);

///
void qquotes_free (Qquotes *this);

/// If quotes number is not complete, returns 1.
int qquotes_waiting (Qquotes *this);

///
void qquotes_add (Qquotes *this, double quote);

/// Returns the maximum or a value < 0 if it can not be calculated.
double qquotes_max (Qquotes *this);

/// Returns the minimum or a value < 0 if it can not be calculated.
double qquotes_min (Qquotes *this);

/// Returns the average or a value < 0 if it can not be calculated.
double qquotes_avg (Qquotes *this);

///
char *qquotes_to_str_new(Qquotes *this);

#endif
