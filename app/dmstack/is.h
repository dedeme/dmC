// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef TKREADER_H
  #define TKREADER_H

#include "dmc/async.h"
#include "Reader.h"

/// Returns Opt<Token> - Reads one token from machine_prg(). If there are no
/// more tokens, returns 'opt_empty()'.
/// Throw 'ST_EXC' if fails.
Opt *tkreader_next(Reader *reader);

#endif
// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Variables heap.

#ifndef HEAP_H
  #define HEAP_H

#include "dmc/async.h"
#include "Token.h"

///
typedef struct heap_Entry HeapEntry;

///
Symbol *heapEntry_symbol (HeapEntry *this);

///
Token *heapEntry_token (HeapEntry *this);

///
typedef struct heap_Heap Heap;

///
Heap *heap_new (void);

/// Adds a new element to 'this'
void heap_add (Heap *this, Symbol *s, Token *tk);

/// Returns a token o NULL
Token *heap_get (Heap *this, Symbol *s);

/// Arr<HeapEntry>
Arr *heap_entries (Heap *this);

#endif
// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Entry point.

#ifndef DMSTACK_H
  #define DMSTACK_H

#include "dmc/async.h"

///
int main (int argc, char *argv[]);

#endif
// Copyright 29-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Errors generator.

#ifndef FAILS_H
  #define FAILS_H

#include "dmc/async.h"
#include "Machine.h"

///
void fails_init (void);

/// Register the machine of current thread.
void fails_register_machine (Machine *m);

/// Unregister the machine of current thread.
void fails_unregister_machine (void);

///
void fails_from_exception (Exc *ex);

/// Error found an unexpected type in stack.
///   m   : Machine
///   type: Expected type.
void fails_type (Machine *m, enum token_Type type);

/// Error found an unexpected type in stack.
///   m    : Machine
///   n    : Number of errors.
///   types: array of expected types. For example:
///          (enum token_Type[]){token_INT, token_FLOAT}
void fails_types (Machine *m, int n, enum token_Type *types);

/// Error found an unexpected type.
///   m    : Machine
///   type : Expected type.
///   token: Failed token.
void fails_type_in (Machine *m, enum token_Type type, Token *token);

/// Error found an unexpected type.
///   m    : Machine
///   n    : Number of errors.
///   types: array of expected types. For example:
///          (enum token_Type[]){token_INT, token_FLOAT}
///   token: Failed token.
void fails_types_in (Machine *m, int n, enum token_Type *types, Token *token);

/// Error in list size.
void fails_size_list (Machine *m, Arr *list, int expected);

/// Error in field type of list.
void fails_type_list (Machine *m, Arr *list, int ix, enum token_Type expected);

/// Error index out of range.
///   m  : Machine
///   min: Minimum value of range, inclusive.
///   max: Maximum value of range, inclusive.
///   ix : Value out of range.
void fails_range (Machine *m, int min, int max, int ix);

/// Throws a fails_range if ix < min or ix > max
///   m  : Machine
///   min: Minimum value of range, inclusive.
///   max: Maximum value of range, inclusive.
///   ix : Value out of range.
void fails_check_range (Machine *m, int min, int max, int ix);

///
void *fails_read_pointer (Machine *m, char *id, Token *pointer);

#endif
// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef READER_H
  #define READER_H

#include "dmc/async.h"
#include "dmc/List.h"
#include "Token.h"

///
typedef struct reader_Reader Reader;

/// Creates a new stack machine
///   source : Identifier (File without extension or 'ad hoc' identifier).
///   prg    : Program to process.
///   nline  : Current line number.
Reader *reader_new (char *source, char *prg);

/// Creates a new stack machine
///   parent : Reader parent.
///   prg    : Program to process.
///   nline  : Current line number.
Reader *reader_new_from_reader (Reader *parent, char *prg, int nline);

/// Returns a token type token_LIST.
Token *reader_process (Reader *this);

///
int reader_is_file (Reader *is_file);

///
char *reader_source (Reader *this);

///
char *reader_prg (Reader *this);

///
int reader_prg_ix (Reader *this);

///
void reader_set_prg_ix (Reader *this, int value);

/// Used to pass an extra Tokebn.
/// Its return can be NULL.
Token *reader_next_tk (Reader *this);

///
void reader_set_next_tk (Reader *this, Token *tk);

///
int reader_nline (Reader *this);

///
void reader_set_nline (Reader *this, int value);

/// Calculate symbol id of 'tk'. 'tk' is a token_SYMBOL.
///   this: The reader.
///   prg : Arr<Token> symbols read until now.
///   tk  : Token type token_SYMBOL.
Token *reader_symbol_id (Reader *this, Arr *prg, Token *tk);

///
void reader_fail (Reader *this, char *msg);

#endif
// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Primitives functions,

#ifndef PRIMITIVES_H
  #define PRIMITIVES_H

#include "dmc/async.h"
#include "Machine.h"

///
typedef void (*primitives_Fn) (Machine *m);

///
void primitives_init (void);

/// Returns Opt<primitives_Fn>
Opt *primitives_get (char *module, char *id);

/// Returns Opt<Map[Token]> - Functions from a module.
Opt *primitives_module (char *module);

#endif
// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// File system management.

#ifndef PRIMITIVES_MODFILE_H
  #define PRIMITIVES_MODFILE_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modfile_mk (void);

#endif
// Copyright 19-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Date-time management.

#ifndef PRIMITIVES_MODTIME_H
  #define PRIMITIVES_MODTIME_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modtime_mk (void);

#endif
// Copyright 31-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Logical operations.

#ifndef MODULES_MODGLOBAL1_H
  #define MODULES_MODGLOBAL1_H

#include "dmc/async.h"
#include "Machine.h"

/// Operation &&
void modglobal1_and (Machine *m);

/// Operation ||
void modglobal1_or (Machine *m);

/// Operation !
void modglobal1_not (Machine *m);

/// Operation >
void modglobal1_greater (Machine *m);

/// Operation >=
void modglobal1_greater_eq (Machine *m);

/// Operation <
void modglobal1_less (Machine *m);

/// Operation <=
void modglobal1_less_eq (Machine *m);

#endif
// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Internet server.

#ifndef PRIMITIVES_MODISERVER_H
  #define PRIMITIVES_MODISERVER_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modiserver_mk (void);

#endif
// Copyright 26-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator module.

#ifndef PRIMITIVES_MODIT_H
  #define PRIMITIVES_MODIT_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modit_mk (void);

#endif
// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// Math module.

#ifndef MODULES_MODMATH_H
  #define MODULES_MODMATH_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modmath_mk (void);

#endif
// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// List module.

#ifndef MODULES_MODLIST_H
  #define MODULES_MODLIST_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modlist_mk (void);

#endif
// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// Map module.

#ifndef MODULES_MODMAP_H
  #define MODULES_MODMAP_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modmap_mk (void);

#endif
// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// Float module.

#ifndef MODULES_MODFLOAT_H
  #define MODULES_MODFLOAT_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modfloat_mk (void);

#endif
// Copyright 03-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// Blob module.

#ifndef MODULES_MODBLOB_H
  #define MODULES_MODBLOB_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modblob_mk (void);

#endif
// Copyright 02-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// Stack management module.

#ifndef MODULES_MODSTK_H
  #define MODULES_MODSTK_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modstk_mk (void);

#endif
// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global scope module

#ifndef MODULES_MODGLOBAL_H
  #define MODULES_MODGLOBAL_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modglobal_mk (void);

#endif
// Copyright 22-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Path management.

#ifndef PRIMITIVES_MODPATH_H
  #define PRIMITIVES_MODPATH_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modpath_mk (void);

#endif
// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cryptographic module

#ifndef PRIMITIVES_MODCRYP_H
  #define PRIMITIVES_MODCRYP_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modcryp_mk (void);


#endif
// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// B64 module.
#ifndef PRIMITIVES_MODB64_H
  #define PRIMITIVES_MODB64_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modb64_mk (void);

#endif
// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Obj module.

#ifndef PRIMITIVES_MODOBJ_H
  #define PRIMITIVES_MODOBJ_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modobj_mk (void);

#endif
// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// JSON module.

#ifndef PRIMITIVES_MODJS_H
  #define PRIMITIVES_MODJS_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modjs_mk (void);

///
void modjs_from_list (Machine *m);

///
void modjs_to_list (Machine *m);

///
void modjs_from_map (Machine *m);

///
void modjs_to_map (Machine *m);

#endif
// Copyright 07-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Container operations.

#ifndef MODULES_MODGLOBAL2_H
  #define MODULES_MODGLOBAL2_H

#include "dmc/async.h"
#include "Machine.h"

/// Resturns container size.
void modglobal2_size (Machine *m);

/// Return an element of container and removes the container.
void modglobal2_get (Machine *m);

/// Sets an element of container and removes the container.
/// Stack has: container - index - value.
void modglobal2_set (Machine *m);

/// Sets an element of container and keeps the container.
/// Stack has: container - index - value.
void modglobal2_setplus (Machine *m);

/// Sets an element of container using its value and removes the container.
/// Stack has: container - index - function.
void modglobal2_up (Machine *m);

/// Sets an element of container using its value and keeps the container.
/// Stack has: container - index - function.
void modglobal2_upplus (Machine *m);

/// Return its element from a reference and removes the reference.
void modglobal2_ref_get (Machine *m);

/// Sets a element in a reference and removes the reference.
/// Stack has: container - value.
void modglobal2_ref_set (Machine *m);

/// Sets a element in a reference and keeps the reference.
/// Stack has: container - value.
void modglobal2_ref_setplus (Machine *m);

/// Sets a element in a reference using its value and removes the reference.
/// Stack has: container - function.
void modglobal2_ref_up (Machine *m);

/// Sets a element in a reference using its value and keeps the reference.
/// Stack has: container - function.
void modglobal2_ref_upplus (Machine *m);

#endif
// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// String module.

#ifndef MODULES_MODSTR_H
  #define MODULES_MODSTR_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modstr_mk (void);

#endif
// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Wrappers.

#ifndef PRIMITIVES_MODWRAP_H
  #define PRIMITIVES_MODWRAP_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modwrap_mk (void);

#endif
// Copyright 30-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module of simple mathematical operations.

#ifndef MODULES_MODGLOBAL0_H
  #define MODULES_MODGLOBAL0_H

#include "dmc/async.h"
#include "Machine.h"

/// Operation +
void modglobal0_add (Machine *m);

/// Operation -
void modglobal0_sub (Machine *m);

/// Operation *
void modglobal0_mul (Machine *m);

/// Operation /
void modglobal0_div (Machine *m);

/// Operation %
void modglobal0_mod (Machine *m);

/// Increment
void modglobal0_incr (Machine *m);

/// Decrement
void modglobal0_decr (Machine *m);

#endif
// Copyright 20-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Clock module.

#ifndef PRIMITIVES_MODCLOCK_H
  #define PRIMITIVES_MODCLOCK_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modclock_mk (void);

#endif
// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

// Int module.

#ifndef MODULES_MODINT_H
  #define MODULES_MODINT_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modint_mk (void);

#endif
// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// System module.

#ifndef PRIMITIVES_MODSYS_H
  #define PRIMITIVES_MODSYS_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modsys_mk (void);

#endif
// Copyright 26-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global definitions.

#ifndef DEFS_H
  #define DEFS_H

#include <stdint.h>
#include "dmc/async.h"

#define VERSION "201909"

typedef intptr_t Int;

#endif
// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program arguments management.

#ifndef ARGS_H
  #define ARGS_H

#include "dmc/std.h"

/// If there is any error, show a message and returns '1'.
int args_init (int argc, char *args[]);

/// Returns Opt<char>. Arguments of dmstack.
/// Params are:
///   dms: Dms file.
Opt *args_param (char *key);

/// Returns Arr<char>. Argumentes of dms file.
Arr *args_dms_params (void);

#endif
// Copyright 16-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Imports library

#ifndef LIB_H
  #define LIB_H

#include "dmc/async.h"
#include "Heap.h"

///
typedef struct lib_Entry LibEntry;

///
Symbol *libEntry_symbol (LibEntry *this);

///
Heap *libEntry_heap (LibEntry *this);

///
typedef struct lib_Lib Lib;

///
Lib *lib_new (void);

/// Adds a new element to 'this'
void lib_add (Lib *this, Symbol *s, Heap *heap);

/// Returns a Heap o NULL
Heap *lib_get (Lib *this, Symbol *s);

/// Arr<LibEntry>
Arr *lib_entries (Lib *this);

#endif

// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token 'Symbol'.

#ifndef SYMBOL_H
  #define SYMBOL_H

#include "dmc/async.h"

///
typedef struct symbol_Symbol Symbol;

/// Creates a new Symbol with an id different to name.
Symbol *symbol_new_id (char *id, char *name);

/// Creates a new Symbol with equals id and name.
Symbol *symbol_new (char *name);

/// Returns the name of 'this'
char *symbol_name (Symbol *this);

/// Returns the id of 'this'
char *symbol_id (Symbol *this);

/// Returns the symbol hash.
int symbol_hash (Symbol *this);

///
Symbol *symbol_clone (Symbol *this);

///
int symbol_eq (Symbol *this, Symbol *other);

///
char *symbol_to_str (Symbol *this);

#endif
// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Minimal meaning unit of program.

#ifndef TOKEN_H
  #define TOKEN_H

#include "dmc/async.h"
#include "DEFS.h"
#include "Symbol.h"

// typedef struct machine_Machine Machine;

///
enum token_Type {
  token_INT, token_FLOAT, token_STRING, token_BLOB, token_LIST, token_SYMBOL
};

///
typedef struct token_Token Token;

///
Token *token_new_int (int line, Int value);

///
Token *token_new_float (int line, double value);

///
Token *token_new_string (int line, char *value);

///
Token *token_new_blob (int line, Bytes *bs);

/// 'value' is Arr<Token>
Token *token_new_list (int line, Arr *value);

///
Token *token_new_symbol (int line, Symbol *value);

///
enum token_Type token_type (Token *this);

///
int token_line (Token *this);

///
Int token_int (Token *this);

///
double token_float (Token *this);

///
char *token_string (Token *this);

///
Bytes *token_blob (Token *this);

/// Returns Arr<Token>
Arr *token_list (Token *this);

///
Symbol *token_symbol (Token *this);

/// Returns a new token equals to 'this'
Token *token_clone (Token *this);

///
int token_eq (Token *this, Token *other);

///
char *token_to_str (Token *this);

///
char *token_type_to_str (enum token_Type type);

///
Token *token_from_pointer (char *id, void *pointer);

#endif
// Copyright 12-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Import cache.

#ifndef IMPORTS_H
  #define IMPORTS_H

#include "dmc/async.h"
#include "Machine.h"

///
void imports_init ();

/// Annotation of an import on way.
void imports_put_on_way (Symbol *key);

/// Removes nnotation of an import on way.
void imports_quit_on_way (Symbol *key);

/// Returns if an import is on way.
int imports_is_on_way (Symbol *key);

///
void imports_add (Symbol *key, Heap *heap);

/// Returns an import from library or NULL.
Heap *imports_get (Symbol *key);

/// Returns system heap.
Heap *imports_base (void);

/// Reads an import symbol. If reading fails return an
/// error message in 'key'. Otherwise key is "".
Kv *imports_read_symbol (Token *tk);

#endif
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
// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array structure.

#ifndef DMC_ARR_H
  #define DMC_ARR_H

#include "DEFS.h"
#include "Tp.h"
#include "Tp3.h"

typedef struct js_Js Js;

typedef struct it_It It;

///
typedef struct arr_Arr Arr;

/// Creates a new Array with buffer size of 15 elements.
Arr *arr_new (void);

/// 'buffer' must be > 0.
Arr *arr_bf_new (int buffer);

/// Creates a new array from several elements.
Arr *arr_new_from (void *e, ...);

/// Creates a new array from a C array. For example:
///   Arr *a = arr_new_c(3, (void *[]){"c", "d", "e"});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
Arr *arr_new_c (int size, void **es);

/// Returns a new array with elements of 'this'.
Arr *arr_copy (Arr *this);

///
int arr_size (Arr *this);

/// Resturn the element at position ix.
void *arr_get (Arr *this, int ix);

/// Return a pointer to the first element of 'this'
void **arr_start (Arr *this);

/// Returns a pointer to the next element after the last element of 'this'.
/// 'arr_end' does not point to a valid element.
void **arr_end (Arr *this);

/// Adds an element at the end of 'this'. 'e' will be freed by 'this'.
void arr_push (Arr *this, void *e);

/// Returns and removes the last element.
void *arr_pop (Arr *this);

/// Returns the las element.
void *arr_peek (Arr *this);

/// Sets the element at position ix.
void arr_set (Arr *this, int ix, void *e);

/// Inserts an element at position ix.
void arr_insert (Arr *this, int ix, void *e);

/// Removes an element at position ix. Buffer size of 'this' does not change.
void arr_remove (Arr *this, int ix);

/// Adds pointer to elements of 'other' to 'this'.
void arr_cat (Arr *this, Arr *other);

/// Inserts pointer to elements of 'other' at 'ix'
void arr_insert_arr (Arr *this, int ix, Arr *other);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void arr_remove_range (Arr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void arr_clear (Arr *this);

/// Removes every element of 'this'.
void arr_bf_clear (Arr *this, int buffer);

/// Reverses elements of 'this'.
void arr_reverse (Arr *this);

/// Sorts 'this' ascendantly using the function 'greater' that returns 'true'
/// if 'e1' > 'e2'.
void arr_sort (Arr *this, int (*greater)(void *e1, void *e2));

/// arr_shuflle remix 'this' elements. It should be used after calling
/// rnd_init() or sys_init().
void arr_shuffle (Arr *this);

/// Returns '1' if every element of 'this' yields '1' with 'pred'.
int arr_all (Arr *this, int (*pred)(void *e));

/// Returns '1' if some element of 'this' yields '1' with 'pred'.
int arr_any (Arr *this, int (*pred)(void *e));

/// Returns the index of the first elements which returns 'true'
/// with 'pred', or -1 if such element does not exist.
int arr_index (Arr *this, int (*pred)(void *e));

/// Returns the index of the last elements which returns 'true'
/// with 'pred', or -1 if such element does not exist.
int arr_last_index (Arr *this, int (*pred)(void *e));

/// arr_filter_in removes every element which returns 'false' with 'pred'.
void arr_filter_in (Arr *this, int (*pred)(void *e));

/// Returns a new Arr. See it_take.
Arr *arr_take (Arr *this, int n);

/// Returns a new Arr. See it_takef.
Arr *arr_takef (Arr *this, int (*predicate)(void *e));

/// Returns a new Arr. See it_drop.
Arr *arr_drop (Arr *this, int n);

/// Returns a new Arr. See it_dropf.
Arr *arr_dropf (Arr *this, int (*predicate)(void *e));

/// Returns a new Arr. See it_filter.
Arr *arr_filter_to (Arr *this, int (*predicate)(void *e));

/// Returns a new Arr. See it_map.
Arr *arr_map (Arr *this, void *(*converter)(void *e));

/// Returns a new Arr. See it_map2.
Arr *arr_map2 (Arr *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

/// Returns a new Arr. Returns Arr[Tp]. See it_zip.
Arr *arr_zip (Arr *a1, Arr *a2);

/// Returns a new Arr. Returns Arr[Tp3]. See it_zip3.
Arr *arr_zip3 (Arr *a1, Arr *a2, Arr *a3);

/// Returns Tp[Arr, Arr] from an Arr[Tp]. 'Return_e1' contains elements of
/// source 'Tp_e1' and 'return_e2' elementso of 'Tp_e2'.
Tp *arr_unzip (Arr *this);

/// Returns Tp[Arr, Arr, Arr] from an Arr[Tp2]. 'Return_e1' contains elements
/// of source 'Tp_e1', 'return_e2' elements of 'Tp_e2' and 'return_e3'
/// elements of 'Tp_e3'.
Tp3 *arr_unzip3 (Arr *this);

/// Returns Tp[Arr, Arr] (duplicates, rest) See it_duplicates.
Tp *arr_duplicates (Arr *this, int (feq)(void *e1, void *e2));

/// Creates an iterator over 'this'.
It *arr_to_it (Arr *this);

/// Creates an Arr from 'it'.
Arr *arr_from_it (It *it);

/// Returns a Js from an element of 'this'
Js *arr_to_js (Arr *this, Js *(*to)(void *e));

/// Parses a Js to an element of 'this'.
Arr *arr_from_js (Js *js, void *(*from)(Js *jse));

#endif
// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage exceptions.
/// Only functions 'exc_init', 'exc_stack'  and 'exc_throw' must be used
/// directly. The rest must be used through the macros TRY-CATCH-_TRY and
/// THROW-_THROW. (See "?libdmc@dmc/DEFS#hp:TRY").

#ifndef DM_EXC_H
  #define DM_EXC_H

#include <setjmp.h>

typedef struct arr_Arr Arr;

///
typedef struct exc_Exc Exc;

/// Returns exception type. Predefined types are:
///   exc_generic_t
///   exc_range_t
///   exc_illegal_argument_t
///   exc_illegal_state_t
///   exc_io_t
char *exc_type (Exc *this);

/// Returns exception message.
char *exc_msg (Exc *this);

/// Returns Arr[char]. The exception stack trace.
Arr *exc_stack (Exc *this);

/// Intializes jumps buffer. This function has to be called before using macros
/// TRY-CATCH-FINALLY-_TRY or THROW-_THROW.
void exc_init ();

/// Initializes thread data. It is intended to beeng use only
/// by 'async_thread'.
void exc_thread_init (void);

/// Removes a thread data. It is intended to beeng use only by 'async_thread'.
void exc_thread_end (void);

/// Adds a exception to buffer of current Exc in current thread.
void exc_add (jmp_buf *jump);

/// Returns current Exc in current thread.
Exc *exc_get(void);

/// Removes the top of jumps buffer of current Exc in current thread.
void exc_remove ();

/// Sends an exception.
/// If no TRY block has been defined it stops the program.
///   type   : Excepion type.
///   message: Message to show.
///   file   : Error file.
///   func   : Error function.
///   line   : Error line number.
void exc_throw (
  char *type, char *message, char *file, char *func, int line
);

///
#define exc_generic_t "generic"

///
#define exc_range_t "range"

/// Exception for index out of range.
///   begin: Lower limit inclusive.
///   end  : Upper limit inclusive.
///   index: The index out of range (< begin and > end).
char *exc_range (int begin, int end, int index);

///
#define exc_illegal_argument_t "argument"

/// Exception for argument with a wrong value.
///   argument_name: Name of wrong argument.
///   expected     : Value expected.
///   actual       : Actual value.
char *exc_illegal_argument (char *argument_name, char *expected, char *actual);

///
#define exc_illegal_state_t "state"

/// Exception for attempting to use an object in wrong state.
///   cause: Description of problem.
char *exc_illegal_state (char *cause);

///
#define exc_io_t "io"

/// Exception for Input - Output error.
///   cause: Description of problem.
char *exc_io (char *cause);

#endif
// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Structure for working with bytes. Example:
///   Bytes *bs = bytes_new();
///   const unsigned char bss[] = {0, 23, 116, 225};
///   bytes_add_bytes(bs, bss, 4);
///   char b41 = b41_encodeBytes(bs);
///   assert(!strcmp("RRoixx", b41));
///   bytes_free(bs);

#ifndef DMC_BYTES_H
  #define DMC_BYTES_H

#include "Js.h"

///
typedef struct bytes_Bytes Bytes;

///
Bytes *bytes_new(void);

///
Bytes *bytes_bf_new(int length);

/// Returns a new allocated 'Bytes' whitch is copy of 'bs'.
Bytes *bytes_from_bytes(unsigned char *bs, int length);

/// Returns a 'Bytes' whitch is copy of 's' without the ending zero.
Bytes *bytes_from_str(char *s);

///
unsigned char *bytes_bs(Bytes *this);

///
int bytes_len(Bytes *this);

/// Adds to 'this' a new copy of 'bs'.
void bytes_add_bytes(Bytes *this, unsigned char *bs, int length);

/// Adds to 'this' a new copy of 'another'.
void bytes_add(Bytes *this, Bytes *another);

/// Adds to 'this' a copy of 's' without the ending zero.
void bytes_add_str(Bytes *this, char *s);

///
Js *bytes_to_js(Bytes *this);

///
Bytes *bytes_from_js(Js *js);

#endif
// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of doubles.

#ifndef DMC_DARR_H
  #define DMC_DARR_H

#include "Js.h"

///
typedef struct darr_Darr Darr;

///
Darr *darr_new(void);

/// buffer must be > 0.
Darr *darr_bf_new(int buffer);

/// Creates a new array from a C array. For example:
///   Darr *a = darr_new_c(3, (double[]){1.0, 1.8, 1.3});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
Darr *darr_new_c (int size, double *es);

/// Returns a new Darr with elements from 0 to 'ix' (exclusive),
Darr *darr_left(Darr *this, int ix);

/// Returns a new Darr with elements from 'ix' (inclusive) to end of 'this'.
Darr *darr_right(Darr *this, int ix);

/// Returns a new Darr with elements from 'begin' (inclusive) to
/// to 'end' (exclusive),
Darr *darr_sub(Darr *this, int begin, int end);

///
Darr *darr_copy(Darr *this);

///
int darr_eq(Darr *this, Darr *other, double gap);

///
int darr_size(Darr *this);

/// If ix is < 0 then is changed to 'darr_size - ix'.
double darr_get(Darr *this, int ix);

///
double *darr_start(Darr *this);

///
double *darr_end(Darr *this);

///
void darr_push(Darr *this, double e);

/// If ix is < 0 then is changed to 'darr_size - ix'.
void darr_set(Darr *this, int ix, double e);

/// If ix is < 0 then is changed to 'darr_size - ix'.
void darr_insert(Darr *this, int ix, double e);

/// If ix is < 0 then is changed to 'darr_size - ix'.
void darr_remove(Darr *this, int ix);

///
void darr_cat(Darr *this, Darr *other);

/// If ix is < 0 then is changed to 'darr_size - ix'.
void darr_insert_arr(Darr *this, int ix, Darr *other);

/// If begin or end are < 0 then is changed to 'darr_size - itsValue'.
void darr_remove_range(Darr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void darr_clear (Darr *this);

/// Removes every element of 'this'.
void darr_bf_clear (Darr *this, int buffer);

///
void darr_reverse(Darr *this);

///
void darr_sort(Darr *this);

///
Js *darr_to_js(Darr *this);

///
Darr *darr_from_js(Js *js);

#endif
// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash (immutable) structure.

#ifndef DM_HASH_H
  #define DM_HASH_H

#include "dmc/Arr.h"
#include "dmc/Opt.h"

///
typedef struct hash_Hash Hash;

/// Initializates a Hash. Hash can be cast to Map<Kv>.
Hash *hash_new(void);

/// Returns the number of elements. (O(n) operation).
int hash_count(Hash *this);

/// Puts 'value' with key 'key'. If key already exists, it will not
/// be deleted; but will not be used.
///   this : The Hash.
///   key  : Entry key.
///   value: New value
Hash *hash_put(Hash *this, char *key, void *value);

/// Returns 1 if 'this' has key 'key'.
int hash_has_key(Hash *this, char *key);

/// Returns the value pointed by key or 'opt_empty' if 'key' does.
/// not exist.
Opt *hash_get(Hash *this, char *key);

/// Returns a new hash with duplicates removed.
Hash *hash_compact(Hash *this);

/// Returns a new hash with the entry with key 'key' removed.
Hash *hash_remove(Hash *this, char *key);

/// Returns keys of this in a Arr[char] with duplicated keys removed.
Arr *hash_keys(Hash *this);

/// Returns an Arr<Kv> with dulicated keys removed.
Arr *hash_kvs(Hash *this);

/// Creates an iterator over 'this'. Duplicates are removed.
///   return: It<Kv>
It *hash_to_it(Hash *this);

/// Creates a Hash from a 'it'.
///   it: It<Kv>
Hash *hash_from_it(It *it);

/// Returns a Js from a value of 'this'. Duplicates are removed.
///   to: Value converter.
Js *hash_to_js(Hash *this, Js *(*to)(void *e));

/// Parses a Js to a value of 'this'.
///   from: Value converter.
Hash *hash_from_js(Js *js, void *(*from)(Js *jse));

#endif
// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Pair key - value.

#ifndef DMC_KV_H
  #define DMC_KV_H

///
typedef struct kv_Kv Kv;

///
Kv *kv_new(char *key, void *value);

///
char *kv_key(Kv *this);

///
void *kv_value(Kv *this);

#endif
// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Generator of random numbers.

#ifndef DMC_RND_H
  #define DMC_RND_H

#include "dmc/Arr.h"

///
typedef struct rnd_Box Box;

/// Intializates the random number generator.
void rnd_init(void);

/// Generates a new double between 0.0 (inclusive) and 1.0 (exclusive).
double rnd_d(void);

/// Generates a new int between 0 (inclusive) and 'top' (exclusive).
int rnd_i(int top);

/// Returns an 'Box' that iterates over 'a' elements randomly. When it finishes
/// with all the elements of 'a', restarts again.
Box *rnd_box_new(Arr *a);

/// Returns the next element of 'this'.
void *rnd_box_next(Box *this);

#endif
// Copyright 29-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of multithread programs.
/// NOTE: This file must be included instead of 'std.h'.

#ifndef DMC_ASYNC_H
  #define DMC_ASYNC_H

#include <pthread.h>
/// Necesary definition for multithreading garbage collector.
#define GC_THREADS
#include "gc.h"
#include "dmc/std.h"

/// Launchs 'fn' in a new joinable thread. Example of use:
///   void fn() { puts("Here"); }
///   pthread_t *thr = async_thread(fn);
///   async_join(thr); // Waits for thr.
/// NOTE: After calling 'async_thread' is mandatory to call 'async_join' to
///         free resources.
pthread_t *async_thread (void (*fn)(void));

/// Launchs 'fn' in a new joinable thread. Example of use:
///   void fn(char *tx) { puts(tx); }
///   pthread_t *thr = async_thread2((FPROC)fn, "Hello");
///   async_join(thr); // Waits for thr.
/// NOTE: After calling 'async_thread' is mandatory to call 'async_join' to
///         free resources.
pthread_t *async_thread2 (void (*fn)(void *), void *value);

/// Launch 'fn' in a new thread. Example of use:
///   void fn(char *tx) { puts(tx); }
///   async_thread_detached((FPROC)fn, "Hello");
void async_thread_detached (void (*fn)(void *), void *value);

/// Wait until thr finishes.
void async_join (pthread_t *thr);

///
typedef struct async_AsyncActor AsyncActor;

/// 'millis' is the latence time.
AsyncActor *asyncActor_new (int millis);

/// Executes 'fn(value)' synchronicaly. This function returns immediatly.
void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value);

/// Executes 'fn(value)' synchronicaly. This function stops the program
/// until 'fn' is finished.
void asyncActor_wait (AsyncActor *this, void (*fn)(void));

/// Finalizes 'this'. 'this' also will finish is pendant jobs.
void asyncActor_end (AsyncActor *this);

/// Waits until 'this' is finished.
void asyncActor_join (AsyncActor *this);

///
typedef struct async_AsyncTimer AsyncTimer;

/// Executes 'fn(value)' each 'millis' milliseconds.
AsyncTimer *asyncTimer_new (void (*fn)(void *), void *value, int millis);

/// Finalizes 'this'.
void asyncTimer_end (AsyncTimer *this);

#endif
// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cryptographic utilities.

#ifndef DMC_CRYP_H
  #define DMC_CRYP_H

/// Generates a b64 random key of a length 'lg'.
char *cryp_genk (int lg);

/// Codified 'k' in irreversible way, using 'lg' b64 digits..
///   k : String to codify.
///   lg: Length of result.
char *cryp_key (char *k, int lg);

/// Encodes 's' with key 'k'.
///   s: Message to encode.
///   k: Key for encoding.
char *cryp_cryp (char *s, char *k);

/// Decodes 'b64' using key 'k'. 'b64' was codified with cryp().
///   b64: Text codified with cryp().
///   k  : Key for decoding.
char *cryp_decryp (char *b64, char *k);

/// Encodes automatically 's' with a random key of 'nk' digits.
///   nK: Number of digits for random key (1 to 64 both inclusive).
///   s : Text for enconding.
char *cryp_auto_cryp (char *s, int nk);

/// Decodes a text codified with autoCryp().
///   b64: Codified text.
char *cryp_auto_decryp (char *b64);

/// Encodes 's' whith key 'k' and an autoKey of length 'nK'.
///   k : Key for encoding.
///   nk: Digits to generate autoKey (1 to 40 both inclusive).
///   s : Message to encode.
char *cryp_encode (char *s, int nk, char *k);

/// Decodes a string codified with encode()
///   b64: Message encoded with encode().
///   k  : Key for encoding.
char *cryp_decode (char *b64, char *k);

#endif
// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for HTML conections between client - server.

#ifndef DMC_CGI_H
  #define DMC_CGI_H

#include "dmc/std.h"

/// Returns of cgi_get_session_data.
typedef struct cgi_Session CgiSession;

/// Returns comunication key.
char *cgiSession_key(CgiSession *this);

/// Returns connection identifier.
char *cgiSession_id(CgiSession *this);

/// Returns the standard length of keys.
int cgi_klen(void);

/// Initializes a new interface of commnications.
///   home        : Aboslute path of application directory. For example:
///                   "/peter/wwwcgi/dmcgi/JsMon"
///                   or
///                   "/home/deme/.dmCApp/JsMon" ).
///   t_expiration: Time in seconds.
void cgi_init(char *home, time_t t_expiration);

/// Returns 'home' directory.
char *cgi_home(void);

/// Sets the key which 'cgi_ok' and 'cgi_err' will use. This function is called
/// when connection or authentication.
void cgi_set_key(char *k);

/// Returns Opt<CgiSession>.
/// If 'session_id' is not valid, it returns 'opt_empty'.
Opt *cgi_get_session(char *session_id);

/// Adds an user to database.
///   admin : Admin name.
///   akey  : Admin password.
///   user  : New user name.
///   ukey  : New user password.
///   level : New user level. Level "0" is the admin one.
///   return: A boolean field {ok:true|false}, sets to true if operations
///           succeeded. A fail can come up if 'admin' authentication fails.
char *cgi_add_user(
  char *admin, char *akey,
  char *user, char *ukey, char *level
);

/// Removea an user from database.
///   admin : Admin name.
///   akey  : Admin password.
///   user  : User name to remove.
///   return: A boolean field {ok:true|false}, sets to true if
///           operation succeeded. A fail can come up if 'admin' authentication
///           fails.
char *cgi_del_user(char *admin, char *akey, char *user);

/// Modifies user level.
///   admin : Admin name.
///   akey  : Admin password.
///   user  : User name to change level.
///   level : New user level. Level "0" is the admin one.
///   return: A boolean field {ok:true|false}, sets to true if
///           operation succeeded. A fail can come up if 'admin' authentication
///           fails or 'user' does not exist.
char *cgi_change_level(
  char *admin, char *akey, char *user, char *level
);

/// Changes user password.
///   user   : User name to change password.
///   key    : Old password.
///   new_key: New password.
///   return : A boolean field {ok:true|false}, sets to true if operation
///            succeeded. A fail can come up if 'user' authentication fails.
char *cgi_change_pass(char *user, char *key, char *new_key);

/// cgi_del_session deletes 'session' and returns an empty response.
char *cgi_del_session(char *session_id);

/// Sends to client level, key, page_id and session_id.
/// If authentication failed every value is "".
///   user      : User name.
///   key       : User password.
///   expiration: If is set to false, session will expire after 30 days.
///   return    : 'level', 'key', 'pageId' and 'sessionId' fields.
char *cgi_authentication(char *user, char *key, int expiration);

/// Returns client 'connectionId' and 'key'. If conection failed both are "".
///   session_id: Session identifier.
///   return    : {connectionId: String, key: String}.
///               'key' is a new key, set for the new connection.
char *cgi_connect(char  *session_id);

/// Returns a normal response.
/// 'data' is a Map[Js]
char *cgi_ok(Map *data);

/// Retuns an empty response.
char *cgi_empty(void);

/// Returns an error response, setting {error:msg}.
char *cgi_error(char *msg);

/// Returns a expired response, setting {expired:1}.
char *cgi_expired(void);

/// Runs a "long run" task. This function is intended to be called until it
/// returns {"longRunEnd"='true'}.
///   fn    : Map[Js] *(*)(void *ctx, Map[Js *rq]). "Long run" task. <i>It must
///           not be defined as inner function</i>.
///   ctx   : Context. It can be NULL. Value to pass to fn.
///   rq    : Map[js]. Data for 'fn'. 'rq' must have a field called
///           "longRunFile" which value the first time it is called is "" and
///           after
///           that its value is the returned by this function.
///   return:
///     first call     : A Map[Js] with an only field "longRunFile" whitch must
///                      be used in following calls.
///     following calls: - If 'fn' was finished the Map returned with 'fn' with
///                        the field {"longRunEnd"='true'} added.
///                      - If 'fn' is running a Map with the only field
///                        {"longRunEnd"='false'}
Map *cgi_long_run(Map *(*fn)(void *ctx, Map *rq), void *ctx, Map *rq);
#endif
// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator.

#ifndef DM_IT_H
  #define DM_IT_H

#include <stddef.h>
#include "Arr.h"

typedef struct opt_Opt Opt;

///
typedef struct it_It It;

///
typedef Opt *(*it_Next)(void *);

/// it_new creates a new It.
///   o   : Container
///   next: Function which returns a element of 'o' or Opt_empty() if there
///         are no more elements.
It *it_new (void *o, Opt *(*next)(void *o));

///
It *it_empty (void);

///
It *it_unary (void *e);

/// it_range is an iterator that returns values between begin (inclusive)
/// and end (exclusive).
It *it_range (int begin, int end);

/// it_range0 is equals to it_range(0, end).
It *it_range0 (int end);

///
int it_has_next (It *this);

///
void *it_next (It *this);

/// Shows next element witout advancing.
void *it_peek (It *this);

/// it_add adds an element at the end of 'this's
It *it_add (It *this, void *element);

/// it_add adds an element at the beginning of 'this's
It *it_add0 (It *this, void *element);

///
It *it_cat (It *this, It *another);

///
It *it_take (It *this, size_t n);

///
It *it_takef (It *this, int (*predicate)(void *e));

///
It *it_drop (It *this, size_t n);

///
It *it_dropf (It *this, int (*predicate)(void *e));

///
It *it_filter (It *this, int (*predicate)(void *e));

///
It *it_map (It *this, void *(*converter)(void *e));

/// it_map2 applies conv1 to the first element and conv2 to the others.
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

/// Returns It<Tp>.
It *it_zip (It *it1, It *it2);

/// Returns It<Tp3>.
It *it_zip3 (It *it1, It *it2, It *it3);

///
It *it_reverse (It *this);

/// it_sort sorts 'this' calling 'arr_sort'.
It *it_sort (It *this, int (*greater)(void *e1, void *e2));

///
void it_each (It *this, void (*f)(void *e));

///
void it_each_ix (It *this, void (*f)(void *e, size_t ix));

///
size_t it_count (It *this);

///
int it_eq (It *it1, It *it2, int (*feq)(void *e1, void *e2));

///
int it_index (It *this, int (*predicate)(void *e));

///
int it_contains (It *this, int (*predicate)(void *e));

///
int it_last_index (It *this, int (*predicate)(void *e));

/// Returns the first element which satisfies 'predicate' or opt_empty().
Opt *it_find (It *this, int (*predicate)(void *e));

/// Returns the first element which satisfies 'predicate' or 'option'.
void *it_ofind (It *this, void *option, int (*predicate)(void *e));

/// Creates an array from 'this'.
Arr *it_to (It *this);

/// Creates an It from 'a'.
It *it_from (Arr *a);

/// Returns Tp<Arr, Arr> (dup, rest): 'dup' with duplicates values (only the
/// first case) and 'rest' with every element of 'this' without duplicates.
Tp *it_duplicates (It *this, int (feq)(void *e1, void *e2));

#endif


// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Functions using external programs.

#ifndef DMC_EXT_H
  #define DMC_EXT_H

/// Calls "wget -q -O - 'url'" and returns the text read.
/// If the reading fails, it returns an empty string.
char *ext_wget(char *url);

/// Reads a text using GUI. It calls:
///   zenity --entry --title='title' --text='prompt' 2>/dev/null
/// The return removes starting and trailing spaces.
/// If user clicks on cancel, it returns an empty string.
/// It is posible set a default text adding in promp:
///   \" --entry-text \"[text_to_add]
char *ext_zenity_entry(char *title, char *prompt);

/// ext_zenity_msg shows a message box. It calls:
///   zenity --notification --window-icon='icon' --text='text' 2>/dev/null
/// 'icon' is one of gnome icon stock. For example: info, dialog-warning,
/// dialog-error, dialog-information, face-wink, etc
void ext_zenity_msg(char *icon, char *text);

/// ext_pdf generates a pdf file from a html text. It calls:
///   pdfPrinter -s %s -t %s 'options' 2>&1
///
///   tx_source  : Text html
///   file_target: Path of the new pdf file
///   options    : Options for pdfPrinter
void ext_pdf(
  char *tx_source,
  char *file_target,
  char *options
);

/// ext_zip compress source in target. It calls:
///   zip -q 'target' 'source' 2>&1
/// if 'target' already exists, source will be added to it. If you require a
/// fresh target file, you have to delete it previously.
///   source: can be a file or directory,
///   target: Zip file. If it is a relative path, it hangs on source parent.
void ext_zip(char *source, char *target);

/// ext_unzip uncompress source in target, It calls:
///   unzip -q 'source' -d 'target' 2>&1
///
///   source: Zip file.
///   target: A directory. It it does not exist, it is created.
void ext_unzip(char *source, char *target);

#endif
// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of three values.

#ifndef DMC_TP3_H
  #define DMC_TP3_H

///
typedef struct tp3_Tp3 Tp3;

///
Tp3 *tp3_new(void *e1, void *e2, void *e3);

///
void *tp3_e1(Tp3 *this);

///
void *tp3_e2(Tp3 *this);

///
void *tp3_e3(Tp3 *this);

#endif
// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing file paths.

#ifndef DMC_PATH_H
  #define DMC_PATH_H

#include "Opt.h"

/// Returns name and extension of path.
/// If path is "" or ends at ("/") it returns "".
char *path_name (char *path);

/// Returns the parent path of 'path'.
/// If 'path' is "/" or a string without '/', it returns an empty string.
char *path_parent (char *path);

/// Returns only extension of path. Extension is returned with point,
/// (e.g., ".", ".txt").
/// If path does not have extension it returns "".
char *path_extension (char *path);

/// Returns only name of path.
/// If path is "", ends at ("/"), or if file starts with point, it returns "".
char *path_only_name (char *path);

/// Concatenates paths. Variable argumens must finish with NULL.
char *path_cat (char *s, char *more, ...);

/// Returns Opt<char> Cannonical representation of 'path'.
///   - If some component of 'path' is not in file system, returns 'opt_empty'.
///   - Directories do not finish in '/'.
Opt *path_canonical (char *s);

#endif
// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing dates.

#ifndef DMC_DATE_H
  #define DMC_DATE_H

#include <time.h>
#include "dmc/std.h"

/// Makes a new time_t.
///   this : New time_t.
///   year : Year with all digits.
///   month: Month in base 1 (1 to 12).
///   day  : Day in base 1 (1 to 31).
time_t date_new (int day, int month, int year);

/// Returns the current date.
time_t date_now (void);

/// Makes a date from a string is in format 'yyyymmdd'.
/// (month and day in base 1).
time_t date_from_str (char *date);

/// Makes a date from a string in format '[x]x/[x]x/[xxx]x'.
/// (month and day in base 1).
time_t date_from_iso (char *date);

/// Mmakes a date from a string in format '[x]x/[x]x/[xxx]x'.
/// (month and day in base 1).
time_t date_from_us (char *date);

/// Makes a date from a string is in format '[x]xSP[x]xSP[xxx]x'.
/// If 'data' is not valid, returns '0'.
time_t date_from_iso_sep (char *date, char sep);

/// Makes a date from a string in format '[x]xSP[x]xSP[xxx]x'.
/// If 'data' is not valid, returns '0'.
time_t date_from_us_sep (char *date, char sep);

///
int date_eq (time_t this, time_t another);

///
int date_cmp (time_t this, time_t another);

/// Returns the difference in days this - another.
int date_df (time_t this, time_t another);

///
time_t date_add (time_t this, int days);

///
int date_day (time_t this);

///
int date_month (time_t this);

///
int date_year (time_t this);

/// Formats a time_t. Format can be:
///   %a     The abbreviated name of the day of the week according to the
///          current locale.
///   %A     The full name of the day of the week according to the current
///          locale.
///   %b     The abbreviated month name according to the current locale.
///   %B     The full month name according to the current locale.
///   %c     The preferred date and time representation for the current
///          locale.
///   %C     The century number (year/100) as a 2-digit integer. (SU)
///   %d     The day of the month as a decimal number (range 01 to 31).
///   %D     Equivalent to %m/%d/%y.  (Yecch—for Americans only.  Americans
///          should note that in other countries %d/%m/%y is rather common.
///          This means that in international context this format is
///          ambiguous and should not be used.) (SU).
///   %e     Like %d, the day of the month as a decimal number, but a
///          leading zero is replaced by a space. (SU).
///   %F     Equivalent to %Y-%m-%d (the ISO 8601 date format). (C99).
///   %G     The ISO 8601 week-based year (see NOTES) with century as a
///          decimal number.  The 4-digit year corresponding to the ISO
///          week number (see %V).  This has the same format and value as
///          %Y, except that if the ISO week number belongs to the previous
///          or next year, that year is used instead. (TZ).
///   %g     Like %G, but without century, that is, with a 2-digit year
///          (00-99). (TZ).
///   %h     Equivalent to %b.  (SU).
///   %H     The hour as a decimal number using a 24-hour clock (range 00
///          to 23).
///   %I     The hour as a decimal number using a 12-hour clock (range 01
///          to 12).
///   %j     The day of the year as a decimal number (range 001 to 366).
///   %k     The hour (24-hour clock) as a decimal number (range 0 to 23);
///          single digits are preceded by a blank.  (See also %H.)  (TZ)
///   %l     The hour (12-hour clock) as a decimal number (range 1 to 12);
///          single digits are preceded by a blank.  (See also %I.)  (TZ)
///   %m     The month as a decimal number (range 01 to 12).
///   %M     The minute as a decimal number (range 00 to 59).
///   %n     A newline character. (SU)
///   %O     Modifier: use alternative format, see below. (SU)
///   %p     Either "AM" or "PM" according to the given time value, or the
///          corresponding strings for the current locale.  Noon is treated
///          as "PM" and midnight as "AM".
///   %P     Like %p but in lowercase: "am" or "pm" or a corresponding
///          string for the current locale. (GNU)
///   %r     The time in a.m. or p.m. notation.  In the POSIX locale this
///          is equivalent to %I:%M:%S %p.  (SU)
///   %R     The time in 24-hour notation (%H:%M).  (SU) For a version
///          including the seconds, see %T below.
///   %s     The number of seconds since the Epoch, 1970-01-01 00:00:00
///          +0000 (UTC). (TZ)
///   %S     The second as a decimal number (range 00 to 60).  (The range
///          is up to 60 to allow for occasional leap seconds).
///   %t     A tab character. (SU).
///   %T     The time in 24-hour notation (%H:%M:%S). (SU).
///   %u     The day of the week as a decimal, range 1 to 7, Monday being
///          1.  See also %w.  (SU)
///   %U     The week number of the current year as a decimal number, range
///          00 to 53, starting with the first Sunday as the first day of
///          week 01.  See also %V and %W.
///   %V     The ISO 8601 week number (see NOTES) of the current year as a
///          decimal number, range 01 to 53, where week 1 is the first week
///          that has at least 4 days in the new year.  See also %U and %W.
///          (SU).
///   %w     The day of the week as a decimal, range 0 to 6, Sunday being
///          0.  See also %u.
///   %W     The week number of the current year as a decimal number, range
///          00 to 53, starting with the first Monday as the first day of
///          week 01.
///   %x     The preferred date representation for the current locale
///          without the time.
///   %X     The preferred time representation for the current locale
///          without the date.
///   %y     The year as a decimal number without a century (range 00 to
///          99).
///   %Y     The year as a decimal number including the century.
///   %z     The +hhmm or -hhmm numeric timezone (that is, the hour and
///          minute offset from UTC). (SU).
///   %Z     The timezone name or abbreviation.
///   %%     A literal '%' character.
char *date_f (time_t this, char *template);

/// Returns a string in format 'yyyymmdd'.
char *date_to_str (time_t this);

/// Returns a string in format 'dd/mm/yyyy'.
char *date_to_iso (time_t this);

/// Returns a string in format 'mm/dd/yyyy'.
char *date_to_us (time_t this);

///
Js *date_to_js (time_t this);

///
time_t date_from_js (Js *js);

///
typedef struct timeval DateTm;

/// Returns the current time with microsconds of precission.
///   DateTm (struct timeval) has following fields:
///     time_t tv_sec - Seconds.
///     long int tv_usecs - Microseconds (0 - 999999).
DateTm *dateTm_now ();

/// Returns t1 - t2.
DateTm *dateTm_tdf (DateTm *t1, DateTm *t2);

/// Adds 'millis' milliseconds to 't'. 'millis' can be negative.
DateTm *dateTm_add (DateTm *t, int millis);

/// Returns t1 - t2 in milliseconds.
int dateTm_df (DateTm *t1, DateTm *t2);

#endif
// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

///String utilities.

#ifndef DMC_STR_H
  #define DMC_STR_H

#include <stdarg.h>
#include <string.h>
#include "Opt.h"
#include "Arr.h"

/// Returns a copy of 'str'.
char *str_new(char *s);

/// Returns a string with the character 'ch'.
char *str_c(char ch);

/// Returns 'strcoll(s1, s2)'.
int str_cmp_locale(char *s1, char *s2);

/// Returns 'strcmp(s1, s2) > 0'.
int str_greater(char *s1, char *s2);

/// Returns 'strcoll(s1, s2) > 0'.
int str_greater_locale(char *s1, char *s2);

/// Returns 'true' if 'str1 == str2'.
int str_eq(char *str1, char *str2);

/// Indicates if 'str' starts with 'substr'.
int str_starts(char *str, char *substr);

/// Indicates if 'str' ends with 'substr'.
int str_ends(char *str, char *substr);

/// Returns first position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_cindex(char *str, char ch);

/// Returns first position of 'ch' in 'str' from start or -1 if 'ch' is missing.
int str_cindex_from(char *str, char ch, int start);

/// Returns first position of 'substr' in 'str' or -1 if 'substr' is missing.
int str_index(char *str, char *substr);

/// Returns first position of 'substr' in 'str' from start or -1 if 'substr' is
/// missing.
int str_index_from(char *str, char *substr, int start);

/// Returns last position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_last_cindex(char *str, char ch);

/// Returns last position of 'substr' in 'str' or -1 if 'substr' is missing.
int str_last_index(char *str, char *substr);

/// str_cat is a string concatenation.
/// Variable argumens must finish with NULL.
char *str_cat(char *s, ...);

/// Returns a substring.
///   - If 'begin' or 'end' are negatives, they are subtracted from 'strlen(s)'.
///   - If 'begin' or 'end' are out of bounds, function throws a RANGE exception.
char *str_sub(char *str, int begin, int end);

/// str_left is equals to 'str_sub(str, 0, end)'.
char *str_left(char *str, int end);

/// str_right is equals to 'str_sub(str, begin, strlen(s))'.
char *str_right(char *str, int begin);

/// Returns a new string removing spaces (ch <= ' ') at left.
char *str_ltrim(char *str);

/// Returns a new string removing spaces (ch <= ' ') at right.
char *str_rtrim(char *str);

/// Returns a new string removing spaces (ch <= ' ') at left and right.
char *str_trim(char *str);

/// Splits 'str' in an Arr[char].
/// For example (using ';' as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// Returns an Arr<char>.
Arr *str_csplit(char *str, char sep);

/// str_csplit_trim is similar to <tt>str_csplit</tt> but trimming elements.
/// Returns an Arr<char>.
Arr *str_csplit_trim(char *str, char sep);

/// Splits 'str' in an Arr<char>.
/// For example (using ";" as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// If 'sep' is "" return will have only one string equals to 'str'.
/// Returns an Arr<char>.
Arr *str_split(char *str, char *sep);

/// str_split_trim is similar to <tt>str_split</tt> but trimming elements.
/// Returns an Arr[char]
Arr *str_split_trim(char *str, char *sep);

/// Joins elements of 'a', separated by 'sep'.
/// 'a' is Arr<char>.
char *str_cjoin(Arr *a, char sep);

/// Joins elements of 'a', separated by 'sep'.
/// 'a' is Arr<char>.
char *str_join(Arr *a, char *sep);

/// Returns a new string replacing 'old' by 'new' in 's'.
char *str_creplace(char *s, char old, char new);

/// Returns a new string replacing 'old' by 'new' in 's'.
/// If 'old' is "", it does nothing.
char *str_replace(char *s, char *old, char *new);

/// Returns a string with format similar to 'vprintf'.
char *str_vf(char *format, va_list args);

/// Returns a string with format similar to 'printf'.
char *str_f(char *format, ...);

/// Returns utf8 caracter number or -1 if 's' is not a valid utf8 string.
int str_runes(char *s);

/// Reads next rune of 's'.
/// If there are no more runes or it fails, 'rune' is equals to "".
/// Example:
///   char *tx = "a text";
///   char *rune;
///   char *rest = str_next_rune(&rune, tx);
///   while (*rune) {
///     puts(rune);
///     rest = str_next_rune(&rune, rest);
///   }
char *str_next_rune(char **rune, char *s);

/// Codifies a string in Unicode. Returns an 'Opt_empty()' if there are errors.
///   return: Opt<unsigned>
Opt *str_to_unicode(char *s);

/// Decodifies a string from Unicode. Returns an 'Opt_empty()' if there are
/// errors.
///   return: Opt<char>
Opt *str_from_unicode(unsigned *u);

/// Decodifies a string from ISO-8859-1.
char *str_from_iso(char *s);

/// Returns 's' converted to uppercase. It is necessary set language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_upper_new("cañón");
///   puts(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_upper (char *s);

/// Returns 's' converted to lowercase. It is necessary set language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_lower_new("cañón");
///   puts(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_lower (char *s);

/// Replaces " by \" and \ by \\ and insert the result inside quotes.
char *str_to_escape (char *s);

/// Restores the string escaped with 'escape'. If 's' does not come from
/// 'escape' the result is indefined.
char *str_from_escape (char *s);

#endif
// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map structure.

#ifndef DMC_MAP_H
  #define DMC_MAP_H

#include "dmc/Arr.h"
#include "dmc/Opt.h"

///
typedef struct map_Map Map;

/// Initializates a map. Map can be cast to Arr<Kv>.
Map *map_new(void);

///
int map_size(Map *this);

/// Puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this : The map.
///   key  : Entry key.
///   value: New value
void map_put(Map *this, char *key, void *value);

/// Returns 1 if 'this' has key 'key'.
int map_has_key(Map *this, char *key);

/// Returns the value pointed by key or 'opt_empty' if 'key' does not exist.
Opt *map_get(Map *this, char *key);

/// Removes value with key 'key' or does nothing if 'key' does not exist.
void map_remove(Map *this, char *key);

/// Returns keys of this in a Arr<char>.
Arr *map_keys(Map *this);

/// Equals to (Arr *)this
/// Returns an Arr[Kv]
Arr *map_kvs(Map *this);

/// Sorts 'this' from keys.
void map_sort(Map *this);

/// Sorts 'this' in locale from keys.
void map_sort_locale(Map *this);

/// Creates an iterator over 'this'.
///   return: It<Kv>
It *map_to_it(Map *this);

/// Creates a Map from 'it'
///   it: It[Kv]
Map *map_from_it(It *it);

/// Returns a Js from a value of 'this'.
///   to: Value converter.
Js *map_to_js(Map *this, Js *(*to)(void *e));

/// Parses a Js to a value of 'this'.
///   from: Value converter.
Map *map_from_js(Js *js, void *(*from)(Js *jse));

#endif
// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions.

#ifndef DMC_DEFS_H
  #define DMC_DEFS_H

typedef struct js_Js Js;

typedef struct schd_Task SchdTask;

///
#define MALLOC(type) (type *)GC_MALLOC(sizeof(type))

///
#define ATOMIC(size) GC_MALLOC_ATOMIC(size)

///
#define REPEAT(n) { \
  int __i = (n) + 1; \
  while (--__i) {

///
#define _REPEAT }}

///
#define RANGE0(i, end) { \
  int __end = end; \
  int i = -1; \
  while (++i < __end) {

///
#define RANGE(i, begin, end) { \
  int __end = end; \
  int i = (begin) - 1; \
  while (++i < __end) {

///
#define _RANGE }}

/// Iterates over an Iarr. See EACH.
#define IEACH(a, n) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  while(__p < __pend) { \
    n = *__p++;

/// Iterates over an Iarr. See EACH_IX.
#define IEACH_IX(a, n, ix) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  int ix = -1; \
  while(__p < __pend) { \
    ++ix; \
    n = *__p++;

/// Iterates over an Darr. See EACH.
#define DEACH(a, n) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  while(__p < __pend) { \
    n = *__p++;

/// Iterates over an Darr. See EACH_IX
#define DEACH_IX(a, n, ix) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  int ix = -1; \
  while(__p < __pend) { \
    ++ix; \
    n = *__p++;

/// Iterates over an 'Arr'. You can access to the 'element' index with _i.
/// For example:
///   EACH(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
///
///   a      : An Arr *.
///   type   : Element type without pointer sign (*).
///   element: An element of type 'type'.
#define EACH(a, type, element) { \
  Arr *__arr = (Arr *)a; \
  size_t __size = arr_size(__arr); \
  size_t _i; \
  type *element; \
  for (_i = 0; _i < __size; ++_i) { \
    element = arr_get(__arr, _i);

/// Iterates over an 'Arr'. You can access to the 'element' index with i.
/// For example:
///   EACH(a, char, s, i) {
///     printf("[%d] -> %s\n", i, s);
///   } _EACH
///
///   a      : An Arr *.
///   type   : Element type without pointer sign (*).
///   e: An element of type 'type'.
///   i: Element index.
#define EACH_IX(a, type, e, i) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_start(__a); \
  void **__pend = arr_end(__a); \
  type *e; \
  int i = -1; \
  while(__p < __pend) { \
    ++i; \
    e = *__p++;

/// Iterates over an 'It'. You can access to the 'element' index with _i.
/// For example:
///   EACH(it, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
///
///   a      : An It *.
///   type   : Element type without pointer sign (*).
///   element: An element of type 'type'.
#define EACHI(it, type, element) { \
  It *__it = (It *)it; \
  int _i = -1; \
  type *element; \
  while (it_has_next(__it)) { \
    ++_i; \
    element = it_next(__it);

/// Iterates over a 'List'.
/// For example:
///   EACHL(list, char, s) {
///     printf("%s\n", s);
///   } _EACH
///
///   list   : An List *,
///   type   : Element type without pointer sign (*),
///   element: An element of type 'type',
#define EACHL(list, type, element) { \
  List *_EACHL_list = (List *)list; \
  type *element; \
  while (!list_empty(_EACHL_list)) { \
    element = list_head(_EACHL_list); \
    _EACHL_list = list_tail(_EACHL_list);

/// Iterates over an 'Arr' in reverse order. You can access to the 'element'
/// index with _i.
/// For example:
///   EACHR(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
///
///   a      : An Arr *.
///   type   : Element type without pointer sign (*).
///   element: An element of type 'a'.
#define EACHR(a, type, element) { \
  Arr *__arr = (Arr *)a; \
  size_t _i = arr_size(__arr); \
  type *element; \
  while (_i) { \
    element = arr_get(__arr, --_i);

/// Finalizes an EACHL, EACH or a EACHR.
#define _EACH }}

///
typedef void *(*FCOPY)(void *);

///
typedef int (*FCMP)(void *, void *);

///
typedef void (*FPROC)(void *);

///
typedef int (*FPRED)(void *);

///
typedef Js *(*FTO)(void *);

///
typedef void *(*FFROM)(Js *);

///
typedef void (*FLOOP)(void *, SchdTask *);

/// Macros to manage exceptions. Example:
///   TRY
///     ...
///   CATCH (e)
///     puts(exc_msg(e));
///   _TRY
/// NOTE: CATCH block must return 'void'
#define TRY { \
  jmp_buf *_TRY_buf = MALLOC(jmp_buf); \
  exc_add(_TRY_buf); \
  if (!setjmp(*_TRY_buf)) { \

/// See TRY.
#define CATCH(e) ;exc_remove();} else { Exc *e = exc_get();

/// See>TRY.
#define _TRY ;exc_remove();}}

/// Example:
///   THROW(exc_io_t) "Working directory not found: %s", strerror(errno) _THROW
#define THROW(type) exc_throw(type, str_f(

///
#define _THROW ), __FILE__, (char *)__func__, __LINE__);

/// Example:
///   EXC_GENERIC("Fail")
#define EXC_GENERIC(msg) \
  THROW(exc_generic_t) msg _THROW

/// Throw a range exception if v < 'min' or v > 'max'.
/// Example:
///   EXC_RANGE(v, 0, 23) // -1 and 24 throw exeption.
#define EXC_RANGE(value, min, max) { \
    int __v = value; \
    if (__v < (min) || __v > (max)) \
      THROW(exc_range_t) exc_range((min), (max), __v) _THROW \
  }

/// Example:
///   EXC_ILLEGAL_ARGUMENT("Fail", "a value", "another value")
#define EXC_ILLEGAL_ARGUMENT(msg, expected, actual) \
  THROW(exc_illegal_argument_t) \
    exc_illegal_argument(msg, expected, actual) \
  _THROW

/// Example:
///   EXC_ILLEGAL_STATE("Fail")
#define EXC_ILLEGAL_STATE(msg) \
  THROW(exc_illegal_state_t) exc_illegal_state(msg) _THROW

/// Example:
///   EXC_IO("Fail")
#define EXC_IO(msg) \
  THROW(exc_io_t) exc_io(msg) _THROW

/// Reads a 'field' of 'map'. If 'field' is not found produce an
/// ILLEGAL_ARGUMENT exception, otherwise returns its value in 'type var'
/// using 'fun'.
/// Examples:
///   CGI_GET(int, index, js_ri, m)
///   CGI_GET(char *, value, js_rs, m)
///   CGI_GET(Arr *, values, js_ra, m)
///
///   type: type of var.
///   var : name of variable.
///   fun : function to pass 'Js' to 'type'.
///   map : A Map<Js>.
#define CGI_GET(type, var, fun, map) \
  type var; \
  { \
    Opt *js = map_get(map, #var); \
    if (opt_is_empty(js))  \
      EXC_ILLEGAL_ARGUMENT(#var, "Map key", "Key not found") \
    var = fun(opt_get(js)); \
  }

/// Calls CGI_GET with 'var' as 'int'.
#define CGI_GET_BOOL(var, map) \
  CGI_GET(int, var, js_rb, map)

/// Calls CGI_GET with 'var' as 'int'.
#define CGI_GET_INT(var, map) \
  CGI_GET(int, var, js_ri, map)

/// Calls CGI_GET with 'var' as 'double'.
#define CGI_GET_DOUBLE(var, map) \
  CGI_GET(double, var, js_rd, map)

/// Calls CGI_GET with 'var' as 'char *'.
#define CGI_GET_STR(var, map) \
  CGI_GET(char *, var, js_rs, map)

/// Calls CGI_GET with 'var' as 'Arr<Js>'.
#define CGI_GET_ARR(var, map) \
  CGI_GET(Arr *, var, js_ra, map)

/// Calls CGI_GET with 'var' as 'Map<Js>'.
#define CGI_GET_MAP(var, map) \
  CGI_GET(Map *, var, js_ro, map)

#endif
// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing files.

#ifndef DMC_FILE_H
  #define DMC_FILE_H

#include <time.h>
#include "dmc/Arr.h"
#include "dmc/Bytes.h"

///
typedef struct file_FileLck FileLck;

/// Returns a new file path whose template is:
/// /tmp/'prefix'xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp (char *prefix);

/// Returns a new file path whose template is:
/// 'dir'/'prefix'xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp_in (char *dir, char *prefix);

/// Returns the working directory
char *file_cwd (void);

/// Sets the working directory
void file_cd (char *path);

/// Makes a directory with 0755 permissions.
/// If parent directory does not exist it creates it.
/// If 'path' already exists it does nothing.
void file_mkdir (char *path);

/// fReturns an Arr[char] with names of files and directories existing in
/// 'path'.
/// Values '.' and '..' are not in the return.
Arr *file_dir (char *path);

/// Deletes file or directory named 'path' although it is a not empty directory.
/// If 'path' does not exists it does nothing.
void file_del (char *path);

/// Renames 'old_path' to 'new_path'.
void file_rename (char *old_path, char *new_path);

/// Makes a symbol link from 'new_path' to 'old_path'.
void file_link (char *old_path, char *new_path);

/// file_exits returns true if 'path' exists in the file system.
int file_exists (char *path);

/// Returns true if file is a directory
int file_is_directory (char *path);

/// Returns information of 'path'. If path does no exist or another error
/// happens, this function produces a exception.
/// Some fields to check in 'struct stat' are:
///   mode_t st_mode  - It can be tested with S_ISDIR(), S_ISREG or S_ISLNK
///                     among others. This macros are defined in sys/stat.h
///   uid_t st_uid    - User id
///   gid_t st_gid    - Group id
///   off_t st_size   - File size
///   time_t st_atime - Last access to file in seconds.
///   time_t st_mtime - Last file modification in seconds.
struct stat *file_info (char *path);

/// Returns the size of 'path'.
int file_size(char *path);

/// Returns the last access in seconds.
time_t file_modified(char *path);

/// reads data from 'path', including ends of line.
/// This function opens, reads and closes file.
char *file_read (char *path);

/// Writes 'data' on 'path'.
/// This function opens, writes and closes file.
void file_write (char *path, char *text);

/// Appends 'data' on 'path'.
/// This function opens, writes and closes file.
void file_append (char *path, char *text);

/// Binary copy source to target.
void file_copy (char *source_path, char *target_path);

/// Opens a file to read with file_read_line or file_read_bin.
/// It returns a FileLck object which will be freed when close is called.
FileLck *file_ropen (char *path);

/// Opens a file to write with file_write_line or file_write_bin.
/// It returns a FileLck object which will be freed when close is called.
FileLck *file_wopen (char *path);

/// Opens a file to append with file_write_line or file_write_bin.
/// It returns a FileLck object which will be freed when close is called..
FileLck *file_aopen (char *path);

/// Reads a text file opened with file_ropen.
/// It does not delete ends of line.
/// When reading is finished, returns a blank string.
char *file_read_line (FileLck *lck);

/// Writes a text file opened with file_wopen or file_aopen.
void file_write_text (FileLck *lck, char *text);

/// Reads a binary file opened with file_ropen.
/// When reading is finished, returns an empty Bytes.
Bytes *file_read_bin_buf (FileLck *lck, int buffer);

/// file_read_bin is the same as 'file_read_bin_bf' using a buffer of 8192.
Bytes *file_read_bin (FileLck *lck);

/// Writes a binary file opened with file_wopen.
/// Returns 0 if there is no error.
void file_write_bin (FileLck *lck, Bytes *bs);

/// Closes a file open with file_ropen, file_wopen or file_aopen.
void file_close (FileLck *lck);

#endif
// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strings constructor.

#ifndef DMC_BUF_H
  #define DMC_BUF_H

///
typedef struct buf_Buf Buf;

/// Initializes a 'buf' with size 150.
Buf *buf_new(void);

/// Initializes a 'buf'.
Buf *buf_bf_new(int buffer_size);

/// Returns the length of the enveloped string.
int buf_len(Buf *this);

/// Returns a reference to the string wrapped. Return is intended to
/// be not modified.
char *buf_str(Buf *this);

/// Adds 'length bytes of 'data' to 'buf'.
/// 'length' must be less or equals to 'strlen(data)'.
/// It is not necessary that 'data' be a null-terminated string, but it must
/// no have characters \0
void buf_add_buf(Buf *this, char *data, int length);

/// Adds 'data' to 'buf'.
void buf_add(Buf *this, char *data);

/// Adds a character.
void buf_cadd(Buf *this, char data);

/// Returns a copy of the enveloped string.
char *buf_to_str(Buf *this);

/// Resets buffer (but does not reduce its size).
void buf_reset(Buf *this);

#endif
// Copyright 22-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Option.

#ifndef DMC_OPT_H
  #define DMC_OPT_H

typedef struct opt_Opt Opt;

/// Creates a full option.
Opt *opt_new (void *value);

/// Creates an empty option.
Opt *opt_empty (void);

/// Returns 1 if option has no value.
int opt_is_empty (Opt *this);

/// Returns 1 if option has value.
int opt_is_full (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty.
void *opt_get (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty with 'msg' as message.
void *opt_eget (Opt *this, char *msg);

/// Returns value if 'this' is empty.
void *opt_oget (Opt *this, void *value);

/// Returns NULL if 'this' is empty.
void *opt_nget (Opt *this);

#endif
// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing json strings.

#ifndef DMC_JS_H
  #define DMC_JS_H

#include "dmc/Arr.h"
#include "dmc/Map.h"

/// 'Js' is an alias of 'char'.
typedef struct js_Js Js;

/// Returns '1' if json is "null" or '0' in another case.
int js_is_null (Js *json);

/// Read boolean.
int js_rb (Js *json);

/// Read int.
int js_ri (Js *json);

/// Read long.
long js_rl (Js *json);

/// Read double.
double js_rd (Js *json);

/// Read string.
char *js_rs (Js *json);

/// Read array in an Arr<Js>.
Arr *js_ra (Js *json);

/// Read object in a Map<js>.
Map *js_ro (Js *json);

/// Write a null value.
Js *js_wn(void);

/// Write a boolean value.
Js *js_wb(int value);

/// Write an int value.
Js *js_wi(int n);

/// Write an long value.
Js *js_wl(long n);

/// Write a double value with a maximum of 9 decimal positions.
Js *js_wd(double n);

/// Write a string.
Js *js_ws(char *s);

/// Write an Arr<Js>.
Js *js_wa(Arr *a);

/// Write a Map<Js>.
Js *js_wo(Map *m);

#endif
// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of ints.

#ifndef DMC_IARR_H
  #define DMC_IARR_H

#include "Js.h"

///
typedef struct iarr_Iarr Iarr;

///
Iarr *iarr_new(void);

/// buffer must be > 0
Iarr *iarr_bf_new(int buffer);

/// Creates a new array from a C array. For example:
///   Iarr *a = iarr_new_c(3, (int[]){2, 0, 4});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
Iarr *iarr_new_c (int size, int *es);

/// Returns a new Iarr with elements from 0 to 'ix' (exclusive),
Iarr *iarr_left(Iarr *this, int ix);

/// Returns a new Iarr with elements from 'ix' (inclusive) to end of 'this'.
Iarr *iarr_right(Iarr *this, int ix);

/// Returns a new Iarr with elements from 'begin' (inclusive) to
/// to 'end' (exclusive),
Iarr *iarr_sub(Iarr *this, int begin, int end);

///
Iarr *iarr_copy(Iarr *this);

///
int iarr_size(Iarr *this);

///
int iarr_eq(Iarr *this, Iarr *other);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
int iarr_get(Iarr *this, int ix);

///
int *iarr_start(Iarr *this);

///
int *iarr_end(Iarr *this);

///
void iarr_push(Iarr *this, int e);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_set(Iarr *this, int ix, int e);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_insert(Iarr *this, int ix, int e);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_remove(Iarr *this, int ix);

///
void iarr_cat(Iarr *this, Iarr *other);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_insert_arr(Iarr *this, int ix, Iarr *other);

/// If begin or end are < 0 then is changed to 'iarr_size - itsValue'.
void iarr_remove_range(Iarr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void iarr_clear (Iarr *this);

/// Removes every element of 'this'.
void iarr_bf_clear (Iarr *this, int buffer);

///
void iarr_reverse(Iarr *this);

///
void iarr_sort(Iarr *this);

///
Js *iarr_to_js(Iarr *this);

///
Iarr *iarr_from_js(Js *js);

#endif
// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Standard files of dmc.

#ifndef DMC_STD_H
  #define DMC_STD_H

#include <stdio.h>
#include <stdlib.h>
#include <gc.h>

#include "DEFS.h"
#include "Tp.h"
#include "Kv.h"
#include "Tp3.h"
#include "Exc.h"
#include "Opt.h"
#include "Bytes.h"
#include "Arr.h"
#include "It.h"
#include "Map.h"
#include "Buf.h"
#include "str.h"
#include "sys.h"
#include "path.h"
#include "file.h"
#include "Js.h"

#endif
// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List (immutable) structure.

#ifndef DM_LIST_H
  #define DM_LIST_H

///
typedef struct list_List List;
typedef struct opt_Opt Opt;
typedef struct it_It It;
typedef struct arr_Arr Arr;
typedef struct js_Js Js;

/// Creates a new List.
List *list_new(void);

/// Returns the number of elements. (O(n) operation).
int list_count(List *this);

/// Returns every element of 'this' less the first one. If "this" is
/// empty, throws an exception.
List *list_tail(List *this);

/// Returns the first element of 'this'. If "this" is empty.
/// throws an exception.
void *list_head (List *this);

/// Returns the element in 'ix' position -- head is in position 0.
/// (O(n) operation).
Opt *list_get (List *this, int ix);

/// Returns '1' if 'this' is empty.
int list_empty(List *this);

/// Adds 'o' at head. 'o' must be not NULL.
List *list_cons(List *this, void *o);

/// Returns 'this + l'.
List *list_cat(List *this, List *l);

/// list_reverse returns this in reverse order.
List *list_reverse(List *this);

/// list_to_it returns an iterator from top to bottom.
It *list_to_it (List *this);

/// list_from_it return a List with elements of 'it' in reverse order.
List *list_from_it (It *it);

/// list_to_arr returns an Arr with 'this' elements.
Arr *list_to_arr (List *this);

/// list_from_arr returns a list with 'a' elements.
List *list_from_arr (Arr *a);

/// list_to_json returns a serialization of 'this' using 'to' to.
/// convert elements.
Js *list_to_js(List *this, Js *(*to)(void *));

/// list_from_json restores a serialized List using 'from' to convert elements.
List *list_from_js(Js *js, void *(*from)(Js *));

#endif
// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing globals values.
/// A way to initializes system is:
///   sys_init("app_name");
///   sys_locale("es_ES.utf8");

#ifndef DMC_SYS_H
  #define DMC_SYS_H

#include "dmc/Opt.h"

/// Initializates a normal program and call 'rnd_init()'.
/// After call this function 'sys_home' and 'sys_user' are available.
/// It creates the user directory in "~/.dmCApp/" + 'path'.
void sys_init (char *path);

///
char *sys_home (void);

///
char *sys_uname (void);

///
char *sys_udir (void);

/// Sets LC_ALL, for example:
///   sys_set_locale("es_ES.utf8")
void sys_set_locale (char *language);

/// Returns the current locale.
char *sys_locale (void);

/// Executes 'command', redirecting stderr to stdout, and returns its standard
/// output. If command fails, function returns an empty Opt.
///   return: Opt<char>.
Opt *sys_cmd(char *command);

/// Stops the current thread.
void sys_sleep (int millis);

#endif
// Copyright 01-May-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Inet server.

#ifndef DMC_ISERVER_H
  #define DMC_ISERVER_H

#include "dmc/std.h"

/// Request read by Iserver.
typedef struct iserver_Rq IserverRq;

/// Returns a error message or an empty string if everithing was well.
char *iserverRq_error (IserverRq *this);

/// Returns Opt<char>. If no request was received, returns 'opt_emtpy'.
/// If 'iserverRq_error' returns an error, throws an ILLEGAL_STATE exception.
Opt *iserverRq_msg (IserverRq *this);

/// Retuns the direction (IPv4) of connected server. Throws an ILLEGAL_STATE
/// exception if no request was read.
/// For being sure about receiving request, 'iserverRq_msg' should be tested.
char *iserverRq_host (IserverRq *this);

/// Writes response in 'this' and close it.
/// Returns an error message or an empty string if everithing was well.
char *iserverRq_write (IserverRq *this, char *response);

///
typedef struct iserver_Iserver Iserver;

///
Iserver *iserver_new (int port);

/// Read text in a not blocking way.
IserverRq *iserver_read (Iserver *this);

///
void iserver_close (Iserver *this);

#endif
// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Decimal number and numeric utilities.

#ifndef DMC_DEC_H
  #define DMC_DEC_H

#include "Js.h"

///
typedef struct dec_Dec Dec;

/// Makes a new Dec. It requires link with -lm.
///   n    : Number which will be rounded to 'scale'.
///   scale: Decimal positions. Maximum scale is 10.
Dec *dec_new(double n, int scale);

///
char *dec_to_str(Dec *this);

/// Returns the rounded double value of 'this'.
double dec_n(Dec *this);

/// Returns the scale of 'this'.
int dec_scale(Dec *this);

/// Return has an error gap proportional to digits of 'd1' and 'd2'.
int dec_eq(double d1, double d2);

/// Returns true if d1 == d2 with an error margin of +- gap.
int dec_eq_gap(double d1, double d2, double gap);

/// Returns true if d1 == d2 with an error margin of +- gap.
int dec_eqf_gap(float d1, float d2, float gap);

/// Returns 'true' if all characters of 's' are digits.
/// ("" returns 'true').
int dec_digits(const char *s);

/// Returns a number without thousand separators and
/// with decimal point.
char *dec_regularize_iso(char *s);

/// Returns a number without thousand separators and with
/// decimal point.
char *dec_regularize_us(char *s);

/// Returns 'true' if "s" is a regularized number. If is "" returns 'true'.
/// "xxx.", "." or ".xxx" also return 'true'.
int dec_number(char *s);

///
Js *dec_to_js(Dec *this);

///
Dec *dec_from_js(Js *js);

#endif
// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Pair of two values.

#ifndef DMC_TP_H
  #define DMC_TP_H

///
typedef struct tp_Tp Tp;

///
Tp *tp_new(void *e1, void *e2);

///
void *tp_e1(Tp *this);

///
void *tp_e2(Tp *this);

#endif
// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// B64 encoder - decoder

#ifndef DMC_B64_H
  #define DMC_B64_H

#include "dmc/Bytes.h"

///
char *b64_decode(char *b64);

///
Bytes *b64_decode_bytes(char *b64);

///
char *b64_encode(char *s);

///
char *b64_encode_bytes(Bytes *bs);

#endif
