static void texc_add(jmp_buf *jump);
static void texc_remove();
static char *texc_get();

static void texc_throw_(char *message);
static void texc_throw(char *function, char *message);
static void texc_expect (
  char *function, char *message, char *actual, char *expected);
// begin inclusive, end exclusive.
static void texc_check_range (char *function, int begin, int end, int index);
static void texc_check_type (char *function, Obj *o, char *type);
