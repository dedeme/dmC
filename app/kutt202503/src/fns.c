// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "fns.h"
#include "kut/js.h"

static int new_id = 0;
char *fns_next_id (char *mod) {
  return str_f("%s_%d", mod, new_id++);
}

char *fns_mk_fn_head (int with_return, Arr *pars, char *id) {
  char *fmap (char *p) { return str_f("Val %s", p); }
  return str_f("%s%s (%s)",
    with_return ? "Val " : "void ",
    id,
    arr_size(pars) ? arr_cjoin(arr_map(pars, (FMAP)fmap), ',') : "void"
  );
}

char *fns_mk_pos (char *module_id, int line) {
  return js_ws(str_f("%s:%d", module_id, line));
}

char *fns_fun_code (Arr *fns) {
  int ffilter (char *fn_code) { return *fn_code; }
  return arr_cjoin(arr_filter_to(fns, (FPRED)ffilter), '\n');
}

char *fns_cut_msg (char *msg, int len) {
  // Arr<char>
  Arr *rs = str_runes(msg);
  if (arr_size(rs) <= len) return msg;
  return str_f("%s...", arr_join(arr_take(rs, len - 3), ""));
}
