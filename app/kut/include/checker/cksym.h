// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code checker.

#ifndef CHECKER_CKSYM_H
  #define CHECKER_CKSYM_H

typedef struct cksym_Cksym Cksym;

/// Constructor.
/// The parameter 'used' is set to FALSE.
///   id: Symbol identifier.
///   fix: Code file index.
///   nline: Line where 'id' appears for first time.
Cksym *cksym_new (char *id, int fix, int nline);

/// Symbol identifier.
char *cksym_get_id (Cksym *this);

/// Code file index.
int cksym_get_fix (Cksym *this);

/// Line where 'this' appears for first time.
int cksym_get_nline (Cksym *this);

/// 'TRUE' if 'id' was used.
int cksym_is_used (Cksym *this);

/// Set the parameter 'used' to 'TRUE'.
void cksym_set_used (Cksym *this);
#endif
