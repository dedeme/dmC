// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "args.h"
#include "DEFS.h"

// Arr<char>
static Arr *args = NULL;

// Map<char>
static Map *stkargs = NULL;

static int production = 1;

static void help (void) {
  puts(
    "dmstack v. " VERSION ".\n"
    "Copyright 23-Sept-2019 ºDeme\n"
    "GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
    "\n"
    "Use:\n"
    "  dmstack [Options] dmsProgram [-- dmsProgramOptions]\n"
    "or to show this message:\n"
    "  dmstack\n"
    "-----------------------\n"
    "Options: They are\n"
    "  -d : Execute in debug mode. \n"
    "dmsProgram: Path to .dms file, with or without extension .dms.\n"
    "dmsProgramOptions: Options to be passed to .dms program.\n"
  );
}

int args_init (int argc, char *argv[]) {
  args = arr_new();
  stkargs = map_new();

  if (argc < 2) {
    help();
    return 1;
  }

  int c = 1;
  for (; c < argc; ++c) {
    if (c == argc) break;
    char *a = argv[c];

    if (*a == '-') {
      if (str_eq(a, "--")) {
        break;
      } else if (str_eq(a, "-d")) {
        production = 0;
        map_put(stkargs, "-d", "");
        continue;
      }

      printf ("Unkown option '%s'\n\n=======================\n", a);
      help();
      return 1;
    } else {
      map_put(stkargs, "dms", a);
      ++c;
      break;
    }
  }

  if (!map_has_key(stkargs, "dms")) {
    puts(
      "Name of .dms file is missing.\n\n"
      "======================="
    );
    help();
    return 1;
  }

  if (c == argc) return 0;

  if (!str_eq(argv[c], "--")) {
    puts(
      "Expected '--'.\n\n"
      "======================="
    );
    help();
    return 1;
  }

  ++c;
  for (; c < argc; ++c) arr_push(args, argv[c]);

  return 0;
}

Opt *args_param (char *key) {
  return map_get(stkargs, key);
}

Arr *args_dms_params (void) {
  return args;
}

int args_is_production (void) {
  return production;
}
