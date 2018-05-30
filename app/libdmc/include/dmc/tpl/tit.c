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
///   #include "dmc/tpl/tit.c"
///   #undef TY
///   #undef IT
///   #undef OP
///   #undef FUN

#include "dmc/It.h"

inline
IT *FUN(new)(void *o, TY *(*next)(void *o)) {
  return (IT *)it_new(o, (void *(*)(void *))next);
}

inline
IT *FUN(empty)(void) {
  return (IT *)it_empty();
}

inline
IT *FUN(unary)(TY *e) {
  return (IT*)it_unary(e);
}

inline
IT *FUN(range)(int begin, int end) {
  return (IT*)it_range(begin, end);
}

inline
IT *FUN(range0)(int end) {
  return (IT*)it_range0(end);
}

inline
bool FUN(has_next)(IT *this) {
  return it_has_next((It *)this);
}

inline
TY *FUN(next)(IT *this) {
  return it_next((It *)this);
}

inline
TY *FUN(peek)(IT *this) {
  return it_peek((It *)this);
}

inline
IT *FUN(add)(IT *this, TY *element) {
  return (IT *)it_add((It *)this, element);
}

inline
IT *FUN(add0)(IT *this, TY *element) {
  return (IT *)it_add0((It *)this, element);
}

inline
IT *FUN(cat)(IT *this, IT *another) {
  return (IT *)it_cat((It *)this, (It *)another);
}

inline
IT *FUN(take)(IT *this, size_t n) {
  return (IT *)it_take((It *)this, n);
}

inline
IT *FUN(takef)(IT *this, bool (*predicate)(TY *e)) {
  return (IT *)it_takef((It *)this, (bool(*)(void *))predicate);
}

inline
IT *FUN(drop)(IT *this, size_t n) {
  return (IT *)it_drop((It *)this, n);
}

inline
IT *FUN(dropf)(IT *this, bool (*predicate)(TY *e)) {
  return (IT *)it_dropf((It *)this, (bool(*)(void *))predicate);
}

inline
IT *FUN(filter)(IT *this, bool (*predicate)(TY *e)) {
  return (IT *)it_filter((It *)this, (bool(*)(void *))predicate);
}

inline
IT *FUN(map)(IT *this, TY *(*converter)(TY *e)) {
  return (IT *)it_map((It *)this, (void *(*)(void *))converter);
}

inline
IT *FUN(map2)(IT *this, TY *(*conv1)(TY *e), TY *(*conv2)(TY *e)) {
  return (IT *)it_map2(
    (It *)this, (void *(*)(void *))conv1, (void *(*)(void *))conv2
  );
}

inline
IT *FUN(reverse)(IT *this) {
  return (IT*)it_reverse((It *)this);
}

inline
IT *FUN(sort)(IT *this, bool (*comparator)(TY *e1, TY *e2)) {
  return (IT *)it_sort((It *)this, (bool(*)(void *, void *))comparator);
}

inline
void FUN(each)(IT *this, void (*f)(TY *e)) {
  it_each((It *)this, (void(*)(void*))f);
}

inline
void FUN(each_ix)(IT *this, void (*f)(TY *e, size_t ix)) {
  it_each_ix((It *)this, (void (*)(void *, size_t))f);
}

inline
size_t FUN(count)(IT *this) {
  return it_count((It *)this);
}

inline
bool FUN(eq)(IT *it1, IT *it2, bool (*feq)(TY *e1, TY *e2)) {
  return it_eq((It *)it1, (It *)it2, (bool (*)(void *, void *))feq);
}

inline
int FUN(index)(IT *this, bool (*predicate)(TY *e)) {
  return it_index((It *)this, (bool (*)(void *))predicate);
}

inline
bool FUN(contains)(IT *this, bool (*predicate)(TY *e)) {
  return it_contains((It *)this, (bool (*)(void *))predicate);
}

inline
int FUN(last_index)(IT *this, bool (*predicate)(TY *e)) {
  return it_last_index((It *)this, (bool (*)(void *))predicate);
}

inline
OP *FUN(find)(IT *this, bool (*predicate)(TY *e)) {
  return (OP *)it_find((It *)this, (bool (*)(void *))predicate);
}

inline
TY *FUN(ofind)(IT *this, TY *option, bool (*predicate)(TY *e)) {
  return (TY *)it_ofind((It *)this, option, (bool (*)(void *))predicate);
}
