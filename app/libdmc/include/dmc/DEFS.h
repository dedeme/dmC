// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions.

#ifndef DM_DEFS_H
  #define DM_DEFS_H

///
#define MALLOC(type) (type *)GC_MALLOC(sizeof(type))

///
#define ATOMIC(size) GC_MALLOC_ATOMIC(size)

/// Scheme:
///   void name(void *o)
#define FNX(name, type, o) void name(void *__o) { \
  type *o = (type *)__o;

/// Scheme:
///   bool name(void *o)
#define FNP(name, type, o) bool name(void *__o) { \
  type *o = (type *)__o;

/// Scheme:
///   void *name(void *o)
#define FNM(name, type, o) void *name(void *__o) { \
  type *o = (type *)__o;

/// Scheme:
///   bool name(void *o1, void *o2)
#define FNE(name, type, o1, o2) \
  bool name(void *__o1, void *__o2) { \
    type *o1 = (type *)__o1; \
    type *o2 = (type *)__o2;

///
#define _FN }

/// Uses 'i' as iterator from 'begin' to 'end'
///   i    : [int] Iterator variable
///   begin: [int] Intial value (inclusive)
///   end  : [int] Last value (exclusive)</pre>
#define RANGE(i, begin, end) { \
  int __begin = (begin); \
  int __end = (end); \
  int i; \
  for (i = __begin; i < __end; ++i)

/// Uses 'i' as iterator from 0 to 'end'
///   i    : [int] Iterator variable
///   end  : [int] Last value (exclusive)</pre>
#define RANGE0(i, end) { \
  int __end = (end); \
  int i; \
  for (i = 0; i < __end; ++i)

///
#define _RANGE }

/// Iterates over a 'List'.
///   list   : An List *
///   type   : Element type without pointer sign (*)
///   element: An element of type 'type'
/// For example:
///   EACHL(list, char, s) {
///     printf("%s\n", s);
///   } _EACH
#define EACHL(list, type, element) { \
  List *_EACHL_list = (List *)list; \
  type *element; \
  while (!list_empty(_EACHL_list)) { \
    element = list_head(_EACHL_list); \
    _EACHL_list = list_tail(_EACHL_list);

/// Iterates over a 'Arr'. You can access to the 'element' index with _i.
///   a      : An Arr *
///   type   : Element type without pointer sign (*)
///   element: An element of type 'type'
/// For example:
///   EACH(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACH(a, type, element) { \
  Arr *__arr = (Arr *)a; \
  size_t __size = arr_size(__arr); \
  size_t _i; \
  type *element; \
  for (_i = 0; _i < __size; ++_i) { \
    element = arr_es(__arr)[_i];

/// Iterates over an 'Arr' in reverse order. You can access to the 'element'
/// index with _i.
///   a      : An Arr *
///   fn
///   type   : Element type without pointer sign (*)
///   element: An element of type 'a'
/// For example:
///   EACHR(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACHR(a, type, element) { \
  Arr *__arr = (Arr *)a; \
  size_t _i = arr_size(__arr); \
  type *element; \
  while (_i) { \
    element = arr_es(__arr)[--_i];

/// Finalizes an EACHL, EACH or a EACHR
#define _EACH }}

/// Repeat its body 'n' times. 'n' must be greater than 0.
#define REPEAT(n) { \
  int __i = (n) + 1; \
  while (--__i) {

/// Finalizes an REPEAT
#define _REPEAT }}

/// Macros to manage exceptions. Example:
///   TRY {
///     ...
///   } CATCH (e) {
///     puts(e);
///   }_TRY
/// NOTE: <i>CATCH block must return 'void'</i>
#define TRY {void _TRY_try() {

/// See <a href="#hp:TRY">TRY</a>
#define CATCH(e) ;exc_remove();} void _TRY_catch(char *e) {exc_remove();

/// See <a href="#hp:TRY">TRY</a>
#define _TRY } jmp_buf *_TRY_buf = MALLOC(jmp_buf); \
  exc_add(_TRY_buf); \
  int _TRY_val = setjmp(*_TRY_buf); \
  if(_TRY_val != 0) {_TRY_catch(exc_msg());} else {_TRY_try();}}

/// Example
///   THROW "Working directory can no be find: %s", strerror(errno) _THROW
#define THROW(type) exc_throw(type, str_printf(

///
#define _THROW ), __FILE__, (char *)__func__, __LINE__);

/// XNULL(var) throws a null_pointer exception when variable 'var' is null.
#define XNULL(var) if (!var) \
  exc_throw(exc_null_pointer_t, exc_null_pointer(#var) \
  , __FILE__, (char *)__func__, __LINE__);

/// ONULL(var) throws a null_pointer exception when opt_is_null(var).
#define ONULL(var) if (opt_is_null((Opt *)var)) \
  exc_throw(exc_illegal_state_t, exc_illegal_state("Opt " #var " is null") \
  , __FILE__, (char *)__func__, __LINE__);

#endif

