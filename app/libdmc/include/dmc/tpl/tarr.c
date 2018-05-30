// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code array template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define CT Achar                   // Container type
///   #define IT Ichar                   // Iterator type
///   #define FUN(id) achar ## _ ## id   // Defintion for functions
///   #include "dmc/tpl/tarr.c"
///   #undef TY
///   #undef CT
///   #undef IT
///   #undef FUN


#include "dmc/Arr.h"

inline
CT *FUN(new)(void) {
  return (CT *)arr_new();
}

inline
size_t FUN(size) (CT *this) {
  return arr_size((Arr *)this);
}

inline
void FUN(add) (CT *this, TY *element) {
  arr_add((Arr *)this, element);
}

inline
void FUN(add_arr)(CT *this, CT *another) {
  arr_add_arr((Arr *)this, (Arr *)another);
}

inline
TY *FUN(get) (CT *this, size_t index) {
  return arr_get((Arr *)this, index);
}

inline
void FUN(set)(CT *this, size_t index, TY *element) {
  arr_set((Arr *)this, index, element);
}

inline
void FUN(insert)(CT *this, size_t index, TY *element) {
  arr_insert((Arr *)this, index, element);
}

inline
void FUN(insert_arr)(CT *this, size_t index, CT *another) {
  arr_insert_arr((Arr *)this, index, (Arr *)another);
}

inline
void FUN(remove)(CT *this, size_t index) {
  arr_remove((Arr *)this, index);
}

inline
void FUN(remove_range)(CT *this, size_t begin, size_t end) {
  arr_remove_range((Arr *)this, begin, end);
}

inline
void FUN(reverse)(CT *this) {
  arr_reverse((Arr *)this);
}

inline
void FUN(sort)(CT *this, bool (*f)(TY *e1, TY *e2)) {
  arr_sort((Arr*)this, (bool(*)(void *, void*))f);
}

inline
void FUN(shuffle)(CT *this) {
  arr_shuffle((Arr *)this);
}

inline
IT *FUN(to_it)(CT *this) {
  return (IT*) arr_to_it((Arr *)this);
}

inline
CT *FUN(from_it)(IT *it) {
  return arr_from_it((It *)it);
}
