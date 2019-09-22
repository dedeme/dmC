// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Stack intepreter.

#ifndef MACHINE_H
  #define MACHINE_H

#include "dmc/async.h"
#include "dmc/List.h"
#include "Heap.h"

typedef struct machine_Machine Machine;

/// List<Machine>
List *machine_pmachines (Machine *this);

/// Arr<Token>
Arr *machine_stack (Machine *this);

Heap *machine_heap (Machine *this);

///
Token *machine_prg (Machine *this);

///
void machine_fail (Machine *this, char *msg);

///
void machine_push (Machine *this, Token *tk);

/// Returns the last element of stack.
Token *machine_peek (Machine *this);

/// Returns the last element of stack, if it is of type 'type'.
/// Otherwise the elements is not removed and an exception is raised.
Token *machine_peek_exc (Machine *this, enum token_Type type);

/// Returns the last element of stack, if it is of type 'type'. Otherwise
/// returns NULL.
Token *machine_peek_opt (Machine *this, enum token_Type type);

/// Removes and returns the last element of stack.
Token *machine_pop (Machine *this);

/// Removes and returns the last element of stack, if it is of type 'type'.
/// Otherwise the elements is not removed and an exception is raised.
Token *machine_pop_exc (Machine *this, enum token_Type type);

/// Removes and returns the last element of stack, if it is of type 'type'.
/// Otherwise the element is not removed and returns NULL.
Token *machine_pop_opt (Machine *this, enum token_Type type);

/// Run a process. The new process shares stack with the calling process.
///   source   : Source of 'prg'. Programatic 'prg's have as souce "".
///   pmachines: List<Machine> Parent machines.
///   prg      : Token of type token_LIST.
///   return   : A new machine creates by this function.
Machine *machine_process (char *source, List *pmachines, Token *prg);

/// Runs a process. The new process uses a new own stack.
///   source   : Source of 'prg'. Programatic 'prg's have as souce "".
///   pmachines: List<Machine> Parent machines.
///   prg      : Token of type token_LIST.
///   return   : A new machine creates by this function.
Machine *machine_isolate_process (char *source, List *pmachines, Token *prg);

#endif
