static int texc_init();
// Creates, push and returns jmp_buf
static jmp_buf *texc_push_jump();
static jmp_buf *texc_peek_jump();
static void texc_remove_last_jump();
static char *texc_get_msg();
static void texc_throw_(char *message);
static void texc_throw(char *function, char *message);
static void texc_expect (
  char *function, char *message, char *actual, char *expected);
// begin inclusive, end exclusive.
static void texc_check_range (char *function, int begin, int end, int index);
static void texc_check_type (char *function, Obj *o, char *type);
