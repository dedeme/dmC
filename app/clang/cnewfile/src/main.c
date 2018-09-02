// Copyright 30-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "dmc/std.h"
#include "dmc/Date.h"

static char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sept", "Oct", "Nov","Dec"
  };

static char *date(void) {
  Date date = date_now();
  char *r = date_format(date, "%d-M-%Y");
  int month = date_month(date) - 1;
  return str_replace(r, "M", months[month]);
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

  char *cname = str_to_upper(path);
  cname = str_creplace(cname, '/', '_');
  char *tx = str_replace(template, "$DATE$", date());
  tx = str_replace(tx, "$CNAME$", cname);
  file_write(hfile, tx);
}

static void mkc(char *cfile, char *path) {
  char *template =
    "// Copyright $DATE$ ºDeme\n"
    "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n\n"
    "#include \"$CNAME$.h\"\n\n"
  ;

  char *tx = str_replace(template, "$DATE$", date());
  tx = str_replace(tx, "$CNAME$", path);
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
    THROW(exc_io_t) "'%s' is not a directory", path _THROW

  char *include = path_cat(path, "include", NULL);
  if (!file_is_directory(include))
    THROW (exc_io_t)"'%s' is not a directory", include _THROW

  char *src = path_cat(path, "src", NULL);
  if (!file_is_directory(src))
    THROW(exc_io_t) "'%s' is not a directory", src _THROW

  char *file_name = argv[2];

  char *hfile = str_printf("%s/%s.h", include, file_name);
  if (file_exists(hfile)) {
    THROW(exc_io_t) "'%s' already exists", hfile _THROW
  }

  char *cfile = str_printf("%s/%s.c", src, file_name);
  if (file_exists(cfile)) {
    THROW(exc_io_t) "'%s' already exists", cfile _THROW
  }

  mkh(hfile, file_name);
  mkc(cfile, file_name);

  return 0;
}
