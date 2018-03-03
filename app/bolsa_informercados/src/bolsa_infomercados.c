// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>

static void help() {
  puts("Use:\n  bolsa_informercados file");
}

static char *process_line (char *line) {
  Arr/*char*/ *parts = str_csplit_trim(line, '\t');
  arr_remove(parts, 0);
  char **os = (char **)arr_es(parts);
  char *tmp = os[4];
  os[4] = os[3];
  os[3] = os[2];
  os[2] = tmp;

  char *date = arr_get(parts, 0);
  char *d = str_sub(date, 0, 2);
  char *m = str_sub(date, 3, 5);
  char *y = str_sub_end(date, 6);
  arr_set(parts, 0, str_printf("20%s%s%s", y, m, d));

  arr_add(parts, "false");

  return str_creplace(str_cjoin(it_from(parts), ':'), ',', '.');
}

int main (int argc, char **argv) {
  exc_init();

  if (argc != 2) {
    puts("Wrong number of parameters calling bolsa_infomercados");
    help();
    return 0;
  }

  char *source = argv[1];
  char *target = str_printf("%s.dat", source);

  char *text = file_read(source);

  Arr/*char*/ *lines = str_csplit(text, '\n');
  arr_remove(lines, 0);
  arr_reverse(lines);

  Arr/*char*/ *new_lines = arr_new();
  EACH(lines, char, line) {
    char *tmp = str_trim(line);
    if (*tmp) {
      arr_add(new_lines, process_line(line));
    }
  }_EACH

  file_write(target, str_cjoin(it_from(new_lines), '\n'));

  return 0;
}
