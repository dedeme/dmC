// Copyright 30-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cnewfile.h"
#include "dmc/std.h"
#include "dmc/date.h"

#include <limits.h>
#include <stdint.h>

static char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sept", "Oct", "Nov","Dec"
  };

static char *mk_date() {
  time_t date = date_now();
  char *r = date_f(date, "%d-M-%Y");
  int month = date_month(date) - 1;
  char *tmp = r;
  r = str_replace(tmp, "M", months[month]);
  return r;
}

static void help () {
  puts(
    "Use: cnewfile root relativePath\n"
    "root: It is the common parent of 'include' and 'src'\n"
    "relativePath: It is the path of '.h .c' files relative to 'root',\n"
    "  without extension\n"
    "For expample:\n"
    "cnewfile /home/frank/cprograms/oneprg test/save_test\n"
    "will create two files:\n"
    "/home/frank/cprograms/oneprg/include/test/save_test.h\n"
    "and\n"
    "/home/frank/cprograms/oneprg/src/test/save_test.c\n"
  );
}

static void mkh(char *hfile, char *path) {
  char *template =
    "// Copyright $DATE$ ºDeme\n"
    "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n\n"
    "#ifndef $CNAME$_H\n"
    "  #define $CNAME$_H\n\n"
    "#include \"dmc/std.h\"\n\n"
    "#endif\n"
  ;

  char *d = mk_date();
  char *cname = str_creplace(str_to_upper(path), '/', '_');
  char *tx = str_replace(str_replace(template, "$DATE$", d),
    "$CNAME$", cname
  );
  file_write(hfile, tx);
}

static void mkc(char *cfile, char *path) {
  char *template =
    "// Copyright $DATE$ ºDeme\n"
    "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n\n"
    "#include \"$CNAME$.h\"\n\n"
  ;

  char *d = mk_date();
  char *tx = str_replace(str_replace(template, "$DATE$", d), "$CNAME$", path);
  file_write(cfile, tx);
}

int main (int argc, char **argv) {
  sys_init("ctemplate");

  if (argc != 3) {
    help();
    return 0;
  }

  char *path = argv[1];

  if (!file_is_directory(path))
    EXC_ILLEGAL_STATE(str_f("'%s' is not a directory", path))

  char *include = path_cat(path, "include", NULL);
  if (!file_is_directory(include))
    EXC_ILLEGAL_STATE(str_f("'%s' is not a directory", include))

  char *src = path_cat(path, "src", NULL);
  if (!file_is_directory(src))
    EXC_ILLEGAL_STATE(str_f("'%s' is not a directory", src))

  char *file_name = argv[2];

  char *hfile = str_f("%s/%s.h", include, file_name);
  if (file_exists(hfile))
    EXC_ILLEGAL_STATE(str_f("'%s' already exists", hfile))

  char *cfile = str_f("%s/%s.c", src, file_name);
  if (file_exists(cfile))
    EXC_ILLEGAL_STATE(str_f("'%s' already exists", cfile))

  mkh(hfile, file_name);
  mkc(cfile, file_name);
  return 0;
}
