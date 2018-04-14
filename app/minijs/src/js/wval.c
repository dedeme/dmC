// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/wval.h"
#include "js/wfun.h"
#include "ast/Attach.h"

static void write_att(Buf *bf, Map/*char*/ *imports, Attach *a) {
  Arr/*values*/ *vs;
  switch (attach_type(a)) {
    case ADOT:
      buf_cadd(bf, '.');
      buf_add(bf, attach_id(a));
      break;
    case ASUB:
      buf_cadd(bf, '[');
      wval_writebf(bf, imports, arr_get(attach_values(a), 0));
      buf_cadd(bf, ']');
      break;
    case AFN:
      vs = attach_values(a);
      buf_cadd(bf, '(');
      if (arr_size(vs)) {
        wval_writebf(bf, imports, arr_get(vs, 0));
        RANGE(i, 1, arr_size(vs)) {
          buf_cadd(bf, ',');
          wval_writebf(bf, imports, arr_get(vs, i));
        }_RANGE
      }
      buf_cadd(bf, ')');
      break;
    default:
      THROW "Unknown Attach type %d", attach_type(a) _THROW
  }
}

static void write_atts(Buf *bf, Map/*char*/ *imports, Arr/*Attach*/ *atts) {
  EACH(atts, Attach, a) {
    write_att(bf, imports, a);
  }_EACH
}

static void write_null(Buf *bf) {
  buf_add(bf, "null");
}

static void write_bif(Buf *bf, Map/*char*/ *imports, Value *v) {
  buf_add(bf, arr_get(value_ids(v), 0));
  write_atts(bf, imports, value_attachs(v));
}

static void write_str(Buf *bf, Map/*char*/ *imports, Value *v, char quote) {
  buf_cadd(bf, quote);
  buf_add(bf, arr_get(value_ids(v), 0));
  buf_cadd(bf, quote);
  write_atts(bf, imports, value_attachs(v));
}

static void write_arr(Buf *bf, Map/*char*/ *imports, Value *v) {
  Arr/*Value*/ *vs = value_values(v);
  buf_cadd(bf, '[');
  if (arr_size(vs)) {
    wval_writebf(bf, imports, arr_get(vs, 0));
    RANGE(i, 1, arr_size(vs)) {
      buf_cadd(bf, ',');
      wval_writebf(bf, imports, arr_get(vs, i));
    }_RANGE
  }
  buf_cadd(bf, ']');
  write_atts(bf, imports, value_attachs(v));
}

static void write_map(Buf *bf, Map/*char*/ *imports, Value *v) {
  Arr/*Value*/ *vs = value_values(v);
  buf_cadd(bf, '{');
  if (arr_size(vs)) {
    wval_writebf(bf, imports, arr_get(vs, 0));
    buf_cadd(bf, ':');
    wval_writebf(bf, imports, arr_get(vs, 1));
    RANGE(i, 2, arr_size(vs)) {
      buf_cadd(bf, ',');
      wval_writebf(bf, imports, arr_get(vs, i++));
      buf_cadd(bf, ':');
      wval_writebf(bf, imports, arr_get(vs, i));
    }_RANGE
  }
  buf_cadd(bf, '}');
  write_atts(bf, imports, value_attachs(v));
}

static void write_id(Buf *bf, Map/*char*/ *imports, Value *v) {
  char *id = arr_get(value_ids(v), 0);
  id = map_oget(imports, id, id);
  buf_add(bf, id);
  write_atts(bf, imports, value_attachs(v));
}

static void write_group(Buf *bf, Map/*char*/ *imports, Value *v) {
  buf_cadd(bf, '(');
  wval_writebf(bf, imports, arr_get(value_values(v), 0));
  buf_cadd(bf, ')');
  write_atts(bf, imports, value_attachs(v));
}

static void write_new(Buf *bf, Map/*char*/ *imports, Value *v) {
  buf_add(bf, "new ");
  char *id = arr_get(value_ids(v), 0);
  id = map_oget(imports, id, id);
  buf_add(bf, id);
  Arr/*Value*/ *vs = value_values(v);
  buf_cadd(bf, '(');
  if (arr_size(vs)) {
    wval_writebf(bf, imports, arr_get(vs, 0));
    RANGE(i, 1, arr_size(vs)) {
      buf_cadd(bf, ',');
      wval_writebf(bf, imports, arr_get(vs, i));
    }_RANGE
  }
  buf_cadd(bf, ')');
  write_atts(bf, imports, value_attachs(v));
}

void wval_writebf(Buf *bf, Map/*char*/ *imports, Value *v) {
  switch (value_vtype(v)) {
    case VNULL:
      write_null(bf);
      break;
    case VBOOL:
    case VBYTE:
    case VINT:
    case VFLOAT:
      write_bif(bf, imports, v);
      break;
    case VCHAR:
      write_str(bf, imports, v, '\'');
      break;
    case VSTR:
      write_str(bf, imports, v, '"');
      break;
    case VSTR2:
      write_str(bf, imports, v, '`');
      break;
    case VARR:
      write_arr(bf, imports, v);
      break;
    case VMAP:
      write_map(bf, imports, v);
      break;
    case VFN:
      wfun_writebf(bf, imports, v);
      break;
    case VID:
      write_id(bf, imports, v);
      break;
    case VLUNARY:
      break;
    case VRUNARY:
      break;
    case VBINARY:
      break;
    case VTERNARY:
      break;
    case VWITH:
      break;
    case VGROUP:
      write_group(bf, imports, v);
      break;
    case VNEW:
      write_new(bf, imports, v);
      break;
    default:
      THROW "Unknown Value type %d", value_vtype(v) _THROW
  }
}

void wval_write(LckFile *lck, Map/*char*/ *imports, Value *v) {
}
