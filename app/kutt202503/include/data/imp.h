// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Import data.

#ifndef DATA_IMP_H
  #define DATA_IMP_H

/// Structure.
typedef struct {
  int ln;      // Line number of definition statement.
  char *mpath; // Relative user module path (without .ktt).
  char *id;    // Unique indentifier.
} Imp;

/// Constructor
///   ln   : Line number of definition statement.
///   mpath: Relative user module path (without .ktt).
///   id   : Unique indentifier.
Imp *imp_new (int ln, char *mpath, char *id);

///
char *imp_to_js (Imp *this);

///
Imp *imp_from_js (char *js);

#endif
