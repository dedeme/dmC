// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Errors and warnings
/// <p>
/// Fields:
///   errors -> Errors in source code (until ten and in reverse order)
///   warnings -> Warnings in source code (until ten and in reverse order)

#ifndef TYPES_FAILS_H
  # define TYPES_FAILS_H

#include "types/Afail.h"
#include "stdbool.h"

/*.-.*/

///
typedef struct fails_Fails Fails;

///
Afail *fails_errors(Fails *this);

///
Afail *fails_warnings(Fails *this);

/*.-.*/

/// Creates an empty errors
Fails *fails_new();

/// Create a new errors with an error
Fails *fails_new_error(Fail *e);

/// Create a new errors with a warning
Fails *fails_new_warning(Fail *e);

/// Returns true if there are some error
bool fails_is_error(Fails *this);

/// Adds an error if there are less than ten.
void fails_add_error(Fails *this, Fail *e);

/// Adds a errors until ten.
void fails_add_errors(Fails *this, Afail *es);

/// Adds a warning if there are less than ten.
void fails_add_warning(Fails *this, Fail *w);

/// Adds a wranings until ten.
void fails_add_warnings(Fails *this, Afail *ws);



#endif

