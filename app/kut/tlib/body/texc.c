typedef struct texc_Texc{
  char *msg;
  Arr *jumps;
} Texc;
static Texc *texc_e = NULL;
static int texc_init() {
  if (texc_e) return 0;

  texc_e = MALLOC(Texc);
  texc_e->jumps = tarr_new_();
  texc_e->msg = "";
  return 1;
}
static jmp_buf *texc_push_jump() {
  jmp_buf *bf = MALLOC(jmp_buf);
  tarr_push(texc_e->jumps, bf);
  return bf;
}
static jmp_buf *texc_peek_jump() {
  return (jmp_buf *)tarr_peek(texc_e->jumps);
}
static void texc_remove_last_jump() {
  tarr_pop(texc_e->jumps);
}
static char *texc_get_msg() {
  return texc_e->msg;
}
static void texc_throw_(char *message) {
  texc_e->msg = message;
  longjmp(*((jmp_buf *)tarr_peek(texc_e->jumps)), 1);
}
static void texc_throw(char *function, char *message) {
  texc_e->msg = str_f(
    "Typed exception in %s:\n%s", function, message
  );
  longjmp(*((jmp_buf *)tarr_peek(texc_e->jumps)), 1);
}
static void texc_expect (
  char *function, char *message, char *expected, char *actual
) {
  texc_e->msg = str_f(
    "Typed exception in %s:\n%s\nExpected: %s\n  Actual: %s",
    function, message, expected, actual
  );
  longjmp(*((jmp_buf *)tarr_peek(texc_e->jumps)), 1);
}
// [begin, end]
static void texc_check_range (char *function, int begin, int end, int index) {
  if (index < begin || index > end) {
    texc_e->msg = str_f(
      "Typed exception in %s:\nIndex out of range: %d out of [%d - %d]",
      function, index, begin, end
    );
    longjmp(*((jmp_buf *)tarr_peek(texc_e->jumps)), 1);
  }
}
static void texc_check_type (char *function, Obj *o, char *type) {
  if (strcmp(o->type, type))
    texc_expect(function, "Bad object type", type, o->type);
}
