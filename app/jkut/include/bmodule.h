// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in modules management.

#ifndef BMODULE_H
  #define BMODULE_H

/// Returns TRUE if 'md_name' is a valid module symbol.
int bmodule_exists (char *md_sym);

/// Returns TRUE if the module 'md_sym' contains the function 'fn_sym'
int bmodule_has (char *md_sym, char *fn_sym);

#endif
