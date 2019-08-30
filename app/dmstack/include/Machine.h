// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Stack intepreter.

#ifndef MACHINE_H
  #define MACHINE_H

#include "dmc/std.h"
#include "dmc/List.h"
#include "Token.h"

typedef struct machine_Machine Machine;

/// List<Map<Token>>
List *machine_pheaps (Machine *this);

/// Arr<Token>
Arr *machine_stack (Machine *this);

/// Map<Token>
Map *machine_heap (Machine *this);

///
Token *machine_prg (Machine *this);

///
void machine_fail (Machine *this, char *msg);

///
void machine_push (Machine *this, Token *tk);

/// Removes and returns the last element of stack.
Token *machine_pop (Machine *this);

/// Removes and returns the last element of stack, if it is of type 'type'.
/// Otherwise the elements is not removed and an exception is raised.
Token *machine_pop_exc (Machine *this, enum token_Type type);

/// Returns Opt<Token> - Removes and returns the last element of stack, if it
/// is of type 'type'. Otherwise the elements is not removed and returns
/// NULL.
Token *machine_pop_opt (Machine *this, enum token_Type type);

/// Creates a new variable or raise a fail is 'id' is duplicate.
void machine_create_var (Machine *this, char *id, Token *value);

/// Changes token if a variable or raise a fail is 'id' does not exist.
void machine_set_var (Machine *this, char *id, Token *value);

/// Deletes a variable if it exists. Never raises fail.
void machine_del_var (Machine *this, char *id);

/// Returns Map<Token> Tokens exported (in the heap) at the end of process.
///   pheaps: List<Map<Token>> Parent heaps.
///   prg: Token of type token_LIST.
Map *machine_process (List *pheaps, Token *prg);

#endif
