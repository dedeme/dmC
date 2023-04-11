// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bmodule.h"
#include "DEFS.h"
#include "runner/fail.h"
#include "mods/md_arr.h"
#include "mods/md_bytes.h"
#include "mods/md_b64.h"
#include "mods/md_cryp.h"
#include "mods/md_dic.h"
#include "mods/md_file.h"
#include "mods/md_iter.h"
#include "mods/md_js.h"
#include "mods/md_math.h"
#include "mods/md_path.h"
#include "mods/md_regex.h"
#include "mods/md_str.h"
#include "mods/md_sys.h"
#include "mods/md_tcp.h"
#include "mods/md_thread.h"
#include "mods/md_time.h"

static const char *modules[] = {
  "arr", "b64", "bytes", "cryp", "dic", "iter", "js", "file",
	"math", "path", "regex", "str", "sys", "tcp", "thread", "time",
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
  if (!strcmp(md, "bytes")) return md_bytes_get(fn);
  if (!strcmp(md, "b64")) return md_b64_get(fn);
  if (!strcmp(md, "cryp")) return md_cryp_get(fn);
  if (!strcmp(md, "dic")) return md_dic_get(fn);
  if (!strcmp(md, "file")) return md_file_get(fn);
  if (!strcmp(md, "iter")) return md_iter_get(fn);
  if (!strcmp(md, "js")) return md_js_get(fn);
  if (!strcmp(md, "math")) return md_math_get(fn);
  if (!strcmp(md, "path")) return md_path_get(fn);
  if (!strcmp(md, "regex")) return md_regex_get(fn);
  if (!strcmp(md, "str")) return md_str_get(fn);
  if (!strcmp(md, "sys")) return md_sys_get(fn);
  if (!strcmp(md, "tcp")) return md_tcp_get(fn);
  if (!strcmp(md, "thread")) return md_thread_get(fn);
  if (!strcmp(md, "time")) return md_time_get(fn);

  EXC_KUT(str_f("Module '%s' not found", md));
  return NULL; // Unreachable
}
