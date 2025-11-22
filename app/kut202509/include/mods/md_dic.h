// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in module dic.

#ifndef MODS_MD_DIC_H
  #define MODS_MD_DIC_H

#include "bfunction.h"

/// Returns Bfunction with name 'fmane'.
/// Throw EXC_KUT if 'fname' does not exist.
Bfunction md_dic_get (char *fname);

/// Returns the expression of 'dic' with key 'key'.
///   dic: Map<Exp>.
///   key: Key to find.
/// Throws EXC_KUT if 'key' does not exist.
Exp *md_dic_fget (Map *dic, char *key);

#endif
