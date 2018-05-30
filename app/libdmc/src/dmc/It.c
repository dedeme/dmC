// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/It.h"
#include "dmc/Opt.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/Arr.h"
#include "dmc/DEFS.h"

struct it_It {
  void *o;
  void *e;
  void *(*next)(void *o);
};

It *it_new(
  void *o,
  void *(*next)(void *o)
) {
  It *this = MALLOC(It);
  this->o = o;
  this->next = next;
  this->e = next(o);
  return this;
}

/**/static void *empty_next (void *o) { return NULL; }
It *it_empty() {
  return it_new(NULL, empty_next);
}

/**/typedef struct {
/**/  void *e;
/**/  int is_first;
/**/} it_unary_O;
/**/static FNM (unary_next, it_unary_O, o) {
/**/  return o->is_first ? o->e : NULL;
/**/}_FN
It *it_unary(void *e) {
  XNULL(e)

  it_unary_O *o = MALLOC(it_unary_O);
  o->e = e;
  o->is_first = true;
  return it_new(o, unary_next);
}

/**/typedef struct {
/**/  int *i;
/**/  int *end;
/**/} it_range_O;
/**/static FNM (range_next, it_range_O, o) {
/**/  int i = *o->i;
/**/  if (i >= *(o->end)) {
/**/    return NULL;
/**/  }
/**/  *(o->i) = ++i;
/**/  return o->i;
/**/}_FN
It *it_range(int begin, int end) {
  it_range_O *o = MALLOC(it_range_O);
  o->i = ATOMIC(sizeof(int));
  o->end = ATOMIC(sizeof(int));
  *(o->i) = begin;
  *(o->end) = end;
  return it_new(o, range_next);
}

It *it_range0(int end) {
  return it_range(0, end);
}

bool it_has_next (It *this) {
  XNULL(this)
  return this->e;
}

inline
void *it_next (It *this) {
  XNULL(this)
  void *r = this->e = this->next(this->o);
  XNULL(r)
  return r;
}

inline
void *it_peek (It *this) {
  XNULL(this)
  XNULL(this->e)
  return this->e;
}

It *it_add(It *this, void *element) {
  XNULL(this)
  return it_cat(this, it_unary(element));
}

It *it_add0(It *this, void *element) {
  XNULL(this)
  return it_cat(it_unary(element), this);
}

/**/typedef struct {
/**/  It *it1;
/**/  It *it2;
/**/} it_cat_O;
/**/static FNM (cat_next, it_cat_O, o) {
/**/  return (it_has_next(o->it1)) ? it_next(o->it1) : it_next(o->it2);
/**/}_FN
It *it_cat (It *this, It *another) {
  XNULL(this)
  XNULL(another)

  it_cat_O *o = MALLOC(it_cat_O);
  o->it1 = this;
  o->it2 = another;
  return it_new(o, cat_next);
}

/**/typedef struct {
/**/  It *it;
/**/  size_t i;
/**/  size_t n;
/**/} it_take_O;
/**/static FNM (take_next, it_take_O, o) {
/**/  if (o->i < o->n && it_has_next(o->it)) {
/**/    o->i += 1;
/**/    return it_next(o->it);
/**/  }
/**/  else return NULL;
/**/} _FN
It *it_take (It *this, size_t n) {
  XNULL(this)

  it_take_O *o = MALLOC(it_take_O);
  o->it = this;
  o->n = n;
  o->i = 0;
  return it_new(o, take_next);
}

/**/typedef struct {
/**/  It *it;
/**/  bool (*f)(void *e);
/**/} it_takef_O;
/**/static FNM (takef_next, it_takef_O, o) {
/**/  return o->f(it_peek(o->it)) && it_has_next(o->it) ? it_next(o->it) : NULL;
/**/} _FN
It *it_takef (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  it_takef_O *o = MALLOC(it_takef_O);
  o->it = this;
  o->f = predicate;
  return it_new(o, takef_next);
}

It *it_drop (It *this, size_t n) {
  XNULL(this)

  size_t i = 0;
  while (it_has_next(this) && i++ < n)
    it_next(this);
  return this;
}

It *it_dropf (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  while (it_has_next(this) && predicate(it_peek(this)))
    it_next(this);
  return this;
}

/**/typedef struct {
/**/  It *it;
/**/  bool (*f)(void *e);
/**/} it_filter_O;
/**/static FNM (filter_next, it_filter_O, o) {
/**/  for(;;) {
/**/    void *next = it_next(o->it);
/**/    if (next && !o->f(next)) continue;
/**/    return next;
/**/  }
/**/} _FN
It *it_filter (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  it_filter_O *o = MALLOC(it_filter_O);
  o->it = this;
  o->f = predicate;
  filter_next(o);
  return it_new(o, filter_next);
}

/**/typedef struct {
/**/  It *it;
/**/  void *(*f)(void *);
/**/} it_map_O;
/**/static FNM (map_next, it_map_O, o) {
/**/  return it_has_next(o->it) ? o->f(it_next(o->it)) : NULL;
/**/} _FN
It *it_map (It *this, void *(*converter)(void *e)) {
  XNULL(this)

  it_map_O *o = MALLOC(it_map_O);
  o->it = this;
  o->f = converter;
  return it_new(o, map_next);
}

/**/typedef struct {
/**/  It *it;
/**/  bool is_first;
/**/  void *(*f1)(void *);
/**/  void *(*f)(void *);
/**/} map2_O;
/**/static FNM (map2_next, map2_O, o) {
/**/  if (!it_has_next(o->it)) return NULL;
/**/  if (o->is_first) {
/**/    o->is_first = 0;
/**/    return o->f1(it_next(o->it));
/**/  }
/**/  return o->f(it_next(o->it));
/**/} _FN
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e)) {
  XNULL(this)

  map2_O *o = MALLOC(map2_O);
  o->it = this;
  o->is_first = true;
  o->f1 = conv1;
  o->f = conv2;
  return it_new(o, map2_next);
}

It *it_reverse (It *this) {
  XNULL(this)

  Arr *a = arr_from_it(this);
  arr_reverse(a);
  return arr_to_it(a);
}

It *it_sort (It *this, bool (*comparator)(void *, void *)) {
  XNULL(this)

  Arr *a = arr_from_it(this);
  arr_sort(a, comparator);
  return arr_to_it(a);
}

void it_each (It *this, void (*f)(void *e)) {
  XNULL(this)
  while (it_has_next(this)) f(it_next(this));
}

void it_each_ix (It *this, void (*f)(void *e, size_t ix)) {
  XNULL(this)

  size_t c = 0;
  while (it_has_next(this))
    f(it_next(this), c++);
}

size_t it_count (It *this) {
  XNULL(this)

  size_t c = 0;
  while (it_has_next(this)) {
    it_next(this);
    ++c;
  }
  return c;
}

bool it_eq (It *it1, It *it2, bool (*feq)(void *e1, void *e2)) {
  XNULL(it1)
  XNULL(it2)

  while (it_has_next(it1) && it_has_next(it2))
    if (!feq(it_next(it1), it_next(it2)))
      return false;
  if (it_has_next(it1) || it_has_next(it2))
    return false;
  return true;
}

int it_index (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      return ix;
    ++ix;
  }
  return -1;
}

bool it_contains (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  while(it_has_next(this))
    if (predicate(it_next(this)))
      return true;
  return false;
}

int it_last_index (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  int r = -1;
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      r = ix;
    ++ix;
  }
  return r;
}

Opt *it_find (It *this, bool (*predicate)(void *e)) {
  XNULL(this)

  while(it_has_next(this)) {
    void *next = it_next(this);
    if (predicate(next)) {
      return opt_new(next);
    }
  }
  return opt_null();
}

void *it_ofind (It *this, void *option, bool (*predicate)(void *e)) {
  XNULL(this)
  XNULL(option)

  while(it_has_next(this)) {
    void *next = it_next(this);
    if (predicate(next)) {
      return opt_new(next);
    }
  }
  return option;
}

