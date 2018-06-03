// Copyright 03-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include "minijs.h"
#include "DEFS.h"
#include "dmc/sys.h"

static void help(void) {
  printf("minijs. v%s\n\n", VERSION);
  puts(
    "Use:\n"
		"  minijs [OPTIONS] <main_file> \n"
		"  or\n"
		"  minijs -h\n\n"
    "OPTIONS:\n"
    "  -js <path> File '.js'.\n"
    "      Default: index.js in the same directory that <main_file>\n"
    "  -p <paths> List of source paths separated with ':'\n"
    "      Automaticaly added current directory.\n\n"
    "main_file:\n"
    "  mini file without extension\n\n"
    "Examples:\n"
    "  minijs MyProgram\n"
    "  minijs -p /home/my/lib1:lib2 MyProgram\n"
  );
}

int main (int argc, char **argv) {
  sys_init("minijs");

  help();

  return 0;
}
