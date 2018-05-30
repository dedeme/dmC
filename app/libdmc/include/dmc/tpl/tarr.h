// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header array template.<p>
/// Use:
///   ///
///   typedef struct achar_Achar Achar;
///   #define TY char                    // Element type
///   #define CT Achar                   // Container type
///   #define IT Ichar                   // Iterator type
///   #define FUN(id) achar ## _ ## id   // Defintion for functions
///   #include "dmc/tpl/tarr.h"
///   #undef TY
///   #undef CT
///   #undef IT
///   #undef FUN

#include "dmc/Arr.h"

/// FUN(new) initializates an array
CT *FUN(new)(void);

/// FUN(size) returns 'this' size
size_t FUN(size)(CT *this);

/// FUN(add) adds an element
void FUN(add)(CT *this, TY *element);

/// FUN(add_arr) dds an array
void FUN(add_arr)(CT *this, CT *another);

/// FUN(get) returns element at position 'index'. Tests limits.<br>
/// If the element is NULL throws a exception.
TY *FUN(get)(CT *this, size_t index);

/// FUN(set) replaces element at 'index' by a new 'element'. Tests limits
/// and 'element' can not be NULL.<p>
void FUN(set)(CT *this, size_t index, TY *element);

/// FUN(insert) inserts an element at 'index'. Tests limits and 'element' can
/// no be NULL.<p>
void FUN(insert)(CT *this, size_t index, TY *element);

/// FUN(insert_arr) inserts an array at 'index'. Tests limits.<p>
void FUN(insert_arr)(CT *this, size_t index, CT *another);

/// FUN(remove) removes the elemente at 'index'. Tests limits.<p>
void FUN(remove)(CT *this, size_t index);

/// FUN(remove_range) removes elements between 'begin' (inclusive) and 'end'
/// (exclusive). Tests limits.<p>
void FUN(remove_range)(CT *this, size_t begin, size_t end);

/// FUN(reverse) reverses elements of 'this'
void FUN(reverse)(CT *this);

/// FUN(sort) sorts elements of 'this' according 'f'
///   f: Function which returns 'true' if the order is ascendent and e1 > e2,
///      or the order is descendent and e2 > e1
void FUN(sort)(CT *this, bool (*f)(TY *e1, TY *e2));

/// FUN(shuflle) remix 'this' elements. It should be used after calling
/// <tt>rnd_init()</tt> or <tt>sys_init()</tt>
void FUN(shuffle)(CT *this);

///
IT *FUN(to_it)(CT *this);

///
CT *FUN(from_it)(IT *it);
