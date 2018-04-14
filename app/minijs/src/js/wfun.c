// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/wfun.h"
#include "ast/Stat.h"
#include "js/wval.h"

static void write_stats(Buf *bf, Map/*char*/ *imports, Arr/*Stat*/ *stats);

static void write_assign (Buf *bf, Map/*char*/ *imports, Stat *st) {
  Arr/*Value*/ *values = stat_values(st);
  wval_writebf(bf, imports, arr_get(values, 0));
  buf_add(bf, " = ");
  wval_writebf(bf, imports, arr_get(values, 1));
}

static void write_va(Buf *bf, Map/*char*/ *imports, Stat *st) {
  Dvalue *dv = arr_get(stat_dvalues(st), 0);
  buf_add(bf, "let ");
  buf_add(bf, dvalue_id(dv));
  buf_add(bf, " = ");
  wval_writebf(bf, imports, dvalue_value(dv));
}

static void write_stat(Buf *bf, Map/*char*/ *imports, Stat *st) {
  switch (stat_stype(st)) {
    case SVAL:
    case SVAR:
      write_va(bf, imports, st);
      break;
    case SFN:
      break;
    case SASSIGN:
      write_assign(bf, imports, st);
      break;
    case SRETURN:
      break;
    case SBREAK:
      break;
    case SCONTINUE:
      break;
    case SBLOCK:
      break;
    case SWHILE:
      break;
    case SDO:
      break;
    case SFOR0:
      break;
    case SFOR:
      break;
    case SFOR_EACH:
      break;
    case SIF:
      break;
    case STRY:
      break;
    case STHROW:
      break;
    case SNATIVE:
      break;
    default:
      THROW "Unknown Stat type %d", stat_stype(st) _THROW
  }
  buf_add(bf, ";\n");
}

static void write_stats(Buf *bf, Map/*char*/ *imports, Arr/*Stat*/ *stats) {
  buf_add(bf, "{\n");

  EACH(stats, Stat, st) {
    write_stat(bf, imports, st);
  }_EACH

  buf_add(bf, "}");
}

void wfun_writebf(Buf *bf, Map/*char*/ *imports, Value *v) {
  buf_add(bf, "function (");
  Arr/*char*/ *params = value_ids(v);
  if (arr_size(params)) {
    buf_add(bf, arr_get(params, 0));
    RANGE(i, 1, arr_size(params)) {
      buf_add(bf, ", ");
      buf_add(bf, arr_get(params, i));
    }_RANGE
  }
  buf_add(bf, ") ");

  write_stats(bf, imports, value_stats(v));
}

void wfun_write(LckFile *lck, Map/*char*/ *imports, Value *v) {
  Buf *bf = buf_new();
  wfun_writebf(bf, imports, v);
  file_write_text(lck, buf_str(bf));
}
