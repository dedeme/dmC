// Copyright 16-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>

static char *FMENU = "menu.txt";
static char *FFLUX = "/home/deme/.fluxbox/menu";

static char *tab = "  ";

static void process_line(LckFile *target, int i, char *line) {
  char *l = str_trim(line);
  if (!*l || *l =='#') {
    return;
  }

  switch (*l) {
  case '-':
    file_write_text(target, str_printf("%s[nop]\n", tab));
    break;
  case '/':
    file_write_text(
      target,
      str_printf("%s[submenu] (%s)\n", tab, str_sub_end(l, 1))
    );
    tab = str_cat(tab, "  ", NULL);
    break;
  case '<':
    tab = *tab ? str_sub(tab, 0, 2) : tab;
    file_write_text(target, str_printf("%s[end]\n", tab));
    break;
  case '*': {
    Arr/*char*/ *fields = str_split(l, "||");
    switch(arr_size(fields)){
    case 3:
      arr_set(fields, 1, arr_get(fields, 2));
      break;
    case 2:
      break;
    default:
      printf("Bad number of fields (%zu)\n%d: %s\n", arr_size(fields), i, l);
    }
    file_write_text(
      target,
      str_printf(
        "%s[exec] (%s) {%s}\n",
        tab, str_sub_end(arr_get(fields, 0), 1), arr_get(fields, 1)
      )
    );
    break;
  }
  default:
    printf(
      "Invalid directive. A line must start with '-', '&lt;' or '*'.\n%d:%s\n",
      i, l
    );
  }
};

int main (int argc, char **argv) {
  sys_init("mkmenu");

  LckFile *target = file_wopen(FFLUX);
  file_write_text(target, "[begin] (Principal)\n");

  int i = 0;

  FNX(each, char, line) {
    process_line(target, i, line);
    ++i;
  }_FN
  LckFile *fmenu = file_open_it(FMENU);
  it_each(file_to_it(fmenu), each);

  file_write_text(
    target,
    "  [nop]\n"
    "  [submenu] (fluxbox)\n"
    "    [include] (/etc/X11/fluxbox/fluxbox-menu)\n"
    "  [end]\n"
    "[end]\n"
  );

  file_close(target);

  puts("Menú terminado\n");

  return 0;
}
