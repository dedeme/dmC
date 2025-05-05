
// dic ---------------------------------

// {A} -> {A}
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

// [[.sA]] -> {A}
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

// [[.sA]] -> {A}
Val __dic_fromIter (Val it) {
  return __dic_fromArr(__arr_fromIter(it));
}

// s,(s->A) -> {A}
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

// Auxiliar of __js_ro
// Map<char>->{s}
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

// {A},s -> [A]
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

// s,{A},s -> A
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

// {A},s -> b
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

// {A}->[s]
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

// {A}A->B -> {B}
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

// i,*[[s,A]]->{A}
Val __dic_new_from_carr (Val n, Val *els) {
  Val r = __arr_new();
  REPEAT(n.i) {
    Varr *kv = (*els++).a;
    __dic_put(r, *(kv->begin), kv->begin[1]);
  }_REPEAT
  return r;
}

// {A},s,A->()
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

// {A},s -> ()
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

// s,{A},s, A -> ()
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

// {A} -> i
Val __dic_size(Val d) {
  return __arr_size(d);
}

// {A}->[[. sA]]
Val __dic_toArr (Val d) {
  return __dic_copy(d);
}

// {A}->[[. sA]]
Val __dic_toIter (Val d) {
  return __arr_toIter(d);
}

// {A},(A->s)->s
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

// Auxiliar of __js_wo
// {A}->Map<char>
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

// {A}->[A]
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
