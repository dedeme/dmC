static void texc_add(jmp_buf *jump) {
  built_exc_fns('+', jump);
}
static void texc_remove() {
  built_exc_fns('-', NULL);
}
static char *texc_get() {
  char *msg;
  built_exc_fns('<', &msg);
  return msg;
}
static void texc_throw_(char *message) {
  built_exc_fns(':', message);
}
static void texc_throw(char *function, char *message) {
  texc_throw_(str_f(
    "Typed exception in %s:\n%s", function, message
  ));
}
static void texc_expect (
  char *function, char *message, char *expected, char *actual
) {
  texc_throw(function, str_f(
    "%s\nExpected: %s\n  Actual: %s",
    message, expected, actual
  ));
}
// [begin, end]
static void texc_check_range (char *function, int begin, int end, int index) {
  if (index < begin || index > end) {
    texc_throw(function, str_f(
      "Index out of range: %d < %d or %d > %d",
      index, begin, index, end
    ));
  }
}
static void texc_check_type (char *function, Obj *o, char *type) {
  if (strcmp(o->type, type))
    texc_expect(function, "Bad object type", type, o->type);
}
