// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bmodule.h"
#include "DEFS.h"
#include "runner/fail.h"
#include "mods/md_arr.h"
#include "mods/md_dic.h"
#include "mods/md_math.h"
#include "mods/md_str.h"
#include "mods/md_sys.h"

static const char *modules[] = {
  "arr", "b64", "bytes", "cryp", "dic", "iter", "js", "file",
	"math", "path", "regex", "str", "tcp", "thread", "time", "sys",
  NULL
};

int bmodule_exists (char *md_name) {
  const char **p = modules;
  while (*p) {
    if (!strcmp(*p, md_name)) return TRUE;
    ++p;
  }
  return FALSE;
}

// <Bfunction>
Bfunction bmodule_get_function (char *md, char *fn) {
  if (!strcmp(md, "arr")) return md_arr_get(fn);
  if (!strcmp(md, "dic")) return md_dic_get(fn);
  if (!strcmp(md, "math")) return md_math_get(fn);
  if (!strcmp(md, "str")) return md_str_get(fn);
  if (!strcmp(md, "sys")) return md_sys_get(fn);

  EXC_KUT(str_f("Module '%s' not found", md));
  return NULL; // Unreachable
}
