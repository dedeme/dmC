static void dic_add(Arrs *this, char *key, char *value) {
  int exists = 0;
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { exists = 1; break; }
    else ++p;
  if (exists)
    texc_expect("dic.add",
      str_f("Fail adding the key '%s'", key),
      "A new key", "Key is duplicate"
    );
  tarr_pushS(this, key);
  tarr_pushS(this, value);
}
static Arrs *dic_from_arr (Arrs *a) {
  if (tarr_sizeS(a) % 2)
    texc_throw("dic.fromArr", str_f("Array in not a valid dictionary:\n  %s",
      built_to_strS(a)));
  Arrs *r = tarr_new_s();
  char **p = tarr_beginS(a);
  char **end = tarr_endS(a);
  while (p < end) {
    char *k = *p++;
    char *v = *p++;
    dic_put(r, k, v);
  }
  return r;
}
static Arrs *dic_get(Arrs *this, char *key) {
  Arrs *r = tarr_new_s();
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { tarr_pushS(r, *p); return r; }
    else ++p;
  return r;
}
static char *dic_get_or_exc(Arrs *this, char *key) {
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { return *p; }
    else ++p;

  texc_throw("dic.get", str_f("Key '%s' not found", key));
  return NULL; // Unreachable
}
static char *dic_get_or_null(Arrs *this, char *key) {
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { return *p; }
    else ++p;
  return NULL;
}
static int dic_has_key(Arrs *this, char *key) {
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { return 1; }
    else ++p;
  return 0;
}
static Arrs *dic_keys(Arrs *this) {
  Arrs *r = tarr_new_s();
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end) { tarr_pushS(r, *p++); ++p; }
  return r;
}
static Arrs *dic_new() {
  return tarr_new_s();
}
static void dic_put(Arrs *this, char *key, char *value) {
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { *p = value; return; }
    else ++p;
  tarr_pushS(this, key);
  tarr_pushS(this, value);
}
static void dic_remove(Arrs *this, char *key) {
  char **source = tarr_beginS(this);
  char **target = source;
  char **end = tarr_endS(this);
  while (source < end) {
    if (!strcmp(*source, key)) { ++source; ++source; }
    else {
      if (source == target) { ++source; ++source; ++target; ++target; }
      else { *target++ = *source++; *target++ = *source++; }
    }
  }
  if (target < source) { tarr_popS(this); tarr_popS(this); }
}
static void dic_set(Arrs *this, char *key, char *value) {
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end)
    if (!strcmp(*p++, key)) { *p = value; return; }
    else ++p;
  texc_expect("dic.set",
    str_f("Fail changing the value of key '%s'", key),
    "Key already existing", "Key is not found"
  );
}
static int dic_size(Arrs *this) {
  return tarr_sizeS(this) / 2;
}
static Arrs *dic_values(Arrs *this) {
  Arrs *r = tarr_new_s();
  char **p = tarr_beginS(this);
  char **end = tarr_endS(this);
  while (p < end) { ++p; tarr_pushS(r, *p++); }
  return r;
}
