// Copyright 03-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Symbol data.

#ifndef READ_SYMBOL_H
  #define READ_SYMBOL_H

#include "read/type.h"

///
typedef struct read_symbol Symbol;

/// NOTE: Values for read only. 'mark' is set to FALSE and modified to TRUE
///       when:
///   - In main code: It is public.
///   - In secondary code: It is used.
struct read_symbol {
  char *id;
  Type *type;
  int mark;
};

/// Constructor
Symbol *symbol_new (char *id, Type *type);

#endif
