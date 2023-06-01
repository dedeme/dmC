// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs.h"
#include "kut/DEFS.h"
#include "kut/cryp.h"

static char *comKey = NULL;

void pgs_process (char *ck, Map *rq) {
  comKey = ck;
  char *module = pgs_rs(rq, "module");
  if (module) {
    if (str_eq(module, "abc")) {
    } else {
      EXC_GENERIC(str_f(
        "Value '%s' not found for 'module' in '%s'", module, js_wo(rq)
      ));
    }
  }
}

int pgs_rb (Map *rq, char *key) {
  return js_rb(pgs_r(rq, key));
}

int pgs_ri (Map *rq, char *key) {
  return js_ri(pgs_r(rq, key));
}

int64_t pgs_rl (Map *rq, char *key) {
  return js_rl(pgs_r(rq, key));
}

char *pgs_rs (Map *rq, char *key) {
  return js_rs(pgs_r(rq, key));
}

char *pgs_r (Map *rq, char *key) {
  char *r = opt_get(map_get(rq, key));
  if (!r)
    EXC_GENERIC(str_f("Key '%s' not found in '%s'", key, js_wo(rq)));
  return r;
}

void pgs_print (Map *rp) {
  printf(cryp_encode(comKey, js_wo(rp)));
}

void pgs_print_empty (void);
