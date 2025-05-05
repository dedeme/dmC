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

// s -> s
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

// s,i,i -> i
Val ___built_divi(char *pos, Val n1, Val n2) {
  if (n2.i == 0) ___built_throw(pos, (Val)"Division by 0");
  return (Val) (n1.i / n2.i);
}

// s,f,f -> f
Val ___built_divf(char *pos, Val n1, Val n2) {
  if (math_eq(n2.f, 0)) ___built_throw(pos, (Val)"Division by 0");
  return (Val) (n1.f / n2.f);
}

// s,*,*->b
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

// s,*->s
Val ___built_to_str_trace (Val type, Val value) {
  char *t = __arr_get(type, (Val)0).s;
  return (*t == 's')
    ? __js_ws(value)
    : ___built_to_str(type, value)
  ;
}

// s,*->s
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

// s, s -> ()
void ___built_throw (char *pos, Val msg) {
  Exc *exc = exc_get();

  void *array[30];
  size_t size;
  char **strings;
  size = backtrace(array, 30);
  strings = backtrace_symbols(array, size);
  // Arr[char]
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

// b,s,s,*->()
void ___built_trace (Val isComplete, char *pos, Val type, Val value) {
  char *r = ___built_to_str_trace(type, value).s;
  if (!(isComplete.b)) r = arr_join(arr_take(str_runes(r), 50), "");
  puts(str_f("%s %s", ___built_mpos(pos), r));
}

