// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global definitions.

#ifndef DEFS_H
  #define DEFS_H

#include "dmc/std.h"

///
enum State {
  CODE0, // Normal code at the beginning
  CODE, // Normal code
  TEMPLATE_BEGIN, // /*. has been read
  TEMPLATE_PARAM, // Reading template parameters
  TEMPLATE_FIRST_WRITE_MARK, // Searching first write mark
  TEMPLATE_SECOND_WRITE_MARK, // Searchin second write mark
  ERROR // An error happened
};

///
enum TypeTemplate {
  RECORD // Template type record
};

#endif
