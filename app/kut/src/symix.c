// Copyright 11-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "symix.h"

// <char>
static Arr *syms;

void symix_init(void) {
  // NOTE: arr, dic and str are used in st_reader/read_symbol
  //       and all builtin modules in reader/read_main_block;
  syms = arr_new_from(
    // Reserverd words
    "break", "catch", "continue", "default",
    "else", "false", "finally", "for",
    "if", "import", "return", "switch",
    "trace", "true", "try", "while",
    "-separator-",
    // Built-in modules
    "arr", "b64", "bytes", "cryp",
    "dic", "iter", "js", "file",
    "math", "path", "regex", "str",
    "sys", "tcp", "thread", "time",
    "-finalizador-",
    NULL
  );

  char **ss = (char **)arr_begin(syms);
  assert(!strcmp(ss[symix_BREAK], "break"));
  assert(!strcmp(ss[symix_CATCH], "catch"));
  assert(!strcmp(ss[symix_CONTINUE], "continue"));
  assert(!strcmp(ss[symix_DEFAULT], "default"));

  assert(!strcmp(ss[symix_ELSE], "else"));
  assert(!strcmp(ss[symix_FALSE], "false"));
  assert(!strcmp(ss[symix_FINALLY], "finally"));
  assert(!strcmp(ss[symix_FOR], "for"));

  assert(!strcmp(ss[symix_IF], "if"));
  assert(!strcmp(ss[symix_IMPORT], "import"));
  assert(!strcmp(ss[symix_RETURN], "return"));
  assert(!strcmp(ss[symix_SWITCH], "switch"));

  assert(!strcmp(ss[symix_TRACE], "trace"));
  assert(!strcmp(ss[symix_TRUE], "true"));
  assert(!strcmp(ss[symix_TRY], "try"));
  assert(!strcmp(ss[symix_WHILE], "while"));

  assert(!strcmp(ss[symix_ARR], "arr"));
  assert(!strcmp(ss[symix_B64], "b64"));
  assert(!strcmp(ss[symix_BYTES], "bytes"));
  assert(!strcmp(ss[symix_CRYP], "cryp"));

  assert(!strcmp(ss[symix_DIC], "dic"));
  assert(!strcmp(ss[symix_ITER], "iter"));
  assert(!strcmp(ss[symix_JS], "js"));
  assert(!strcmp(ss[symix_FILE], "file"));

  assert(!strcmp(ss[symix_MATH], "math"));
  assert(!strcmp(ss[symix_PATH], "path"));
  assert(!strcmp(ss[symix_REGEX], "regex"));
  assert(!strcmp(ss[symix_STR], "str"));

  assert(!strcmp(ss[symix_SYS], "sys"));
  assert(!strcmp(ss[symix_TCP], "tcp"));
  assert(!strcmp(ss[symix_THREAD], "thread"));
  assert(!strcmp(ss[symix_TIME], "time"));
}

int symix_add(char *sym) {
  EACH(syms, char, s) {
    if (!strcmp(sym, s)) return _i;
  }_EACH
  arr_push(syms, sym);
  return arr_size(syms) - 1;
}

char *symix_get (int ix) {
  return arr_get(syms, ix);
}

static int id = 0;
int symix_generate (void) {
  arr_push(syms, str_f("gen-%d-", id++));
  return arr_size(syms) - 1;
}
