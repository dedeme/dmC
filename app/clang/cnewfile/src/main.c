// Copyright 30-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "dmc/std.h"
#include "dmc/date.h"

static char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sept", "Oct", "Nov","Dec"
  };

static char *mk_date_new(void) {
  time_t date = date_now();
  char *r = date_f_new(date, "%d-M-%Y");
  int month = date_month(date) - 1;
  char *tmp = r;
  r = str_replace_new(tmp, "M", months[month]);
  free(tmp);
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

  char *d = mk_date_new();
  char *cname = str_to_upper_new(path);
  str_creplace(&cname, '/', '_');
  char *tmp = str_replace_new(template, "$DATE$", d);
  char *tx = str_replace_new(tmp, "$CNAME$", cname);
  file_write(hfile, tx);
  free(d);
  free(cname);
  free(tmp);
  free(tx);
}

static void mkc(char *cfile, char *path) {
  char *template =
    "// Copyright $DATE$ ºDeme\n"
    "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n\n"
    "#include \"$CNAME$.h\"\n\n"
  ;

  char *d = mk_date_new();
  char *tmp = str_replace_new(template, "$DATE$", d);
  char *tx = str_replace_new(tmp, "$CNAME$", path);
  file_write(cfile, tx);
  free(d);
  free(tmp);
  free(tx);
}

int main (int argc, char **argv) {
  sys_init("ctemplate");

  if (argc != 3) {
    help();
    return 0;
  }

  char *path = argv[1];

  if (!file_is_directory(path))
    FAIL(str_f_new("'%s' is not a directory", path))

  char *include = path_cat_new(path, "include", NULL);
  if (!file_is_directory(include))
    FAIL(str_f_new("'%s' is not a directory", include))

  char *src = path_cat_new(path, "src", NULL);
  if (!file_is_directory(src))
    FAIL(str_f_new("'%s' is not a directory", src))

  char *file_name = argv[2];

  char *hfile = str_f_new("%s/%s.h", include, file_name);
  if (file_exists(hfile)) {
    FAIL(str_f_new("'%s' already exists", hfile))
  }
  free(include);

  char *cfile = str_f_new("%s/%s.c", src, file_name);
  if (file_exists(cfile)) {
    FAIL(str_f_new("'%s' already exists", cfile))
  }
  free(src);

  mkh(hfile, file_name);
  mkc(cfile, file_name);

  free(hfile);
  free(cfile);

  sys_end();

  return 0;
}
