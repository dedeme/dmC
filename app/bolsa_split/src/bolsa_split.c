// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>

static void help() {
  puts(
    "Use:\n  bolsa_split file increment\n"
    "For example:\n  bolsa_split data.txt 1.77"
  );
}

static char *process_line (char *line, double inc) {
  Arr/*char*/ *rs = arr_new();
  Arr/*char*/ *parts = str_csplit(line, ':');

  arr_add(rs, arr_get(parts, 0));
  arr_add(rs, str_printf("%.3f", atof(arr_get(parts, 1)) * inc));
  arr_add(rs, str_printf("%.3f", atof(arr_get(parts, 2)) * inc));
  arr_add(rs, str_printf("%.3f", atof(arr_get(parts, 3)) * inc));
  arr_add(rs, str_printf("%.3f", atof(arr_get(parts, 4)) * inc));
  arr_add(rs, arr_get(parts, 5));
  arr_add(rs, arr_get(parts, 6));

  return str_cjoin(it_from(rs), ':');
}

int main (int argc, char **argv) {
  exc_init();

  if (argc != 3) {
    puts("Wrong number of parameters calling bolsa_split");
    help();
    return 0;
  }

  char *source = argv[1];
  char *target = str_printf("%s.dat", source);

  double inc = atof(argv[2]);

  char *text = file_read(source);

  Arr/*char*/ *lines = str_csplit(text, '\n');

  Arr/*char*/ *new_lines = arr_new();
  EACH(lines, char, line) {
    char *tmp = str_trim(line);
    if (*tmp) {
      arr_add(new_lines, process_line(line, inc));
    }
  }_EACH

  text = str_cjoin(it_from(new_lines), '\n');
  file_write(target, text);

  return 0;
}
