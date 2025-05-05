
// arr ---------------------------------

// [.A], (A->b) -> b
Val __arr_all (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  int r = TRUE;
  while (p < end) {
    Val v = *p++;
    r = r && (((Val (*)(Val))fn.o)(v)).b;
  }
  return (Val)r;
}

// [.A], (A->b) -> b
Val __arr_any (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  int r = FALSE;
  while (p < end) {
    Val v = *p++;
    r = r || (((Val (*)(Val))fn.o)(v)).b;
  }
  return (Val)r;
}

// [.A]->()
void __arr_clear (Val a) {
  a.a->end = a.a->begin;
}

// [.A],[.A]->()
void __arr_cat (Val a, Val a2) {
  int64_t other_size = __arr_size(a2).i;
  if (other_size) {
    int64_t this_size = __arr_size(a).i;
    int64_t this_buffer = a.a->end_bf - a.a->begin;
    if (this_size + other_size >= this_buffer){
      int64_t new_buffer = this_buffer + other_size + 15;
      a.a->begin = GC_REALLOC(a.a->begin, new_buffer * sizeof(Val));
      a.a->end = a.a->begin + this_size;
      a.a->end_bf = a.a->begin + new_buffer;
    }
    Val *s = a2.a->begin;
    Val *s_end = a2.a->end;
    Val *t = a.a->end;
    while (s < s_end) *t++ = *s++;
    a.a->end = t;
  }
}

// [.A],[.A]->[.A]
Val __arr_cat2 (Val a, Val a2) {
  Val r = __arr_copy(a);
  __arr_cat(r, a2);
  return r;
}

// [.A]->[.A]
Val __arr_copy (Val a) {
  Varr *this = a.a;
  int64_t size = this->end - this->begin;
  int64_t buffer = this->end_bf - this->begin;
  Varr *r = MALLOC(Varr);
  Val *begin = GC_MALLOC(buffer * sizeof(Val));
  memcpy(begin, this->begin, size * sizeof(Val));
  r->begin = begin;
  r->end = begin + size;
  r->end_bf = begin + buffer;
  return (Val)r;
}

// [.A],i->[.A]
Val __arr_drop (Val a, Val n) {
  if (n.i < 0) return __arr_copy(a);
  if (n.i >= __arr_size(a).i) return __arr_new();
  Val r = __arr_new();
  Val *p = a.a->begin + n.i;
  Val *end = a.a->end;
  while (p < end) __arr_push(r, *p++);
  return r;
}

// [.A], (A->b) -> b
Val __arr_dropWhile (Val a, Val fn) {
  Val r = __arr_new();
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    if (!(((Val (*)(Val))fn.o)(*p)).b) break;
    ++p;
  }
  while (p < end) __arr_push(r, *p++);
  return r;
}

// [.A], (A,A->b) -> b
Val __arr_duplicates (Val a, Val fn) {
  Val r = __arr_new();
  Val dup = __arr_new();
  Val *ap = a.a->begin;
  Val *aend = a.a->end;
  while (ap < aend) {
    Val e = *ap++;
    Val *rp = r.a->begin;
    Val *rend = r.a->end;
    int missing = TRUE;
    while (rp < rend) {
      Val e2 = *rp++;
      if ((((Val (*)(Val,Val))fn.o)(e, e2)).b) {
        missing = FALSE;

        Val *dp = dup.a->begin;
        Val *dend = dup.a->end;
        int missing2 = TRUE;
        while (dp < dend) {
          Val e2 = *dp++;
          if ((((Val (*)(Val,Val))fn.o)(e, e2)).b) {
            missing2 = FALSE;
            break;
          }
        }
        if (missing2) __arr_push(dup, e);
        break;
      }
    }
    if (missing) __arr_push(r, e);
  }
  return __arr_new_from_carr((Val)2, (Val[]){r, dup});
}

// [.A], (A->()) -> ()
void __arr_each (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) ((void (*)(Val))fn.o)(*p++);
}

// [.A], (A,i->()) -> ()
void __arr_eachIx (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  int64_t i = 0;
  while (p < end) ((void (*)(Val,Val))fn.o)(*p++, ((Val)i++));
}

// [.A], (A->b) -> b
Val __arr_filter (Val a, Val fn) {
  Val r = __arr_new();
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    Val e = *p++;
    if ((((Val (*)(Val))fn.o)(e)).b) __arr_push(r, e);
  }
  return (Val)r;
}

// [.A], (A->b) -> ()
void __arr_filterIn (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  Val *new_end = p;
  while (p < end) {
    Val e = *p++;
    if ((((Val (*)(Val))fn.o)(e)).b) *new_end++ = e;
  }
  a.a->end = new_end;
}

// [.A], (A->b) -> ([.A]|[])
Val __arr_find (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    Val e = *p++;
    if ((((Val (*)(Val))fn.o)(e)).b)
      return __arr_new_from_carr((Val)1, (Val[]){e});
  }
  return __arr_new();
}

// s,[! A] -> [A]
Val __arr_fromIter (Val it) {
  Val a = __arr_new();
  while (__iter_hasNext(it).b) __arr_push(a, __iter_next(it));
  return a;
}

// s,(s->A) -> [A]
Val __arr_fromJs (Val s, Val fn) {
  return __arr_map(__js_ra(s), fn);
}

// Auxiliar of __js_ra
// Arr<char>->[.s]
Val __arr_from_sarr (Arr *a) {
  Val r = __arr_new();
  EACH(a, char, el) {
    __arr_push(r, (Val)el);
  }_EACH
  return r;
}

// [.A],i->A
Val __arr_get (Val a, Val ix) {
  return a.a->begin[ix.i];
}

// s,[.A],i->A
Val __arr_get2 (char *pos, Val a, Val ix) {
  Varr *arr = a.a;
  int64_t sz = arr->end - arr->begin;
  int64_t i = ix.i;
  if (i < 0 || i >= sz)
    ___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
  return arr->begin[i];
}

// [.A], (A->b) -> i
Val __arr_index (Val a, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  int64_t i = 0;
  while (p < end) {
    if ((((Val (*)(Val))fn.o)(*p++)).b) return (Val)i;
    ++i;
  }
  return (Val)(int64_t)-1;
}

// s,[.A],i,A->()
void __arr_insert (char *pos, Val a, Val ix, Val e) {
  int64_t size = __arr_size(a).i;
  if (ix.i < 0 || ix.i > size)
    ___built_throw(pos, (Val)str_f("index %d out of [0,%d]", ix.i, size));

  __arr_push(a, e);
  Val *p = a.a->end;
  Val *pix = a.a->begin + ix.i;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

// s,[.A],i,[.A]->()
void __arr_insertArr (char *pos, Val a, Val ix, Val other) {
  __arr_insertRange(pos, a, ix, other, (Val)0, __arr_size(other));
}

// s,[.A],i,[.A],i,i->()
void __arr_insertRange (char *pos, Val this, Val ix, Val other, Val begin, Val end) {
  int64_t this_len = __arr_size(this).i;
  if (ix.i < 0 || ix.i > this_len)
    ___built_throw(pos, (Val)str_f("index %d out of [0,%d]", ix.i, this_len));

  int64_t other_size = __arr_size(other).i;
  if (end.i < begin.i)
    ___built_throw(pos, (Val)str_f("end(%d) < begin(%d)", end.i, begin.i));
  if (begin.i < 0)
    ___built_throw(pos, (Val)str_f("begin(%d) < 0", begin.i));
  if (end.i > other_size)
    ___built_throw(pos, (Val)str_f("end(%d) > size(%d)", end.i, other_size));

  int64_t other_len = end.i - begin.i;
  if (!other_len) return;

  int64_t new_len = this_len + other_len;
  int64_t new_size = new_len + 15;
  Val *new_begin = GC_MALLOC(new_size * sizeof(Val));
  memcpy(new_begin, this.a->begin, sizeof(Val) * ix.i);
  memcpy(new_begin + ix.i, other.a->begin + begin.i, sizeof(Val) * other_len);
  memcpy(
    new_begin + (ix.i + other_len),
    this.a->begin + ix.i,
    sizeof(Val) * (__arr_size(this).i - ix.i)
  );
  this.a->begin = new_begin;
  this.a->end = new_begin + new_len;
  this.a->end_bf = new_begin + new_size;
}

// [.s],s -> s
Val __arr_join(Val a, Val sep) {
  Buf *bf = buf_new();
  int first = 1;
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    if (first) {
      first = 0;
    } else {
      buf_add(bf, sep.s);
    }
    buf_add(bf, (*p++).s);
  }
  return (Val)str_new(buf_str(bf));
}

// [.A],(A->B)->[.B]
Val __arr_map(Val a, Val fn) {
  Val r = __arr_new();
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) __arr_push(r, ((Val (*)(Val))fn.o)(*p++));
  return r;
}

// ->[A.]
Val __arr_new (void) {
  Varr *this = MALLOC(Varr);
  this->begin = GC_MALLOC(15 * sizeof(Val));
  this->end = this->begin;
  this->end_bf = this->begin + 15;
  return (Val)this;
}

// A,i->[.A]
Val __arr_new_fill (Val el, Val n) {
  int64_t size = n.i < 0 ? 0 : n.i;
  int64_t bf_size = size + 15;
  Varr *this = MALLOC(Varr);
  this->begin = GC_MALLOC(bf_size * sizeof(Val));
  this->end = this->begin + size;
  this->end_bf = this->begin + bf_size;

  Val *p = this->begin;
  Val *end = this->end;
  while (p < end) *p++ = el;
  return (Val)this;
}

// i,*A->[.A]
Val __arr_new_from_carr (Val n, Val *els) {
  Val r = __arr_new();
  REPEAT(n.i) {
    __arr_push(r, *els++);
  }_REPEAT
  return r;
}

// [.A]->A
Val __arr_peek (char *pos, Val a) {
  int64_t size = __arr_size(a).i;
  if (size == 0) ___built_throw(pos, (Val)"Array is empty");
  return *(a.a->end - 1);
}

// [.A]->A
Val __arr_pop (char *pos, Val a) {
  int64_t size = __arr_size(a).i;
  if (size == 0) ___built_throw(pos, (Val)"Array is empty");
  return *(a.a->end-- - 1);
}

// [.A],A->()
void __arr_push (Val a, Val el) {
  Varr *this = a.a;
  if (this->end == this->end_bf) {
    int size = this->end_bf - this->begin;
    int new_size = size + size;
    this->begin = GC_REALLOC(this->begin, new_size * sizeof(Val));
    this->end = this->begin + size;
    this->end_bf = this->begin + new_size;
  }
  *this->end++ = el;
}

// [.A]->A
Val __arr_reduce (Val a, Val seed, Val fn) {
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) seed = ((Val (*)(Val,Val))fn.o)(seed, *p++);
  return seed;
}

// [.A],i->[.A]
Val __arr_remove (char *pos, Val a, Val ix) {
  int64_t sz = __arr_size(a).i;
  int64_t i = ix.i;
  if (i < 0 || i >= sz)
    ___built_throw(pos, (Val)str_f("index %d out of [0,%d]", i, sz - 1));

  Val *p = a.a->begin + ix.i;
  Val r = *p;
  Val *p1 = p + 1;
  Val *end = a.a->end--;
  while (p1 < end) *p++ = *p1++;

  return r;
}

// [.A],i,i->[.A]
Val __arr_removeRange (char *pos, Val a, Val begin, Val end) {
  int64_t sz = __arr_size(a).i;
  int64_t b = begin.i;
  int64_t e = end.i;
  if (b < 0 || b >= sz)
    ___built_throw(pos, (Val)str_f("begin index %d out of [0,%d]", b, sz - 1));
  if (e < b || e > sz)
    ___built_throw(pos, (Val)str_f("end index %d out of [%d,%d]", e, b, sz));

  int64_t len = e - b;
  Val r = __arr_take(__arr_drop(a, begin), (Val)len);

  Val *p = a.a->begin + b;
  Val *p1 = a.a->begin + e;
  Val *pend = a.a->end;
  a.a->end -= len;
  while (p1 < pend) *p++ = *p1++;

  return r;
}

// [.A]->[.A]
Val __arr_reverse (Val a) {
  Val r = __arr_copy(a);
  __arr_reverseIn(r);
  return r;
}

// [.A]->[.A]
void __arr_reverseIn (Val a) {
  int64_t sz = __arr_size(a).i;
  Val *p = a.a->begin;
  Val *end = a.a->end - 1;
  Val tmp;
  for (int i = 0; i < sz / 2; ++i) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }
}

// [.A],i,A->()
void __arr_set (Val a, Val ix, Val el) {
  a.a->begin[ix.i] = el;
}

// s,[.A],i,A->()
void __arr_set2 (char *pos, Val a, Val ix, Val el) {
  Varr *arr = a.a;
  int64_t sz = arr->end - arr->begin;
  int64_t i = ix.i;
  if (i < 0 || i >= sz)
    ___built_throw(pos, (Val)str_f("index %d out of [0,%d]", i, sz - 1));

  arr->begin[i] = el;
}

// s,[.A],i,[.A]->()
void __arr_setArr (char *pos, Val a, Val ix, Val other) {
  __arr_setRange(pos, a, ix, other, (Val)0, __arr_size(other));
}

// s,[.A],i,[.A],i,i->()
void __arr_setRange (char *pos, Val this, Val ix, Val other, Val begin, Val end) {
  int64_t this_len = __arr_size(this).i;
  int64_t other_size = __arr_size(other).i;

  if (ix.i < 0 || ix.i > this_len - other_size)
    ___built_throw(
      pos, (Val)str_f("index %d out of [0,%d]", ix.i, this_len - other_size)
    );

  if (end.i < begin.i)
    ___built_throw(pos, (Val)str_f("end(%d) < begin(%d)", end.i, begin.i));
  if (begin.i < 0)
    ___built_throw(pos, (Val)str_f("begin(%d) < 0", begin.i));
  if (end.i > other_size)
    ___built_throw(pos, (Val)str_f("end(%d) > size(%d)", end.i, other_size));

  int64_t other_len = end.i - begin.i;
  if (!other_len) return;

  Val *target = this.a->begin + ix.i;
  Val *source = other.a->begin + begin.i;
  memcpy(target, source, sizeof(Val *) * other_len);
}

// [.A]->A
Val __arr_shift (char *pos, Val a) {
  int64_t size = __arr_size(a).i;
  if (size == 0) ___built_throw(pos, (Val)"Array is empty");

  Val *p = a.a->begin;
  Val r = *p;
  Val *p1 = p + 1;
  Val *end = a.a->end--;
  while (p1 < end) *p++ = *p1++;

  return r;
}

// [.A]->A
void __arr_shuffle (Val a) {
  int64_t size = __arr_size(a).i;
  Val *begin = a.a->begin;
  Val *p = a.a->end - 1;
  Val *pix, tmp;
  while (p > begin) {
    pix = begin + sys_rnd_i(size--);
    tmp = *p;
    *p-- = *pix;
    *pix = tmp;
  }
}

// [.A]->i
Val __arr_size (Val a) {
  return (Val)(a.a->end - a.a->begin);
}

// [.A],(AA->b)->()
void __arr_sort (Val a, Val fn) {
  Val (*less0)(Val, Val) = (Val (*)(Val, Val))fn.o;
  int less (Val e1, Val e2) {
    return less0(e1, e2).b;
  }

  void sort(Val *a, int64_t size) {
    if (size < 2) {
      return;
    }
    if (size == 2) {
      if (less(a[1], a[0])) { Val tmp = a[0]; a[0] = a[1]; a[1] = tmp; }
      return;
    }
    if (size == 3) {
      if (less(a[1], a[0])) { Val tmp = a[0]; a[0] = a[1]; a[1] = tmp; }
      if (less(a[2], a[0])) { Val tmp = a[0]; a[0] = a[2]; a[2] = tmp; }
      if (less(a[2], a[1])) { Val tmp = a[1]; a[1] = a[2]; a[2] = tmp; }
    }
    int64_t mid1 = size >> 1;
    int64_t mid2 = size - mid1;
    Val a1[mid1];
    Val a2[mid2];
    Val *pa = a;
    Val *pa1 = a1;
    Val *pa2 = a2;
    for (int64_t i = 0; i < mid1; ++i) *pa1++ = *pa++;
    for (int64_t i = 0; i < mid2; ++i) *pa2++ = *pa++;
    sort(a1, mid1);
    sort(a2, mid2);

    pa = a;
    pa1 = a1;
    Val *pa1_end = a1 + mid1;
    pa2 = a2;
    Val *pa2_end = a2 + mid2;

    for(;;) {
      if (pa1 == pa1_end) {
        while (pa2 < pa2_end) *pa++ = *pa2++;
        break;
      }
      if (pa2 == pa2_end) {
        while (pa1 < pa1_end) *pa++ = *pa1++;
        break;
      }
      if (less(*pa2, *pa1)) *pa++ = *pa2++;
      else *pa++ = *pa1++;
    }
  }

  sort(a.a->begin, __arr_size(a).i);
}

// [.A],i,i->[.A]
Val __arr_sub (Val a, Val begin, Val end) {
  if (begin.i < 0) begin = (Val)(__arr_size(a).i + begin.i);
  if (end.i < 0) end = (Val)(__arr_size(a).i + end.i);
  return __arr_drop(__arr_take(a, end), begin);
}

// [.A],i->[.A]
Val __arr_sub0 (Val a, Val begin) {
  if (begin.i < 0) begin = (Val)(__arr_size(a).i + begin.i);
  return __arr_drop(a, begin);
}

// [.A],i->[A.]
Val __arr_take (Val a, Val n) {
  if (n.i > __arr_size(a).i) return __arr_copy(a);
  Val r = __arr_new();
  Val *p = a.a->begin;
  for (int64_t i = 0; i < n.i; ++i) __arr_push(r, *p++);
  return r;
}

// [.A], (A->b) -> b
Val __arr_takeWhile (Val a, Val fn) {
  Val r = __arr_new();
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    if (!(((Val (*)(Val))fn.o)(*p)).b) break;
    __arr_push(r, *p++);
  }
  return r;
}

Val __arr_toIter_next (Val obj) {
  Val *p = obj.a->begin;
  Val a = *p;
  int64_t sz = (p+1)->i;
  int64_t c = (p+2)->i;
  if (c < sz) {
    Val r = *(a.a->begin+c);
    *(p+2) = (Val)(c + 1);
    return __arr_new_from_carr((Val)1, (Val[]){r});
  }
  return __arr_new();
}

// [.A] -> [!A]
Val __arr_toIter (Val a) {
  return __iter_new(__arr_new_from_carr((Val)2, (Val[]){
    __arr_new_from_carr((Val)3, (Val[]){a, __arr_size(a), (Val)0}),
    (Val)(void *)__arr_toIter_next
  }));
}

// [.A],(A->s)->[s.]
Val __arr_toJs (Val a, Val fn) {
  return __js_wa(__arr_map(a, fn));
}

// Auxiliar of __js_wa
// [.s]->Arr<char>
Arr *__arr_to_sarr (Val a) {
  Arr *r = arr_new();
  Varr *va = a.a;
  Val *p = va->begin;
  Val *end = va->end;
  while (p < end) arr_push(r, (*p++).s);
  return r;
}

// [.A],A->()
void __arr_unshift (Val a, Val e) {
  __arr_push(a, e);
  Val *p = a.a->end;
  Val *pix = a.a->begin;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

// [.[AB]] -> [[.A][.B]]
Val __arr_unzip (Val a) {
  Val a1 = __arr_new();
  Val a2 = __arr_new();
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    Val t = *p++;
    __arr_push(a1, *(t.a->begin));
    __arr_push(a2, *(t.a->begin + 1));
  }
  return __arr_new_from_carr((Val)2, (Val[]){a1, a2});
}

// [.[ABC]] -> [[.A][.B][.C]]
Val __arr_unzip3 (Val a) {
  Val a1 = __arr_new();
  Val a2 = __arr_new();
  Val a3 = __arr_new();
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) {
    Val t = *p++;
    __arr_push(a1, *(t.a->begin));
    __arr_push(a2, *(t.a->begin + 1));
    __arr_push(a3, *(t.a->begin + 2));
  }
  return __arr_new_from_carr((Val)3, (Val[]){a1, a2, a3});
}

Val __arr_zip (Val a1, Val a2) {
  Val r = __arr_new();
  int64_t sz = __arr_size(a1).i < __arr_size(a2).i
    ? __arr_size(a1).i
    : __arr_size(a2).i
  ;

  int64_t c = 0;
  Val *p1 = a1.a->begin;
  Val *p2 = a2.a->begin;
  while (c < sz) {
    __arr_push(r, __arr_new_from_carr((Val)2, (Val[]){*p1++, *p2++}));
    ++c;
  }
  return r;
}

// [.A],[.B][.C] -> [.[ABC]]
Val __arr_zip3 (Val a1, Val a2, Val a3) {
  Val r = __arr_new();
  int64_t sz = __arr_size(a1).i < __arr_size(a2).i
    ? __arr_size(a1).i
    : __arr_size(a2).i
  ;
  sz = sz < __arr_size(a3).i
    ? sz
    : __arr_size(a2).i < __arr_size(a3).i
      ? __arr_size(a2).i
      : __arr_size(a3).i
  ;

  int64_t c = 0;
  Val *p1 = a1.a->begin;
  Val *p2 = a2.a->begin;
  Val *p3 = a3.a->begin;
  while (c < sz) {
    __arr_push(r, __arr_new_from_carr((Val)3, (Val[]){*p1++, *p2++, *p3++}));
    ++c;
  }
  return r;
}
