// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header It template.<p>
/// Use:
///   ///
///   typedef struct ichar_Ichar Ichar;
///   #define TY char                    // Element type
///   #define IT Ichar                   // Iterator type
///   #define OP Ochar                   // Option type
///   #define FUN(id) ichar ## _ ## id   // Defintion for functions
///   #include "dmc/tpl/tit.h"
///   #undef TY
///   #undef IT
///   #undef OP
///   #undef FUN

#include "dmc/It.h"

IT *FUN(new)(void *o, TY *(*next)(void *o));

///
IT *FUN(empty)(void);

///
IT *FUN(unary)(TY *e);

/// FUN(range is an iterator that returns values between begin (inclusive)
/// and end (exclusive)
IT *FUN(range)(int begin, int end);

/// FUN(range0 is equals to FUN(range(0, end);
IT *FUN(range0)(int end);

///
bool FUN(has_next)(IT *this);

///
TY *FUN(next)(IT *this);

// Show next element witout advancing.
TY *FUN(peek)(IT *this);

/// FUN(add adds an element at the end of 'this'
IT *FUN(add)(IT *this, TY *element);

/// FUN(add adds an element at the beginning of 'this'
IT *FUN(add0)(IT *this, TY *element);

///
IT *FUN(cat)(IT *this, IT *another);

///
IT *FUN(take)(IT *this, size_t n);

///
IT *FUN(takef)(IT *this, bool (*predicate)(TY *e));

///
IT *FUN(drop)(IT *this, size_t n);

///
IT *FUN(dropf)(IT *this, bool (*predicate)(TY *e));

///
IT *FUN(filter)(IT *this, bool (*predicate)(TY *e));

///
IT *FUN(map)(IT *this, TY *(*converter)(TY *e));

/// FUN(map2 applies conv1 to the first element and conv2 to the others.
IT *FUN(map2)(IT *this, TY *(*conv1)(TY *e), TY *(*conv2)(TY *e));

///
IT *FUN(reverse)(IT *this);

///
IT *FUN(sort)(IT *this, bool (*comparator)(TY *e1, TY *e2));

///
void FUN(each)(IT *this, void (*f)(TY *e));

///
void FUN(each_ix)(IT *this, void (*f)(TY *e, size_t ix));

///
size_t FUN(count)(IT *this);

///
bool FUN(eq)(IT *it1, IT *it2, bool (*feq)(TY *e1, TY *e2));

///
int FUN(index)(IT *this, bool (*predicate)(TY *e));

///
bool FUN(contains)(IT *this, bool (*predicate)(TY *e));

///
int FUN(last_index)(IT *this, bool (*predicate)(TY *e));

/// Returns the first element which satisfies 'predicate' or opt_null.
OP *FUN(find)(IT *this, bool (*predicate)(TY *e));

/// Returns the first element which satisfies 'predicate' or 'option'.
TY *FUN(ofind)(IT *this, TY *option, bool (*predicate)(TY *e));
