// Copyright 04-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_bytes.h"
#include "exp.h"
#include "DEFS.h"
#include "obj.h"
#include "runner/fail.h"

// \<bytes>, <bytes> -> <bytes>
static Exp *add (Arr *exps) {
  CHECK_PARS ("bytes.add", 2, exps);
  Bytes *bs1 = obj_rget_bytes(arr_get(exps, 0));
  Bytes *bs2 = obj_rget_bytes(arr_get(exps, 1));
  bytes_add(bs1, bs2);
  return obj_bytes(bs1);
}

// \<bytes>, i -> <bytes>
static Exp *drop (Arr *exps) {
  CHECK_PARS ("bytes.drop", 2, exps);
  Bytes *bs = obj_rget_bytes(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  int64_t sz = bytes_len(bs);
  Bytes *bs2 = sz > ix
    ? ix <= 0
      ? bytes_from_bytes(bytes_bs(bs), sz)
      : bytes_from_bytes(bytes_bs(bs) + ix, sz - ix)
    : bytes_new()
  ;
  return obj_bytes(bs2);
}

// \[i...] -> <bytes>
static Exp *from_arr (Arr *exps) {
  CHECK_PARS ("bytes.fromArr", 1, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int sz = arr_size(a);
  Bytes *bs = bytes_new_bf(sz);
  unsigned char *p = bytes_bs(bs);
  EACH(a, Exp, e) {
    *p++ = (unsigned char)exp_rget_int(e);
  }_EACH
  return obj_bytes(bs);
}

// \s -> <bytes>
static Exp *from_str (Arr *exps) {
  CHECK_PARS ("bytes.fromStr", 1, exps);
  return obj_bytes(bytes_from_str(exp_rget_string(arr_get(exps, 0))));
}

// \<bytes>, i -> i
static Exp *get (Arr *exps) {
  CHECK_PARS ("bytes.get", 2, exps);
  Bytes *bs = obj_rget_bytes(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  EXC_RANGE(ix, 0, bytes_len(bs) - 1);
  return exp_int(*(bytes_bs(bs) + ix));
}

// \i -> <bytes>
static Exp *new (Arr *exps) {
  CHECK_PARS ("bytes.new", 1, exps);
  int64_t bf = exp_rget_int(arr_get(exps, 0));
  if (bf < 0) bf = 0;
  Bytes *bs = bytes_new_bf(bf);
  unsigned char *p = bytes_bs(bs);
  while (bf) {
    *p++ = 0;
    --bf;
  }
  return obj_bytes(bs);
}

// \<bytes>, i -> <bytes>
static Exp *take (Arr *exps) {
  CHECK_PARS ("bytes.take", 2, exps);
  Bytes *bs = obj_rget_bytes(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  int64_t sz = bytes_len(bs);
  Bytes *bs2 = ix <= 0
    ? bytes_new()
    : ix >= sz
      ? bytes_from_bytes(bytes_bs(bs), sz)
      : bytes_from_bytes(bytes_bs(bs), ix)
  ;
  return obj_bytes(bs2);
}

// \<bytes>, i, i -> ()
static Exp *set (Arr *exps) {
  CHECK_PARS ("bytes.set", 3, exps);
  Bytes *bs = obj_rget_bytes(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  EXC_RANGE(ix, 0, bytes_len(bs) - 1);
  unsigned char v = exp_rget_int(arr_get(exps, 2));
  *(bytes_bs(bs) + ix) = v;
  return exp_empty();
}

// \<bytes> -> i
static Exp *size (Arr *exps) {
  CHECK_PARS ("bytes.size", 1, exps);
  return exp_int(bytes_len(obj_rget_bytes(arr_get(exps, 0))));
}

// \<byte> -> [i...]
static Exp *to_arr (Arr *exps) {
  CHECK_PARS ("bytes.toArr", 1, exps);
  Bytes *bs = obj_rget_bytes(arr_get(exps, 0));
  int sz = bytes_len(bs);
  // <Exp>
  Arr *a = arr_new();
  unsigned char *p = bytes_bs(bs);
  while (sz) {
    arr_push(a, exp_int((int64_t)*p++));
    --sz;
  }
  return exp_array(a);
}

// \<byte> -> s
static Exp *to_str (Arr *exps) {
  CHECK_PARS ("bytes.toStr", 1, exps);
  return exp_string(bytes_to_str(obj_rget_bytes(arr_get(exps, 0))));
}

Bfunction md_bytes_get (char *fname) {
  if (!strcmp(fname, "add")) return add;
  if (!strcmp(fname, "drop")) return drop;
  if (!strcmp(fname, "fromArr")) return from_arr;
  if (!strcmp(fname, "fromStr")) return from_str;
  if (!strcmp(fname, "get")) return get;
  if (!strcmp(fname, "new")) return new;
  if (!strcmp(fname, "set")) return set;
  if (!strcmp(fname, "size")) return size;
  if (!strcmp(fname, "take")) return take;
  if (!strcmp(fname, "toArr")) return to_arr;
  if (!strcmp(fname, "toStr")) return to_str;

  EXC_KUT(fail_bfunction("bytes", fname));
  return NULL; // Unreachable
}
