// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token 'Symbol'.

#ifndef SYMBOL_H
  #define SYMBOL_H

#include "dmc/async.h"
#include "DEFS.h"

enum symbol_SYSTEM {
  symbol_IMPORT, symbol_IF, symbol_ELSE, symbol_ELIF, symbol_EQUALS,
  symbol_FUNCTION, symbol_AMPERSAND, symbol_NOP, symbol_EVAL, symbol_RUN,
  symbol_MRUN, symbol_DATA, symbol_SYNC, symbol_BREAK, symbol_LOOP,
  symbol_WHILE, symbol_FOR, symbol_CONTINUE, symbol_RECURSIVE, symbol_ASSERT,
  symbol_EXPECT, symbol_THIS, symbol_STACK, symbol_STACK_CHECK,
  symbol_STACK_OPEN, symbol_STACK_CLOSE, symbol_STACK_STOP,

  symbol_PLUS, symbol_TO_STR, symbol_REF_OUT,

  symbol_MAP_, symbol_BLOB_, symbol_THREAD_, symbol_ITERATOR_,
  symbol_FILE_, symbol_ISERVER_, symbol_ISERVER_RQ_, symbol_EXC_,

  symbol_SYSTEM_COUNT
};

///
typedef Int Symbol;

///
void symbol_init (void);

/// Creates a new Symbol with equals id and name.
Symbol symbol_new (char *name);

///
int symbol_eq (Symbol this, Symbol other);

///
char *symbol_to_str (Symbol this);

///
typedef struct symbol_SymbolKv SymbolKv;

///
SymbolKv *symbolKv_new (Symbol key, Symbol value);

///
Symbol symbolKv_key (SymbolKv *this);

///
Symbol symbolKv_value (SymbolKv *this);

#endif
