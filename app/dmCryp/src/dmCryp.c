// Copyright 10-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmCryp.h"
#include "kut/cryp.h"
#include "kut/math.h"
#include "kut/sys.h"
#include "kut/str.h"
#include <stdio.h>

static void help () {
  puts(
    "Use dmCryp <op> <params>\n"
    "  dmCryp k n     : Generate a key of 'n' B64 digits\n"
    "                   (e.g.) dmCryp k 12\n"
    "  dmCryp k n text: Encodes 'text' in 'n' B64 digits\n"
    "                   (e.g.) dmCryp k 8 \"A text\"\n"
    "  dmCryp e k text: Encodes 'text' with key 'k'\n"
    "                   (e.g.) dmCryp e \"A key\" \"A text\"\n"
    "  dmCryp d k text: Decodes 'text' with key 'k'\n"
    "                   (e.g.) dmCryp d \"A key\" \"v7h2iK2mrWU=\"\n"
  );
}

int main(int argc, char **args) {
  sys_init();

  int r = 1;

  if (argc == 3) {
    if (str_eq(args[1], "k")) {
      if (math_digits(args[2])) {
        int n = math_stoi(args[2]);
        if (n > 0) {
          char *k = cryp_genk(n);
          printf(k);
          r = 0;
        } else {
          help();
        }
      } else {
        help();
      }
    } else {
      help();
    }
  } else if (argc == 4) {
    if (str_eq(args[1], "k")) {
      if (math_digits(args[2])) {
        int n = math_stoi(args[2]);
        if (n > 0) {
          char *k = cryp_key(args[3], n);
          printf(k);
          r = 0;
        } else {
          help();
        }
      } else {
        help();
      }
    } else if (str_eq(args[1], "e")) {
      char *msg = cryp_encode(args[2], args[3]);
      printf(msg);
      r = 0;
    } else if (str_eq(args[1], "d")) {
      char *msg = cryp_decode(args[2], args[3]);
      printf(msg);
      r = 0;
    } else {
      help();
    }
  } else {
    help();
  }

  return r;
}
