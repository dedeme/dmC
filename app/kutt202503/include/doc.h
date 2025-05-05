// Copyright 17-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Documentation maker.

#ifndef DOC_H
  #define DOC_H

#include "kut/arr.h"
#include "kut/opt.h"
#include "data/type.h"

/// Returns the JSON documentation of module 'md'.
/// The return is an array of 'docE'.
char *doc_make (char *fpath, char *id, char *tx);

#endif

