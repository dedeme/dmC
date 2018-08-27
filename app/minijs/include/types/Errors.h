// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Errors and warnings
/// <p>
/// Fields:
///   errors -> Errors in source code (until ten and in reverse order)
///   warnings -> Warnings in source code (until ten and in reverse order)

#ifndef TYPES_ERRORS_H
  # define TYPES_ERRORS_H

#include "types/Lerror.h"

/*.-.*/

///
typedef struct errors_Errors Errors;

///
Errors *errors_new(Lerror *errors, Lerror *warnings);

///
Lerror *errors_errors(Errors *this);

///
Lerror *errors_warnings(Errors *this);

/*.-.*/

/// Adds an error if there are less than ten.
Errors *errors_add_error(Errors *this, Error *e);

/// Adds a errors until ten.
Errors *errors_add_errors(Errors *this, Lerror *es);

/// Adds a warning if there are less than ten.
Errors *errors_add_warning(Errors *this, Error *w);

/// Adds a wranings until ten.
Errors *errors_add_warnings(Errors *this, Lerror *ws);



#endif

