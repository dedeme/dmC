// Copyright 29-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in functions.

#ifndef BUILTIN_BUILTIN_H
  #define BUILTIN_BUILTIN_H

/// Returns imports for js module header.
///   to_root: web root from module (./, ../, ../../, etc).
char *builtin_module_head (char *to_root);

/// Generate built-in js files.
///   path: Directory '_js' path.
void builtin_generate (char *path);

#endif
