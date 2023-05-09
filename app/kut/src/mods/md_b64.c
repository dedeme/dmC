// Copyright 04-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_b64.h"
#include "kut/b64.h"
#include "exp.h"
#include "obj.h"
#include "runner/fail.h"

// \s -> s
static Exp *decode (Arr *exps) {
  CHECK_PARS ("b64.decode", 1, exps);
  return exp_string(b64_decode(exp_rget_string(arr_get(exps, 0))));
}

// \s -> <bytes>
static Exp *decode_bytes (Arr *exps) {
  CHECK_PARS ("b64.decodeBytes", 1, exps);
  return obj_bytes(b64_decode_bytes(exp_rget_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *encode (Arr *exps) {
  CHECK_PARS ("b64.encode", 1, exps);
  return exp_string(b64_encode(exp_rget_string(arr_get(exps, 0))));
}

// \<bytes> -> s
static Exp *encode_bytes (Arr *exps) {
  CHECK_PARS ("b64.encodeBytes", 1, exps);
  return exp_string(b64_encode_bytes(obj_rget_bytes(arr_get(exps, 0))));
}

Bfunction md_b64_get (char *fname) {
  if (!strcmp(fname, "decode")) return decode;
  if (!strcmp(fname, "decodeBytes")) return decode_bytes;
  if (!strcmp(fname, "encode")) return encode;
  if (!strcmp(fname, "encodeBytes")) return encode_bytes;

  EXC_KUT(fail_bfunction("b64", fname));
  return NULL; // Unreachable
}
