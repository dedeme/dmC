
// INTERFACE ----------------------------------------


// -------------------------------------
//02-built_in
// -------------------------------------

void ___built_jump (int reference) {
Exc *exc = exc_get();
longjmp(*((jmp_buf *)arr_pop(exc->buf)), reference);
}
char *___built_mpos(char *module_ln) {
char *mpath(char *module_id) {
char *data = file_read(path_cat(__sys_compilerRoot().s, "index.tb", NULL));
Arr *tb = js_ra(data);
EACH(tb, char, e) {
Arr *fs = js_ra(e);
if (!strcmp(js_rs(arr_get(fs, 1)), module_id))
return js_rs(arr_get(fs, 0));
}_EACH
return str_f("?(%s)", module_id);
}
int ix = str_cindex(module_ln, ':');
return str_f("%s%s:", mpath(str_left(module_ln, ix)), module_ln + ix);
}
char *___built_stack_line (char *e) {
char *path = path_cat(__sys_compilerRoot().s, "compilationDb", NULL);
int ix1 = str_last_cindex(e, ')');
if (ix1 == -1) return "";
e = str_left(e, ix1);
ix1 = str_cindex(e, '(');
if (ix1 == -1) return "";
e = str_right(e, ix1 + 1);
ix1 = str_cindex(e, '_');
if (ix1 == -1) return "";
ix1 = str_cindex_from(e, '_', ix1 + 1);
if (ix1 == -1) return "";
int ix2 = str_cindex_from(e, '_', ix1 + 1);
if (ix2 == -1) {
if (str_starts(str_right(e, ix1), "_main+")) ix2 = ix1 + 5;
else return "";
}
char *mdId = str_left(e, ix1);
char *fn = str_sub(e, ix1 + 1, ix2);
char *mdPath = path_cat(path, str_f("%s.mod", mdId), NULL);
if (!file_exists(mdPath)) return "";
Arr *tb = js_ra(file_read(mdPath));
char *fpath = js_rs(arr_get(tb, 0));
if (!strcmp(fn, "main")) return str_f("  %s:[main]", fpath);
Arr *fn_tb = js_ra(arr_get(tb, 3));
int n = -1;
EACH(fn_tb, char, e) {
Arr *fields = js_ra(e);
if (!strcmp(js_rs(arr_get(fields, 1)), fn)) {
n = js_ri(arr_get(fields, 0));
break;
}
}_EACH
if (n != -1) return str_f("  %s:%d:", fpath, n);
return "";
}
Val ___built_divi(char *pos, Val n1, Val n2) {
if (n2.i == 0) ___built_throw(pos, (Val)"Division by 0");
return (Val) (n1.i / n2.i);
}
Val ___built_divf(char *pos, Val n1, Val n2) {
if (math_eq(n2.f, 0)) ___built_throw(pos, (Val)"Division by 0");
return (Val) (n1.f / n2.f);
}
Val ___built_eq (Val type, Val value1, Val value2) {
char *t = __arr_get(type, (Val)0).s;
if (*t == 'b') return (Val)(value1.b == value2.b);
if (*t == 'i') return (Val)(value1.i == value2.i);
if (*t == 'f') return (Val)(math_eq(value1.f, value2.f));
if (*t == 's') return (Val)(!strcmp(value1.s, value2.s));
if (*t == 'a') {
if (__arr_size(value1).i != __arr_size(value2).i) return (Val)FALSE;
Val atp = __arr_get(__arr_get(type, (Val)1), (Val)0);
Varr *a1 = value1.a;
Varr *a2 = value2.a;
Val *e1 = a1->begin;
Val *e2 = a2->begin;
Val *end = a1->end;
while (e1 < end) {
if (!___built_eq(atp, *e1++, *e2++).b) return (Val)FALSE;
}
return (Val)TRUE;
}
if (*t == 'd') {
if (__arr_size(value1).i != __arr_size(value2).i) return (Val)FALSE;
Val dtp = __arr_get(__arr_get(type, (Val)1), (Val)0);
Varr *a = value1.a;
Val *e = a->begin;
Val *end = a->end;
while (e < end) {
Varr *kv = (*e++).a;
Val rs = __dic_get(value2, kv->begin[0]);
if (
!__arr_size(rs).i |
!___built_eq(dtp, kv->begin[1], rs.a->begin[0]).b
) return (Val)FALSE;
}
return (Val)TRUE;
}
if (*t == 't') {
Varr *atp = __arr_get(type, (Val)1).a;
Val *etp = atp->begin;
Val *endtp = atp->end;
Varr *a1 = value1.a;
Varr *a2 = value2.a;
Val *e1 = a1->begin;
Val *e2 = a2->begin;
while (etp < endtp) {
if (!___built_eq(*etp++, *e1++, *e2++).b) return (Val)FALSE;
}
return (Val)TRUE;
}
return (Val)(value1.o == value2.o);
}
Val ___built_to_str_trace (Val type, Val value) {
char *t = __arr_get(type, (Val)0).s;
return (*t == 's')
? __js_ws(value)
: ___built_to_str(type, value)
;
}
Val ___built_to_str (Val type, Val value) {
char *t = __arr_get(type, (Val)0).s;
if (*t == 'b') return value.b ? (Val)"true" : (Val)"false";
if (*t == 'i') return __math_itos(value);
if (*t == 'f') {
char *r = math_ftos(value.f, 9);
if (str_cindex(r, '.') == -1) r = str_f("%s.0", r);
return (Val)r;
}
if (*t == 's') return value;
if (*t == 'a') {
Val atp = __arr_get(__arr_get(type, (Val)1), (Val)0);
Arr *bf = arr_new(); // [s.]
Varr *a = value.a;
Val *e = a->begin;
Val *end = a->end;
while (e < end) {
arr_push(bf, ___built_to_str_trace(atp, *e++).s);
}
return (Val)str_f("[%s]", arr_join(bf, ","));
}
if (*t == 'd') {
Val dtp = __arr_get((__arr_get(type, (Val)1)), (Val)0);
Arr *bf = arr_new(); // [s.]
Varr *a = value.a;
Val *e = a->begin;
Val *end = a->end;
while (e < end) {
Varr *a2 = e++->a;
Val *e2 = a2->begin;
char *key = js_ws((*e2++).s);
char *value = ___built_to_str_trace(dtp, *e2).s;
arr_push(bf, str_f("%s:%s", key, value));
}
return (Val)str_f("{%s}", arr_join(bf, ","));
}
if (*t == 't') {
Arr *bf = arr_new(); // [s.]
Varr *atp = __arr_get(type, (Val)1).a;
Val *etp = atp->begin;
Val *endtp = atp->end;
Varr *a = value.a;
Val *e = a->begin;
while (etp < endtp) {
arr_push(bf, ___built_to_str_trace(*etp++, *e++).s);
}
return (Val)str_f("[. %s]", arr_join(bf, ","));
}
if (*t == 'r') {
Val atp = __arr_get(__arr_get(type, (Val)1), (Val)0);
Arr *bf = arr_new(); // [s.]
while (__iter_hasNext(value).b)
arr_push(bf, ___built_to_str_trace(atp, __iter_next(value)).s);
return (Val)str_f("[! %s]", arr_join(bf, ","));
}
return (Val)str_f("%s(%ld)", t, (long)value.o);
}
void ___built_throw (char *pos, Val msg) {
Exc *exc = exc_get();
void *array[30];
size_t size;
char **strings;
size = backtrace(array, 30);
strings = backtrace_symbols(array, size);
Arr *stack = arr_new();
RANGE0(i, size) {
char *e = ___built_stack_line(strings[i]);
if (*e) arr_push(stack, e);
}_RANGE
free(strings);
exc->stack = stack;
exc->msg = str_f("%s %s", ___built_mpos(pos), msg.s);
exc->type = "";
longjmp(*((jmp_buf *)arr_pop(exc->buf)), 1);
}
void ___built_trace (Val isComplete, char *pos, Val type, Val value) {
char *r = ___built_to_str_trace(type, value).s;
if (!(isComplete.b)) r = arr_join(arr_take(str_runes(r), 50), "");
puts(str_f("%s %s", ___built_mpos(pos), r));
}


// -------------------------------------
//arr
// -------------------------------------

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
void __arr_clear (Val a) {
a.a->end = a.a->begin;
}
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
Val __arr_cat2 (Val a, Val a2) {
Val r = __arr_copy(a);
__arr_cat(r, a2);
return r;
}
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
Val __arr_drop (Val a, Val n) {
if (n.i < 0) return __arr_copy(a);
if (n.i >= __arr_size(a).i) return __arr_new();
Val r = __arr_new();
Val *p = a.a->begin + n.i;
Val *end = a.a->end;
while (p < end) __arr_push(r, *p++);
return r;
}
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
void __arr_each (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) ((void (*)(Val))fn.o)(*p++);
}
void __arr_eachIx (Val a, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
int64_t i = 0;
while (p < end) ((void (*)(Val,Val))fn.o)(*p++, ((Val)i++));
}
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
Val __arr_fromIter (Val it) {
Val a = __arr_new();
while (__iter_hasNext(it).b) __arr_push(a, __iter_next(it));
return a;
}
Val __arr_fromJs (Val s, Val fn) {
return __arr_map(__js_ra(s), fn);
}
Val __arr_from_sarr (Arr *a) {
Val r = __arr_new();
EACH(a, char, el) {
__arr_push(r, (Val)el);
}_EACH
return r;
}
Val __arr_get (Val a, Val ix) {
return a.a->begin[ix.i];
}
Val __arr_get2 (char *pos, Val a, Val ix) {
Varr *arr = a.a;
int64_t sz = arr->end - arr->begin;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
return arr->begin[i];
}
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
void __arr_insertArr (char *pos, Val a, Val ix, Val other) {
__arr_insertRange(pos, a, ix, other, (Val)0, __arr_size(other));
}
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
Val __arr_map(Val a, Val fn) {
Val r = __arr_new();
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) __arr_push(r, ((Val (*)(Val))fn.o)(*p++));
return r;
}
Val __arr_new (void) {
Varr *this = MALLOC(Varr);
this->begin = GC_MALLOC(15 * sizeof(Val));
this->end = this->begin;
this->end_bf = this->begin + 15;
return (Val)this;
}
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
Val __arr_new_from_carr (Val n, Val *els) {
Val r = __arr_new();
REPEAT(n.i) {
__arr_push(r, *els++);
}_REPEAT
return r;
}
Val __arr_peek (char *pos, Val a) {
int64_t size = __arr_size(a).i;
if (size == 0) ___built_throw(pos, (Val)"Array is empty");
return *(a.a->end - 1);
}
Val __arr_pop (char *pos, Val a) {
int64_t size = __arr_size(a).i;
if (size == 0) ___built_throw(pos, (Val)"Array is empty");
return *(a.a->end-- - 1);
}
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
Val __arr_reduce (Val a, Val seed, Val fn) {
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) seed = ((Val (*)(Val,Val))fn.o)(seed, *p++);
return seed;
}
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
Val __arr_reverse (Val a) {
Val r = __arr_copy(a);
__arr_reverseIn(r);
return r;
}
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
void __arr_set (Val a, Val ix, Val el) {
a.a->begin[ix.i] = el;
}
void __arr_set2 (char *pos, Val a, Val ix, Val el) {
Varr *arr = a.a;
int64_t sz = arr->end - arr->begin;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("index %d out of [0,%d]", i, sz - 1));
arr->begin[i] = el;
}
void __arr_setArr (char *pos, Val a, Val ix, Val other) {
__arr_setRange(pos, a, ix, other, (Val)0, __arr_size(other));
}
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
Val __arr_size (Val a) {
return (Val)(a.a->end - a.a->begin);
}
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
Val __arr_sub (Val a, Val begin, Val end) {
if (begin.i < 0) begin = (Val)(__arr_size(a).i + begin.i);
if (end.i < 0) end = (Val)(__arr_size(a).i + end.i);
return __arr_drop(__arr_take(a, end), begin);
}
Val __arr_sub0 (Val a, Val begin) {
if (begin.i < 0) begin = (Val)(__arr_size(a).i + begin.i);
return __arr_drop(a, begin);
}
Val __arr_take (Val a, Val n) {
if (n.i > __arr_size(a).i) return __arr_copy(a);
Val r = __arr_new();
Val *p = a.a->begin;
for (int64_t i = 0; i < n.i; ++i) __arr_push(r, *p++);
return r;
}
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
Val __arr_toIter (Val a) {
return __iter_new(__arr_new_from_carr((Val)2, (Val[]){
__arr_new_from_carr((Val)3, (Val[]){a, __arr_size(a), (Val)0}),
(Val)(void *)__arr_toIter_next
}));
}
Val __arr_toJs (Val a, Val fn) {
return __js_wa(__arr_map(a, fn));
}
Arr *__arr_to_sarr (Val a) {
Arr *r = arr_new();
Varr *va = a.a;
Val *p = va->begin;
Val *end = va->end;
while (p < end) arr_push(r, (*p++).s);
return r;
}
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


// -------------------------------------
//b64
// -------------------------------------

Val __b64_decode (char *pos, Val b64) {
char *r;
TRY {
r = b64_decode(b64.s);
} CATCH(e) {
___built_throw(pos, (Val)exc_msg(e));
}_TRY
return (Val) r;
}
Val __b64_decodeBytes (char *pos, Val b64) {
Bytes *r;
TRY {
r = b64_decode_bytes(b64.s);
} CATCH(e) {
___built_throw(pos, (Val)exc_msg(e));
}_TRY
return (Val) (void *)r;
}
Val __b64_encode (Val s) {
return (Val)b64_encode(s.s);
}
Val __b64_encodeBytes (Val bs) {
return (Val)b64_encode_bytes(bs.o);
}


// -------------------------------------
//bytes
// -------------------------------------

Val __bytes_add (Val bs1, Val bs2) {
bytes_add((Bytes *)bs1.o, (Bytes *)bs2.o);
return bs1;
}
Val __bytes_drop (Val bts, Val begin) {
Bytes *bs = (Bytes *)bts.o;
int64_t ix = begin.i;
int64_t sz = bytes_len(bs);
Bytes *bs2 = sz > ix
? ix <= 0
? bytes_from_bytes(bytes_bs(bs), sz)
: bytes_from_bytes(bytes_bs(bs) + ix, sz - ix)
: bytes_new()
;
return (Val)(void *)bs2;
}
Val __bytes_fromArr (Val a) {
int64_t sz = __arr_size(a).i;
Bytes *bs = bytes_new_bf(sz);
unsigned char *pu = bytes_bs(bs);
Val *p = a.a->begin;
Val *end = a.a->end;
while (p < end) *pu++ = (unsigned char)(*p++).i;
return (Val)(void *)bs;
}
Val __bytes_fromStr (Val str) {
return (Val)(void *)bytes_from_str(str.s);
}
Val __bytes_get (char *pos, Val bs, Val ix) {
int64_t sz = __bytes_size(bs).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
return (Val)(int64_t)(bytes_bs((Bytes *)bs.o)[ix.i]);
}
Val __bytes_new (Val size) {
int64_t bf = size.i;
if (bf < 0) bf = 0;
Bytes *bs = bytes_new_bf(bf);
unsigned char *p = bytes_bs(bs);
while (bf) {
*p++ = 0;
--bf;
}
return (Val)(void *)bs;
}
void __bytes_set (char *pos, Val bs, Val ix, Val byte) {
int64_t sz = __bytes_size(bs).i;
int64_t i = ix.i;
if (i < 0 || i >= sz)
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
bytes_bs((Bytes *)bs.o)[ix.i] = (unsigned char)byte.i;
}
Val __bytes_size (Val bs) {
return (Val)(int64_t)bytes_len((Bytes *)bs.o);
}
Val __bytes_take (Val bts, Val end) {
Bytes *bs = (Bytes *)bts.o;
int64_t ix = end.i;
int64_t sz = bytes_len(bs);
Bytes *bs2 = ix <= 0
? bytes_new()
: ix >= sz
? bytes_from_bytes(bytes_bs(bs), sz)
: bytes_from_bytes(bytes_bs(bs), ix)
;
return (Val)(void *)bs2;
}
Val __bytes_toArr (Val bts) {
Bytes *bs = (Bytes *)bts.o;
int sz = bytes_len(bs);
Val a = __arr_new();
unsigned char *p = bytes_bs(bs);
while (sz) {
__arr_push(a, (Val)(int64_t)*p++);
--sz;
}
return a;
}
Val __bytes_toStr (Val bs) {
return (Val)bytes_to_str((Bytes *)bs.o);
}


// -------------------------------------
//cryp
// -------------------------------------

Val __cryp_decode (Val k, Val c) {
return (Val) cryp_decode(k.s, c.s);
}
Val __cryp_decodeBytes (Val k, Val c) {
return (Val)(void *)cryp_decode_bytes(k.s, c.s);
}
Val __cryp_encode (Val k, Val tx) {
return (Val)cryp_encode(k.s, tx.s);
}
Val __cryp_encodeBytes (Val k, Val bs) {
return (Val)cryp_encode_bytes(k.s, bs.o);
}
Val __cryp_genK (Val lg) {
return (Val)cryp_genk(lg.i);
}
Val __cryp_key (Val s, Val lg) {
return (Val)cryp_key(s.s, lg.i);
}


// -------------------------------------
//dic
// -------------------------------------

Val __dic_copy (Val d) {
Val *p = d.a->begin;
Val *end = d.a->end;
int64_t bf_size = end - p;
Varr *this = MALLOC(Varr);
this->begin = GC_MALLOC(bf_size * sizeof(Val));
this->end = this->begin + bf_size;
this->end_bf = this->end;
Val *pt = this->begin;
while (p < end) *pt++ = __arr_copy(*p++);
return (Val) this;
}
Val __dic_fromArr (Val a) {
Val r = __arr_new();
Varr *arr = a.a;
Val *p = arr->begin;
Val *end = arr->end;
while (p < end) {
Varr *kv = (*p++).a;
__dic_put(r, *(kv->begin), kv->begin[1]);
}
return r;
}
Val __dic_fromIter (Val it) {
return __dic_fromArr(__arr_fromIter(it));
}
Val __dic_fromJs (Val s, Val fn) {
Val d = __js_ro(s);
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
kv->begin[1] = ((Val (*)(Val))fn.o)(kv->begin[1]);
}
return d;
}
Val __dic_from_smap (Map *m) {
Val r = __arr_new();
EACH(m, Kv, el) {
Val e = __arr_new();
__arr_push(e, (Val)kv_key(el));
__arr_push(e, (Val)((char *)kv_value(el)));
__arr_push(r, e);
}_EACH
return r;
}
Val __dic_get(Val d, Val key) {
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
if (!strcmp((*(kv->begin)).s, key.s)) return __rs_some(kv->begin[1]);
}
return __arr_new();
}
Val __dic_get2(char *pos, Val d, Val key) {
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
if (!strcmp((*(kv->begin)).s, key.s)) return kv->begin[1];
}
___built_throw(pos, (Val)str_f("Key %s not found", key.s));
return (Val)0; // Unreachable
}
Val __dic_hasKey(Val d, Val key) {
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
if (!strcmp((*(kv->begin)).s, key.s)) return (Val)1;
}
return (Val)0;
}
Val __dic_keys (Val d) {
Val r = __arr_new();
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
__arr_push(r, *(kv->begin));
}
return r;
}
Val __dic_map (Val d, Val fn) {
Val dr = __dic_copy(d);
Varr *r = dr.a;
Val *p = r->begin;
Val *end = r->end;
while (p < end) {
Varr *kv = (*p++).a;
kv->begin[1] = ((Val (*)(Val))fn.o)(kv->begin[1]);
}
return dr;
}
Val __dic_new_from_carr (Val n, Val *els) {
Val r = __arr_new();
REPEAT(n.i) {
Varr *kv = (*els++).a;
__dic_put(r, *(kv->begin), kv->begin[1]);
}_REPEAT
return r;
}
void __dic_put(Val d, Val key, Val value) {
int new = TRUE;
char *k = key.s;
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
if (!strcmp(k, (*(kv->begin)).s)) {
kv->begin[1] = value;
new = FALSE;
break;
}
}
if (new) __arr_push(d, __arr_new_from_carr((Val)2, (Val[]){key, value}));
}
void __dic_remove(Val d, Val key) {
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
Val *target = NULL;
while (p < end) {
if (target) {
*target++ = *p++;
} else {
Val *t = p++;
Varr *kv = t->a;
if (!strcmp((*(kv->begin)).s, key.s)) target = t;
}
}
if (target) --(a->end);
}
void __dic_set(char *pos, Val d, Val key, Val value) {
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
if (!strcmp((*(kv->begin)).s, key.s)) {
kv->begin[1] = value;
return;
}
}
___built_throw(pos, (Val)str_f("Key %s not found", key.s));
}
Val __dic_size(Val d) {
return __arr_size(d);
}
Val __dic_toArr (Val d) {
return __dic_copy(d);
}
Val __dic_toIter (Val d) {
return __arr_toIter(d);
}
Val __dic_toJs (Val d, Val fn) {
Map *r = map_new();
Varr *va = d.a;
Val *p = va->begin;
Val *end = va->end;
while (p < end) {
Varr *kv = (*p++).a;
arr_push((Arr *)r, kv_new(
(*(kv->begin)).s,
((Val (*)(Val))fn.o)(kv->begin[1]).s
));
}
return __js_wo(__dic_from_smap(r));
}
Map *__dic_to_smap (Val a) {
Map *r = map_new();
Varr *va = a.a;
Val *p = va->begin;
Val *end = va->end;
while (p < end) {
Varr *kv = (*p++).a;
arr_push((Arr *)r, kv_new((*(kv->begin)).s, (kv->begin[1]).s));
}
return r;
}
Val __dic_values (Val d){
Val r = __arr_new();
Varr *a = d.a;
Val *p = a->begin;
Val *end = a->end;
while (p < end) {
Varr *kv = (*p++).a;
__arr_push(r, kv->begin[1]);
}
return r;
}


// -------------------------------------
//file
// -------------------------------------

Val __file_aopen (Val path) {
return (Val)(void *)file_aopen(path.s);
}
Val __file_base (Val path) {
return (Val)path_base (path.s);
}
Val __file_canonical (char *pos, Val path) {
char *r = opt_get(path_canonical(path.s));
if (!r)
___built_throw(pos, (Val)(str_f("'%s' can not be canonicalized", path.s)));
return (Val)r;
}
Val __file_cat (Val path, Val rpaths) {
__arr_unshift(rpaths, *(path.s) ? path : (Val)".");
return __file_clean(__arr_join(rpaths, (Val)"/"));
}
void __file_cd (Val path) {
file_cd(path.s);
}
Val __file_clean (Val path) {
return (Val)path_clean(path.s);
}
void __file_close (Val file) {
file_close(file.o);
}
void __file_copy (char *pos, Val source, Val target) {
void copy (char *source, char *target) {
if (file_is_directory(source)) {
if (file_exists(target) && !file_is_directory(target))
___built_throw(pos, (Val)str_f("'%s' is not a directory", target));
char *tdir = path_cat(target, path_base(source), NULL);
file_del(tdir);
file_mkdir(tdir);
EACH(file_dir(source), char, fname) {
copy(path_cat(source, fname, NULL), tdir);
}_EACH
return;
}
if (file_is_directory(target))
target = path_cat(target, path_base(source), NULL);
file_copy(source, target);
}
return copy(source.s, target.s);
}
void __file_del (Val path) {
file_del(path.s);
}
Val __file_dir (Val path) {
Val r = __arr_new();
EACH(file_dir(path.s), char, name) {
__arr_push(r, (Val)name);
}_EACH
return r;
}
Val __file_exists (Val path) {
return (Val)file_exists(path.s);
}
Val __file_extension (Val path) {
return (Val)path_extension(path.s);
}
Val __file_isDirectory (Val path) {
return (Val)file_is_directory(path.s);
}
Val __file_isLink (Val path) {
return (Val)file_is_link(path.s);
}
Val __file_isRegular (Val path) {
return (Val)file_is_regular(path.s);
}
void __file_mkdir (Val path) {
file_mkdir(path.s);
}
void __file_mklink (Val path, Val link) {
file_link(path.s, link.s);
}
Val __file_parent (Val path) {
return (Val)path_parent(path.s);
}
Val __file_read (Val path) {
return (Val)file_read(path.s);
}
Val __file_readBin (Val file, Val buf) {
return (Val)(void *)file_read_bin_buf(file.o, buf.i);
}
Val __file_readLine (Val file) {
char *l = file_read_line(file.o);
if (*l) {
if (l[strlen(l) - 1] == '\n') l = str_left(l, -1);
return __rs_some((Val)l);
}
return __arr_new();
}
void __file_rename (Val old_path, Val new_path) {
file_rename(old_path.s, new_path.s);
}
Val __file_ropen (Val path) {
return (Val)(void *)file_ropen(path.s);
}
Val __file_size (Val path) {
return (Val)(int64_t)file_size(path.s);
}
Val __file_tm (Val path) {
return (Val)file_modified(path.s);
}
Val __file_tmp (Val dir, Val name) {
return (Val)file_tmp(dir.s, name.s);
}
Val __file_wd (void) {
return (Val)file_wd();
}
Val __file_wopen (Val path) {
return (Val)(void *)file_wopen(path.s);
}
void __file_write (Val path, Val tx) {
file_write(path.s, tx.s);
}
void __file_writeBin (Val file, Val bs) {
file_write_bin(file.o, bs.o);
}
void __file_writeText (Val file, Val tx) {
file_write_text(file.o, tx.s);
}


// -------------------------------------
//iter
// -------------------------------------

Val __iter_all (Val it, Val fn) {
int r = TRUE;
while (__iter_hasNext(it).b) {
Val v = __iter_next(it);
r = r && (((Val (*)(Val))fn.o)(v)).b;
}
return (Val)r;
}
Val __iter_any (Val it, Val fn) {
int r = FALSE;
while (__iter_hasNext(it).b) {
Val v = __iter_next(it);
r = r || (((Val (*)(Val))fn.o)(v)).b;
}
return (Val)r;
}
Val __iter_cat_next (Val obj) {
Val *p = obj.a->begin;
if (p->b) {
Val it = *(p+1);
if (__iter_hasNext(it).b) return __rs_some(__iter_next(it));
*p = (Val) FALSE;
}
Val it = *(p+2);
return (__iter_hasNext(it).b) ? __rs_some(__iter_next(it)) : __arr_new();
}
Val __iter_cat (Val it1, Val it2) {
Val obj = __arr_new_from_carr((Val)3, (Val[]){(Val)TRUE, it1, it2});
return __iter_new_aux(obj, __iter_cat_next);
}
Val __iter_count (Val it) {
int64_t c = 0;
while (__iter_hasNext(it).b) {
__iter_next(it);
++c;
}
return (Val)c;
}
Val __iter_drop (Val it, Val n) {
int64_t nv = n.i;
int64_t c = 0;
for(;;) {
if (c >= nv || !__iter_hasNext(it).b) break;
++c;
__iter_next(it);
}
return it;
}
Val __iter_dropWhile (Val it, Val fn) {
while (__iter_hasNext(it).b && (((Val (*)(Val))fn.o)(__iter_peek(it))).b)
__iter_next(it);
return it;
}
void __iter_each (Val it, Val fn) {
while (__iter_hasNext(it).b)
((void (*)(Val))fn.o)(__iter_next(it));
}
void __iter_eachIx (Val it, Val fn) {
int64_t c = 0;
while (__iter_hasNext(it).b)
((void (*)(Val, Val))fn.o)(__iter_next(it), (Val)c++);
}
Val __iter_empty_next (Val fake) {
return __arr_new();
}
Val __iter_empty (Val fake) {
return __iter_new_aux(fake, __iter_empty_next);
}
Val __iter_filter_next (Val obj) {
Val *p = obj.a->begin;
Val it = *p;
Val fn = *(p+1);
while (__iter_hasNext(it).b) {
Val r = __iter_next(it);
if ((((Val (*)(Val))fn.o)(r)).b) return __rs_some(r);
}
return __arr_new();
}
Val __iter_filter (Val it, Val fn) {
return __iter_new_aux(
__arr_new_from_carr((Val)2, (Val[]){it, fn}),
__iter_filter_next
);
}
Val __iter_find (Val it, Val fn) {
while (__iter_hasNext(it).b) {
Val r = __iter_next(it);
if ((((Val (*)(Val))fn.o)(r)).b) return __rs_some(r);
}
return __arr_new();
}
Val __iter_fromJs (Val s, Val fn) {
return __iter_map(__arr_toIter(__js_ra(s)), fn);
}
Val __iter_hasNext (Val it) {
return (Val)(__arr_size(*(it.a->begin)).i > 0);
}
Val __iter_index (Val it, Val fn) {
int64_t c = 0;
while (__iter_hasNext(it).b) {
Val e = __iter_next(it);
int64_t r = c++;
if ((((Val (*)(Val))fn.o)(e)).b) return (Val)r;
}
return (Val)(int64_t)-1;
}
Val __iter_map_next (Val obj) {
Val *p = obj.a->begin;
Val it = *p;
Val fn = *(p+1);
while (__iter_hasNext(it).b)
return __rs_some(((Val (*)(Val))fn.o)(__iter_next(it)));
return __arr_new();
}
Val __iter_map (Val it, Val fn) {
return __iter_new_aux(
__arr_new_from_carr((Val)2, (Val[]){it, fn}),
__iter_map_next
);
}
Val __iter_next (Val it) {
Val r = *(it.a->begin);
if (__arr_size(r).i == 0)
___built_throw("Built-in:0", (Val)"Iterator is empty");
Val tuple = *(it.a->begin + 1);
Val *p = tuple.a->begin;
*(it.a->begin) = ((Val (*)(Val))(*(p+1)).o)(*p);
return *r.a->begin;
}
Val __iter_new (Val tuple) {
Val *p = tuple.a->begin;
Val a = ((Val (*)(Val))(*(p+1)).o)(*p);
return __arr_new_from_carr((Val)2, (Val[]){a, tuple});
}
Val __iter_new_aux (Val obj, Val (*fn) (Val)) {
return __iter_new(__arr_new_from_carr((Val)2, (Val[]){
obj, (Val)(void *)fn
}));
}
Val __iter_peek (Val it) {
Val r = *(it.a->begin);
if (__arr_size(r).i == 0)
___built_throw("Built-in:0", (Val)"Iterator is empty");
return *r.a->begin;
}
Val __iter_range_next (Val obj) {
Val *p = obj.a->begin;
int64_t begin = p->i;
int64_t end = (p+1)->i;
int withStep = (p+3)->i;
if (withStep) {
int64_t step = (p+2)->i;
if (step > 0) {
if (begin <= end) {
*p = (Val)(begin + step);
return __arr_new_from_carr((Val)1, (Val[]){(Val)begin});
}
} else {
if (begin >= end) {
*p = (Val)(begin + step);
return __arr_new_from_carr((Val)1, (Val[]){(Val)begin});
}
}
} else {
if (begin < end) {
*p = (Val)(begin + 1);
return __arr_new_from_carr((Val)1, (Val[]){(Val)begin});
}
}
return __arr_new();
}
Val __iter_range (char *pos, Val vs) {
Val obj = __arr_new();
Val *p = vs.a->begin;
int64_t sz = __arr_size(vs).i;
if (sz == 1) {
__arr_push(obj, (Val) 0);
__arr_push(obj, *p);
__arr_push(obj, (Val) 1);
__arr_push(obj, (Val) 0);
} else if (sz == 2) {
__arr_push(obj, *p++);
__arr_push(obj, *p);
__arr_push(obj, (Val) 1);
__arr_push(obj, (Val) 0);
} else {
__arr_push(obj, *p++);
__arr_push(obj, *p++);
if (p->i == 0)
___built_throw(pos, (Val)"Range step is 0");
__arr_push(obj, *p);
__arr_push(obj, (Val) 1);
}
return __iter_new_aux(obj, __iter_range_next);
}
Val __iter_reduce (Val it, Val seed, Val fn) {
while (__iter_hasNext(it).b)
seed = ((Val (*)(Val, Val))fn.o)(seed, __iter_next(it));
return seed;
}
Val __iter_take_next (Val obj) {
Val *p = obj.a->begin;
Val it = *p;
int64_t n = (p+1)->i;
int64_t c = (p+2)->i;
if (c < n) {
if (__iter_hasNext(it).b) {
*(p+2) = (Val)(c + 1);
return __rs_some(__iter_next(it));
}
*(p+2) = *(p+1);
}
return __arr_new();
}
Val __iter_take (Val it, Val n){
return __iter_new_aux(
__arr_new_from_carr((Val)3, (Val[]){it, n, (Val)0}),
__iter_take_next
);
}
Val __iter_takeWhile_next (Val obj) {
Val *p = obj.a->begin;
Val it = *p;
Val fn = *(p+1);
return __iter_hasNext(it).b && (((Val (*)(Val))fn.o)(__iter_peek(it))).b
? __rs_some(__iter_next(it))
: __arr_new()
;
}
Val __iter_takeWhile (Val it, Val fn) {
return __iter_new_aux(
__arr_new_from_carr((Val)2, (Val[]){it, fn}),
__iter_takeWhile_next
);
}
Val __iter_toJs (Val it, Val fn) {
return __js_wa(__arr_fromIter(__iter_map(it, fn)));
}
Val __iter_unary_next (Val obj) {
if (__arr_size(obj).i) {
Val r = __arr_copy(obj);
__arr_clear(obj);
return r;
}
return __arr_new();
}
Val __iter_unary (Val value) {
return __iter_new_aux(__rs_some(value), __iter_unary_next);
}


// -------------------------------------
//js
// -------------------------------------

Val __js_isNull (Val json) {
return (Val)js_is_null(json.s);
}
Val __js_ra (Val json) {
return __arr_from_sarr(js_ra(json.s));
}
Val __js_rb (Val json) {
return (Val)js_rb(json.s);
}
Val __js_rf (Val json) {
return (Val)js_rd(json.s);
}
Val __js_ri (Val json) {
return (Val)js_rl(json.s);
}
Val __js_ro (Val json) {
return __dic_from_smap(js_ro(json.s));
}
Val __js_rs (Val json) {
return (Val)js_rs(json.s);
}
Val __js_wa(Val a) {
return (Val)js_wa(__arr_to_sarr(a));
}
Val __js_wb(Val value) {
return (Val)js_wb(value.b);
}
Val __js_wf(Val n) {
return (Val)math_ftos(n.f, 9);
}
Val __js_wf2(Val n, Val dec) {
return (Val)math_ftos(n.f, dec.i);
}
Val __js_wi(Val n) {
return (Val)math_itos(n.i);
}
Val __js_wn(void) {
return (Val)js_wn();
}
Val __js_wo(Val m) {
return (Val)js_wo(__dic_to_smap(m));
}
Val __js_ws(Val s) {
return (Val)js_ws(s.s);
}


// -------------------------------------
//math
// -------------------------------------

Val __math_abs (Val n) {
return (Val)math_abs(n.f);
}
Val __math_acos (Val n) {
return (Val)acos(n.f);
}
Val __math_acosh (Val n) {
return (Val)acosh(n.f);
}
Val __math_asin (Val n) {
return (Val)asin(n.f);
}
Val __math_asinh (Val n) {
return (Val)asinh(n.f);
}
Val __math_atan (Val n) {
return (Val)atan(n.f);
}
Val __math_atanh (Val n) {
return (Val)atanh(n.f);
}
Val __math_ceil (Val n) {
return (Val)ceil(n.f);
}
Val __math_cos (Val n) {
return (Val)cos(n.f);
}
Val __math_cosh (Val n) {
return (Val)cosh(n.f);
}
Val __math_e (void) {
return (Val)M_E;
}
Val __math_eq (Val n1, Val n2, Val gap) {
return (Val)math_eq_gap(n1.f, n2.f, gap.f);
}
Val __math_exp (Val n) {
return (Val)exp(n.f);
}
Val __math_exp2 (Val n) {
return (Val)exp2(n.f);
}
Val __math_floor (Val n) {
return (Val)floor(n.f);
}
Val __math_fromEn (Val s) {
return __math_stof((Val)str_replace(s.s, ",", ""));
}
Val __math_fromHex (Val s) {
char *n = str_trim(s.s);
if (!*n) return __arr_new();
char *tmp;
int64_t r = strtol(n, &tmp, 16);
if (*tmp) return __arr_new();
return __rs_some((Val)r);
}
Val __math_fromIso (Val s) {
return __math_stof((Val)str_replace(str_replace(s.s, ".", ""), ",", "."));
}
Val __math_ftoi (Val n) {
return (Val)(int64_t)(n.f);
}
Val __math_ftos (Val n) {
return (Val)math_ftos(n.f, 9);
}
Val __math_ftos2 (Val n, Val dec) {
return (Val)math_ftos(n.f, dec.i);
}
Val __math_isDigits (Val n) {
return (Val)math_digits(n.s);
}
Val __math_itof (Val n) {
return (Val)(double)(n.i);
}
Val __math_itoh (Val n) {
return (Val)str_f("%x", n.i);
}
Val __math_itos (Val n) {
return (Val)math_itos(n.i);
}
Val __math_log (Val n) {
return (Val)log(n.f);
}
Val __math_log10 (Val n) {
return (Val)log10(n.f);
}
Val __math_log2 (Val n) {
return (Val)log2(n.f);
}
Val __math_max (Val n1, Val n2) {
return n1.f >= n2.f ? n1 : n2;
}
Val __math_maxFloat (void) {
return (Val)DBL_MAX;
}
Val __math_maxInt (void) {
return (Val)(int64_t)
(sizeof(int64_t) == sizeof(long long int) ? LLONG_MAX : LONG_MAX);
}
Val __math_min (Val n1, Val n2) {
return n1.f <= n2.f ? n1 : n2;
}
Val __math_minInt (void) {
return (Val)(int64_t)
(sizeof(int64_t) == sizeof(long long int) ? LLONG_MIN : LONG_MIN);
}
Val __math_pi (void) {
return (Val)M_PI;
}
Val __math_pow (Val base, Val exp) {
return (Val)pow(base.f, exp.f);
}
Val __math_pow10 (Val exp) {
int64_t n = exp.i;
double r = 1;
double mul = n > 0 ? 10 : 0.1;
n = n >= 0 ? n : -n;
REPEAT(n) {
r *= mul;
}_REPEAT
return (Val)r;
}
Val __math_rnd (void) {
return (Val)((double)rand() / (double)RAND_MAX);
}
Val __math_rndi (Val top) {
return (Val)(int64_t)(((double)rand() / (double)RAND_MAX) * top.i);
}
Val __math_round (Val n, Val dec) {
return (Val)math_round(n.f, dec.i);
}
Val __math_sin (Val n) {
return (Val)sin(n.f);
}
Val __math_sinh (Val n) {
return (Val)sinh(n.f);
}
Val __math_sqrt (Val n) {
return (Val)sqrt(n.f);
}
Val __math_stof (Val s) {
char *n = str_trim(s.s);
if (!*n) return __arr_new();
struct lconv *lc = localeconv();
int ix = str_cindex(n, '.');
if (ix != -1) n[ix] = *lc->decimal_point;
char *tmp;
double r = strtod(n, &tmp);
if (*tmp) return __arr_new();
return __rs_some((Val)r);
}
Val __math_stoi (Val s) {
char *n = str_trim(s.s);
if (!*n) return __arr_new();
char *tmp;
int64_t r = strtol(n, &tmp, 10);
if (*tmp) return __arr_new();
return __rs_some((Val)r);
}
Val __math_tan (Val n) {
return (Val)tan(n.f);
}
Val __math_tanh (Val n) {
return (Val)tanh(n.f);
}
Val __math_trunc (Val n) {
return (Val)trunc(n.f);
}


// -------------------------------------
//regex
// -------------------------------------

Val __regex_matches (char *pos, Val s, Val rg) {
char *ss = s.s;
Arr *ms = opt_get(regex_matches(rg.s, ss));
if (!ms)
___built_throw(pos, (Val)str_f("Fail compiling regular expression:\n%s", rg));
Val r = __arr_new();
int ix = 0;
EACH(ms, RegexOffset, of) {
int begin = regexOffset_begin(of);
int end = regexOffset_end(of);
__arr_push(r, (Val)str_sub(ss, ix, begin));
__arr_push(r, (Val)str_sub(ss, begin, end));
ix = end;
}_EACH
__arr_push(r, (Val)str_right(ss, ix));
return r;
}
Val __regex_replace (char *pos, Val s, Val rg, Val repl) {
char *r = opt_get(regex_replace(rg.s, s.s, repl.s));
if (r) return (Val)r;
___built_throw(pos, (Val)str_f("Fail of regular expression:\n%s", rg));
return (Val)0; // Unreachable
}
Val __regex_replacef (char *pos, Val s, Val rg, Val frpl) {
char *fn(char *match) {
return (((Val (*)(Val))frpl.o)((Val)match)).s;
}
char *r = opt_get(regex_replacef(rg.s, s.s, fn));
if (r) return (Val)r;
___built_throw(pos, (Val)str_f("Fail of regular expression:\n%s", rg));
return (Val)0; // Unreachable
}


// -------------------------------------
//rs
// -------------------------------------

Val __rs_some (Val v) {
return __arr_new_from_carr((Val) 1, (Val[]){v});
}
Val __rs_ok (Val v) {
return __arr_new_from_carr((Val) 2, (Val[]){__rs_some(v), (Val)""});
}
Val __rs_fail (Val v) {
return __arr_new_from_carr((Val) 2, (Val[]){__arr_new(), v});
}
Val __rs_get (Val rs) {
return *((*rs.a->begin).a->begin);
}
Val __rs_error (Val rs) {
return rs.a->begin[1];
}


// -------------------------------------
//str
// -------------------------------------

Val __str_cmp (Val s1, Val s2) {
return (Val)(int64_t)strcoll(s1.s, s2.s);
}
Val __str_ends (Val str, Val substr) {
return (Val)str_ends(str.s, substr.s);
}
Val __str_fmt (char *pos, Val format, Val args) {
char *fmt = format.s;
Varr *as = args.a;
Arr *pts = arr_new();
Buf *bf = buf_new();
char *p = fmt;
while (*p) {
char ch = *p++;
if (ch == '%') {
ch = *p;
if (ch == 'v') {
++p;
arr_push(pts, buf_str(bf));
bf = buf_new();
continue;
}
if (ch == '%') ++p;
}
buf_cadd(bf, ch);
}
arr_push(pts, buf_str(bf));
if (arr_size(pts) != __arr_size(args).i + 1)
___built_throw(pos, (Val)str_f(
"Wrong arguments number (expected %d, fund %d)",
arr_size(pts) - 1, __arr_size(args).i
));
Arr *rs = arr_new();
void **ps = arr_begin(pts);
arr_push(rs, *ps++);
Val *pas = as->begin;
Val *pend = as->end;
while (pas < pend) {
arr_push(rs, pas++->s);
arr_push(rs, *ps++);
}
return (Val)arr_join(rs, "");
}
Val __str_fromIso (Val s) {
return (Val)str_from_iso(s.s);
}
Val __str_fromRunes (Val a) {
return __arr_join(a, (Val)"");
}
Val __str_fromUtf16 (Val codepoints) {
int64_t sz = __arr_size(codepoints).i;
unsigned *u = ATOMIC(sizeof(unsigned) * (sz + 1));
Val *p = codepoints.a->begin;
Val *end = codepoints.a->end;
unsigned *pu = u;
while (p < end) *pu++ = (unsigned)(*p++).i;
*pu = 0;
return (Val)str_from_unicode(u);
};
Val __str_get(char *pos, Val s, Val ix) {
if (ix.i < 0 || ix.i >= strlen(s.s))
___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", ix.i, strlen(s.s) - 1));
return (Val)str_new_c(s.s[ix.i]);
}
Val __str_index(Val str, Val substr) {
return (Val)(int64_t)str_index(str.s, substr.s);
}
Val __str_indexFrom (Val str, Val substr, Val start) {
return (Val)(int64_t)str_index_from(str.s, substr.s, start.i);
}
Val __str_lastIndex (Val str, Val substr) {
return (Val)(int64_t)str_last_index(str.s, substr.s);
}
Val __str_left (Val str, Val end) {
Val r = __str_toRunes(str);
int64_t sz = __arr_size(r).i;
end = end.i < 0 ? (Val)(sz + end.i) : end;
return __arr_join(__arr_take(r, end), (Val)"");
}
Val __str_left_slice(Val str, Val begin) {
return (Val)str_left(str.s, begin.i);
}
Val __str_len (Val str) {
return (Val)(int64_t)strlen(str.s);
}
Val __str_ltrim (Val str) {
return (Val)str_ltrim(str.s);
}
Val __str_replace (Val s, Val old, Val new) {
return (Val)str_replace(s.s, old.s, new.s);
}
Val __str_right (Val str, Val begin) {
Val r = __str_toRunes(str);
int64_t sz = __arr_size(r).i;
begin = begin.i < 0 ? (Val)(sz + begin.i) : begin;
return __arr_join(__arr_drop(r, begin), (Val)"");
}
Val __str_right_slice(Val str, Val begin) {
return (Val)str_right(str.s, begin.i);
}
Val __str_rtrim (Val str) {
return (Val)str_rtrim(str.s);
}
Val __str_split (Val str, Val sep) {
char *s = str.s;
char *ssep = sep.s;
Val r = __arr_new();
int len = strlen(ssep);
if (!len) {
char *rune;
s = str_next_rune(&rune, s);
while (*rune) {
__arr_push(r, (Val)rune);
s = str_next_rune(&rune, s);
}
return r;
}
if (len == 1) {
char csep = *ssep;
int i = str_cindex(s, csep);
while (i != -1) {
char *sub = ATOMIC(i + 1);
sub[i] = 0;
memcpy(sub, s, i);
__arr_push(r, (Val)sub);
s = s + i + len;
i = str_cindex(s, csep);
}
__arr_push(r, (Val)str_new(s));
return r;
}
int i = str_index(s, ssep);
while (i != -1) {
char *sub = ATOMIC(i + 1);
sub[i] = 0;
memcpy(sub, s, i);
__arr_push(r, (Val)sub);
s = s + i + len;
i = str_index(s, ssep);
}
__arr_push(r, (Val)str_new(s));
return r;
}
Val __str_splitTrim (Val str, Val sep) {
Val r = __str_split(str, sep);
Val *p = r.a->begin;
Val *end = r.a->end;
while (p < end) {
*p = __str_trim(*p);
++p;
}
return r;
}
Val __str_starts(Val str, Val substr) {
return (Val)str_starts(str.s, substr.s);
}
Val __str_sub (Val str, Val begin, Val end) {
Val r = __str_toRunes(str);
int64_t sz = __arr_size(r).i;
begin = begin.i < 0 ? (Val)(sz + begin.i) : begin;
end = end.i < 0 ? (Val)(sz + end.i) : end;
return __arr_join(__arr_drop(__arr_take(r, end), begin), (Val)"");
}
Val __str_sub_slice(Val str, Val begin, Val end) {
return (Val)str_sub(str.s, begin.i, end.i);
}
Val __str_toLower(Val s) {
return (Val)str_to_lower(s.s);
}
Val __str_toUpper(Val s) {
return (Val)str_to_upper(s.s);
}
Val __str_toRunes (Val str) {
char *s = str.s;
Val r = __arr_new();
char *rune;
s = str_next_rune(&rune, s);
while (*rune) {
__arr_push(r, (Val)rune);
s = str_next_rune(&rune, s);
}
return r;
}
Val __str_toUtf16 (Val str) {
unsigned *u = str_to_unicode(str.s);
Val a = __arr_new();
unsigned *pu = u;
while (*pu) __arr_push(a, (Val)(int64_t)*pu++);
return a;
}
Val __str_trim (Val str) {
return (Val)str_trim(str.s);
}


// -------------------------------------
//sys
// -------------------------------------

static Val __sys_compiler_root_v = (Val) "";
static Val __sys_mainPath_v = (Val) "";
static Val __sys_args_v = (Val) "";
static void __sys_shandler (int sig) {
___built_throw("built-in:0", (Val)"Arithmetic exception");
}
static void __sys_sshandler (int sig) {
___built_throw("built-in:0", (Val)"Segmentation violation");
}
void __sys_init(int argc, char **argv, char *compiler_root) {
sys_init();
signal (SIGFPE, __sys_shandler);
signal (SIGSEGV, __sys_sshandler);
__sys_compiler_root_v = (Val)compiler_root;
__sys_mainPath_v = (Val)*argv;
__sys_args_v = __arr_new();
for (int i = 1; i < argc; ++i) __arr_push(__sys_args_v, (Val)argv[i]);
}
Val __sys_args (void) {
return __arr_copy(__sys_args_v);
}
Val __sys_cmd (Val command, Val args) {
Val rs = __sys_cmd2(command, args);
Val *ok = rs.a->begin;
Val *err = ok + 1;
Val r = __arr_new();
if (*(err->s)) __arr_push(r, __arr_new());
else __arr_push(r, __rs_some(*ok));
__arr_push(r, *err);
return r;
}
Val __sys_cmd2 (Val command, Val args) {
Arr *acmd = arr_new();
arr_push(acmd, command.s);
Val *p = args.a->begin;
Val *end = args.a->end;
while (p < end) arr_push(acmd, str_to_escape((*p++).s));
Tp *tp = sys_cmd2(arr_cjoin(acmd, ' '));
Val r = __arr_new();
__arr_push(r, (Val)tp->e1);
__arr_push(r, (Val)tp->e2);
return r;
}
Val __sys_compilerRoot (void) {
return __sys_compiler_root_v;
}
Val __sys_environ (void) {
return __dic_from_smap(sys_environ());
}
void __sys_exit (Val code) {
exit(code.i);
}
Val __sys_exec (char *pos, Val command, Val args) {
Arr *acmd = arr_new();
arr_push(acmd, command.s);
Val *p = args.a->begin;
Val *end = args.a->end;
while (p < end) arr_push(acmd, str_to_escape((*p++).s));
return (Val)(int64_t)system(arr_cjoin(acmd, ' '));
}
Val __sys_getLocale (void) {
return (Val)sys_get_locale();
}
Val __sys_home (void) {
return (Val)sys_user_home();
}
Val __sys_mainPath (void) {
return __sys_mainPath_v;
}
Val __sys_pid (void) {
return (Val)getpid();
}
void __sys_printError (Val text) {
fprintf(stderr, "%s", text.s);
}
Val __sys_readLine (void) {
return (Val)sys_read_line();
}
void __sys_setLocale (Val charset) {
return sys_set_locale(charset.s);
}
void __sys_sleep (Val millis) {
sys_sleep(millis.i);
}
Val __sys_user (void) {
return (Val)sys_user_name();
}


// -------------------------------------
//tcp
// -------------------------------------

Val __tcp_accept (Val server) {
Rs *rs = tcp_accept(server.o);
void *conn = rs_get(rs);
if (!conn) return __rs_fail((Val)rs_error(rs));
return __rs_ok((Val)conn);
}
void __tcp_closeConnection (Val conn) {
tcp_close_conn(conn.o);
}
void __tcp_closeServer (Val server) {
tcp_close_server(server.o);
}
Val __tcp_dial (Val server_name) {
char *sv = server_name.s;
int ok = TRUE;
Arr *ps = str_csplit(sv, ':');
if (arr_size(ps) != 2) ok = FALSE;
if (ok && !math_digits(arr_get(ps, 1))) ok = FALSE;
if (!ok)
return __rs_fail((Val)str_f("Bad sever:port (%s)", sv));
Rs *rs = tcp_dial(arr_get(ps, 0), math_stoi(arr_get(ps, 1)));
void *conn = rs_get(rs);
if (!conn) return __rs_fail((Val)rs_error(rs));
return __rs_ok((Val)conn);
}
Val __tcp_read (Val conn, Val lim, Val seconds) {
Rs *rs = tcp_read(conn.o, lim.i, seconds.i);
void *bs = rs_get(rs);
if (!bs) return __rs_fail((Val)rs_error(rs));
return __rs_ok((Val)bs);
}
Val __tcp_server (Val port, Val nconns) {
return (Val)(void *)tcp_server(port.i, nconns.i);
}
Val __tcp_write (Val conn, Val data) {
return (Val)tcp_write(conn.o, data.o);
}


// -------------------------------------
//thread
// -------------------------------------

void __thread_join (Val th) {
thread_join(th.o);
}
void __thread_run (Val fn) {
thread_run ((void (*)(void))fn.o);
}
Val __thread_start (Val fn) {
return (Val)(void *)thread_start((void (*)(void))fn.o);
}
void __thread_sync (Val fn) {
thread_sync ((void (*)(void))fn.o);
}


// -------------------------------------
//time
// -------------------------------------

Val __time_addDays (Val tm, Val days) {
return (Val)time_add(tm.i, (int)days.i);
}
Val __time_day (Val tm) {
return (Val)(int64_t)time_day(tm.i);
}
Val __time_dfDays (Val tm1, Val tm2) {
return (Val)(int64_t)time_df(tm1.i, tm2.i);
}
Val __time_eqDay (Val tm1, Val tm2) {
return (Val)time_eq(tm1.i, tm2.i);
}
Val __time_fmt (Val tm_val, Val template) {
char *time_fmt (int64_t tm, char *template) {
char *s, *rs;
time_t tthis = (time_t)(tm / 1000);
struct tm *t = localtime(&tthis);
int size = 126;
while (1) {
rs = (char *)calloc(size, 1); // calloc ok
if (strftime (rs, size, template, t)) {
s = str_new(rs);
free(rs); // free ok
break;
}
free(rs); // free ok
size += size;
}
return s;
}
int64_t tm = tm_val.i;
char *s = template.s;
int len = strlen(s);
int p = 0;
Buf *r = buf_new();
for (;;) {
int i = str_cindex_from(s, '%', p);
if (i == -1) break;
buf_add(r, str_sub(s, p, i));
++i;
if (i == len)
___built_throw(
"built-in(time.fmt):0",
(Val)"Bad '%' sequence at end of string"
);
char ch = s[i];
if (ch == '%') {
buf_cadd(r, ch);
p = i + 1;
continue;
}
if (
ch == 'd' || ch == 'D' || ch == 'm' || ch == 'M' || ch == 'y' ||
ch == 'Y' || ch == 't' || ch == 'T'
) {
switch (ch) {
case 'd' :
buf_add(r, str_ltrim(time_fmt(tm, "%e")));
break;
case 'D':
buf_add(r, time_fmt(tm, "%d"));
break;
case 'm': {
char *v = time_fmt(tm, "%m");
if (*v == '0') v = v + 1;
buf_add(r, v);
break;
}
case 'M':
buf_add(r, time_fmt(tm, "%m"));
break;
case 'y':
buf_add(r, time_fmt(tm, "%y"));
break;
case 'Y':
buf_add(r, time_fmt(tm, "%Y"));
break;
case 't':
buf_add(r, time_fmt(tm, "%T"));
break;
default: { // 'T'
char *millis = str_f("%ld", tm % 1000);
buf_add(r, str_f("%s.%s", time_fmt(tm, "%T"), millis));
}
}
p = i + 1;
continue;
}
___built_throw("built-in(time.fmt):0", (Val)"Bad '%' sequence");
}
buf_add(r, str_right(s, p));
return (Val)str_new(buf_str(r));
}
Val __time_fromClock (Val tm, Val template) {
int to_time_number (char *n, int max) {
if (strlen(n) != 2) return -1;
if (!math_digits(n)) return -1;
int nn = math_stoi(n);
if (nn >= max) return -1;
return nn;
}
char *tms = template.s;
Arr *ps = str_csplit(tms, ':');
if (arr_size(ps) == 3) {
int h = to_time_number(arr_get(ps, 0), 24);
if (h != 1) {
int m = to_time_number(arr_get(ps, 1), 60);
if (m != -1) {
int s = to_time_number(arr_get(ps, 2), 60);
if (s != -1) {
return __rs_some(__time_new(
__time_day(tm), __time_month(tm), __time_year(tm),
(Val)(int64_t)h, (Val)(int64_t)m, (Val)(int64_t)s
));
}
}
}
}
return __arr_new();
}
Val __time_from_str_op_aux(char *date) {
if (strlen(date) != 8 && !math_digits(date)) return __arr_new();
char tpl[5];
memset (tpl, 0, 5);
memcpy(tpl, date + 6, 2);
int d = atoi(tpl);
memcpy(tpl, date + 4, 2);
int m = atoi(tpl);
memcpy(tpl, date, 4);
int y = atoi(tpl);
return __rs_some(__time_newDate(
(Val)(int64_t)d, (Val)(int64_t)m, (Val)(int64_t)y
));
}
Val __time_from_iso_en_op (int is_iso, char *date, char sep) {
Arr *parts = str_csplit(date, sep);
if (arr_size(parts) == 3) {
char *d = arr_get(parts, (is_iso) ? 0 : 1);
char *m = arr_get(parts, (is_iso) ? 1 : 0);
char *y = arr_get(parts, 2);
if (strlen(d) == 1) d = str_f("%s%s", "0", d);
if (strlen(m) == 1) m = str_f("%s%s", "0", m);
if (strlen(y) == 2) y = str_f("%s%s", "20", y);
if (strlen(d) == 2 && strlen(m) == 2 && strlen(y) == 4)
return __time_from_str_op_aux(str_f("%s%s%s", y, m, d));
}
return __arr_new();
}
Val __time_fromEn (Val s, Val sep) {
return __time_from_iso_en_op(FALSE, s.s, *sep.s);
}
Val __time_fromIso (Val s, Val sep) {
return __time_from_iso_en_op(TRUE, s.s, *sep.s);
}
Val __time_fromStr (Val s) {
return __time_from_str_op_aux(s.s);
}
Val __time_hour (Val tm) {
return (Val)(int64_t)time_hour(tm.i);
}
Val __time_millisecond (Val tm) {
return (Val)(int64_t)time_millisecond(tm.i);
}
Val __time_minute (Val tm) {
return (Val)(int64_t)time_minute(tm.i);
}
Val __time_month (Val tm) {
return (Val)(int64_t)(time_month(tm.i) + 1);
}
Val __time_new (Val day, Val month, Val year, Val hour, Val minute, Val second) {
return (Val)time_new_time(
(int)day.i, (int)(month.i - 1), (int)year.i,
(int)hour.i, (int)minute.i, (int)second.i
);
}
Val __time_newDate (Val day, Val month, Val year) {
return (Val)time_new((int)day.i, (int)(month.i - 1), (int)year.i);
}
Val __time_now (void) {
return (Val)time_now();
}
Val __time_second (Val tm) {
return (Val)(int64_t)time_second(tm.i);
}
Val __time_toEn (Val tm) {
return (Val)time_to_us(tm.i);
}
Val __time_toIso (Val tm) {
return (Val)time_to_iso(tm.i);
}
Val __time_toStr (Val tm) {
return (Val)time_to_str(tm.i);
}
Val __time_weekday (Val tm) {
return (Val)(int64_t)time_week_day(tm.i);
}
Val __time_year (Val tm) {
return (Val)(int64_t)time_year(tm.i);
}
Val __time_yearDay (Val tm) {
return (Val)(int64_t)(time_year_day(tm.i) + 1);
}

