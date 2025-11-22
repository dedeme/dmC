// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_cryp.h"
#include "kut/cryp.h"
#include "exp.h"
#include "obj.h"
#include "runner/fail.h"

// \i -> s
static Exp *genk (Arr *exps) {
  CHECK_PARS ("cryp.genk", 1, exps);
  return exp_string(cryp_genk(exp_get_int(arr_get(exps, 0))));
}

// \s, i -> s
static Exp *key (Arr *exps) {
  CHECK_PARS ("cryp.key", 2, exps);
  return exp_string(cryp_key(
    exp_get_string(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1))
  ));
}

// \s, s -> s
static Exp *encode (Arr *exps) {
  CHECK_PARS ("cryp.encode", 2, exps);
  return exp_string(cryp_encode(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  ));
}

// \s, s -> <bytes>
static Exp *encode_bytes (Arr *exps) {
  CHECK_PARS ("cryp.encodeBytes", 2, exps);
  return exp_string(cryp_encode_bytes(
    exp_get_string(arr_get(exps, 0)),
    obj_get_bytes(arr_get(exps, 1))
  ));
}

// \s, s -> s
static Exp *decode (Arr *exps) {
  CHECK_PARS ("cryp.dcode", 2, exps);
  return exp_string(cryp_decode(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  ));
}

// \s, <bytes> -> s
static Exp *decode_bytes (Arr *exps) {
  CHECK_PARS ("cryp.dcode", 2, exps);
  return obj_bytes(cryp_decode_bytes(
    exp_get_string(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  ));
}

Bfunction md_cryp_get (char *fname) {
  if (!strcmp(fname, "genK")) return genk;
  if (!strcmp(fname, "key")) return key;
  if (!strcmp(fname, "decode")) return decode;
  if (!strcmp(fname, "decodeBytes")) return decode_bytes;
  if (!strcmp(fname, "encode")) return encode;
  if (!strcmp(fname, "encodeBytes")) return encode_bytes;

  EXC_KUT(fail_bfunction("cryp", fname));
  return NULL; // Unreachable
}
