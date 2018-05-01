// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/wfun.h"
#include "ast/Stat.h"
#include "js/wval.h"

static void write_stats(Buf *bf, Map/*char*/ *imports, Arr/*Stat*/ *stats);

static void write_va(Buf *bf, Map/*char*/ *imports, Stat *st) {
  Dvalue *dv = arr_get(stat_dvalues(st), 0);
  buf_add(bf, "let ");
  buf_add(bf, dvalue_id(dv));
  buf_add(bf, " = ");
  wval_writebf(bf, imports, dvalue_value(dv));
}

static void write_fn(Buf *bf, Map/*char*/ *imports, Stat *st) {
  wval_writebf(bf, imports, arr_get(stat_values(st), 0));
}

static void write_assign (Buf *bf, Map/*char*/ *imports, Stat *st) {
  Arr/*Value*/ *values = stat_values(st);
  wval_writebf(bf, imports, arr_get(values, 0));
  buf_add(bf, arr_get(stat_ids(st), 0));
  wval_writebf(bf, imports, arr_get(values, 1));
}

static void write_return(Buf *bf, Map/*char*/ *imports, Stat *st) {
  buf_add(bf, "return ");
  Arr/*Value*/ *values = stat_values(st);
  if (arr_size(values)) {
    wval_writebf(bf, imports, arr_get(values, 0));
  }
}

static void write_while(Buf *bf, Map/*char*/ *imports, Stat *st) {
  buf_add(bf, "while ");
  wval_writebf(bf, imports, arr_get(stat_values(st), 0));
  write_stats(bf, imports, arr_get(stat_blocks(st), 0));
}

static void write_do(Buf *bf, Map/*char*/ *imports, Stat *st) {
  buf_add(bf, "do ");
  write_stats(bf, imports, arr_get(stat_blocks(st), 0));
  buf_add(bf, " while ");
  wval_writebf(bf, imports, arr_get(stat_values(st), 0));
}

static void write_for0(Buf *bf, Map/*char*/ *imports, Stat *st) {
  char *var = arr_get(stat_ids(st), 0);
  Arr/*Value*/ *values = stat_values(st);
  Buf *v1 = buf_new();
  wval_writebf(v1, imports, arr_get(values, 0));
  if (arr_size(values) == 1) {
    buf_add(bf, str_printf("for (let %s = 0; %s < %s; ++%s) ",
      var, var, buf_str(v1), var));
  } else {
    Buf *v2 = buf_new();
    wval_writebf(v2, imports, arr_get(values, 1));
    buf_add(bf, str_printf("for (let %s = 0; %s < %s; %s += %s) ",
      var, var, buf_str(v1), var, buf_str(v2)));
  }
  write_stats(bf, imports, arr_get(stat_blocks(st), 0));
}

static void write_for(Buf *bf, Map/*char*/ *imports, Stat *st) {
  char *var = arr_get(stat_ids(st), 0);
  Arr/*Value*/ *values = stat_values(st);
  Buf *v1 = buf_new();
  wval_writebf(v1, imports, arr_get(values, 0));
  Buf *v2 = buf_new();
  wval_writebf(v2, imports, arr_get(values, 1));
  if (arr_size(values) == 2) {
    buf_add(bf, str_printf("for (let %s = %s; %s < %s; ++%s) ",
      var, buf_str(v1), var, buf_str(v2), var));
  } else {
    Buf *v3 = buf_new();
    wval_writebf(v3, imports, arr_get(values, 2));
    buf_add(bf, str_printf("for (let %s = %s; %s < %s; %s += %s) ",
      var, buf_str(v1), var, buf_str(v2), var, buf_str(v3)));
  }
  write_stats(bf, imports, arr_get(stat_blocks(st), 0));
}

static void write_for_each(Buf *bf, Map/*char*/ *imports, Stat *st) {
  char *var = arr_get(stat_ids(st), 0);
  Value *v = arr_get(stat_values(st), 0);
  enum Type_t tp = type_type(value_type(v));
  Buf *v1 = buf_new();
  wval_writebf(v1, imports, v);

  if (tp == MAP) {
    buf_add(bf, str_printf("for(let %s in %s) ", var, buf_str(v1)));
    write_stats(bf, imports, arr_get(stat_blocks(st), 0));
  } else {
    Buf *stsb = buf_new();
    write_stats(stsb, imports, arr_get(stat_blocks(st), 0));
    char *sts;
    if (tp == ARR) {
      sts = str_printf("{\nlet %s=_ob[_i];%s\n}",
        var, str_sub_end(buf_str(stsb), 1)
      );
    } else {
      sts = str_printf("{\nlet %s=_ob.charAt(_i);%s\n}",
        var, str_sub_end(buf_str(stsb), 1)
      );
    }
    buf_add(bf, str_printf(
      "{\nlet _ob = %s;\nfor(let _i = 0; _i < _ob.length; ++_i) %s",
      buf_str(v1), sts
    ));
  }
}

static void write_if(Buf *bf, Map/*char*/ *imports, Stat *st) {
  Arr/*Value*/ *vs = stat_values(st);
  Arr/*Arr[Stat]*/ *bls = stat_blocks(st);

  buf_add(bf, "if ");
  wval_writebf(bf, imports, arr_get(vs, 0));
  write_stats(bf, imports, arr_get(bls, 0));
  int i = 1;
  for (; i < arr_size(vs); ++i) {
    buf_add(bf, " else if ");
    wval_writebf(bf, imports, arr_get(vs, i));
    write_stats(bf, imports, arr_get(bls, i));
  }

  for (; i < arr_size(bls); ++i) {
    buf_add(bf, " else ");
    write_stats(bf, imports, arr_get(bls, i));
  }
}

static void write_try(Buf *bf, Map/*char*/ *imports, Stat *st) {
  Arr/*Arr[Stat]*/ *bls = stat_blocks(st);

  buf_add(bf, "try ");
  write_stats(bf, imports, arr_get(bls, 0));
  buf_add(bf, " catch (");
  buf_add(bf, arr_get(stat_ids(st), 0));
  buf_add(bf, ") ");
  write_stats(bf, imports, arr_get(bls, 1));
  if (arr_size(bls) == 3) {
    buf_add(bf, " finally ");
    write_stats(bf, imports, arr_get(bls, 2));
  }
}

static void write_throw(Buf *bf, Map/*char*/ *imports, Stat *st) {
  buf_add(bf, "throw ");
  wval_writebf(bf, imports, arr_get(stat_values(st), 0));
}

static void write_native(Buf *bf, Map/*char*/ *imports, Stat *st) {
  buf_add(bf, arr_get(stat_ids(st), 0));
}

static void write_stat(Buf *bf, Map/*char*/ *imports, Stat *st) {
  switch (stat_stype(st)) {
    case SVAL:
    case SVAR:
      write_va(bf, imports, st);
      break;
    case SFN:
    case SINC:
      write_fn(bf, imports, st);
      break;
    case SASSIGN:
      write_assign(bf, imports, st);
      break;
    case SRETURN:
      write_return(bf, imports, st);
      break;
    case SBREAK:
      buf_add(bf, "break");
      break;
    case SCONTINUE:
      buf_add(bf, "continue");
      break;
    case SBLOCK:
      write_stats(bf, imports, arr_get(stat_blocks(st), 0));
      break;
    case SWHILE:
      write_while(bf, imports, st);
      break;
    case SDO:
      write_do(bf, imports, st);
      break;
    case SFOR0:
      write_for0(bf, imports, st);
      break;
    case SFOR:
      write_for(bf, imports, st);
      break;
    case SFOR_EACH:
      write_for_each(bf, imports, st);
      break;
    case SIF:
      write_if(bf, imports, st);
      break;
    case STRY:
      write_try(bf, imports, st);
      break;
    case STHROW:
      write_throw(bf, imports, st);
      break;
    case SNATIVE:
      write_native(bf, imports, st);
      return;
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
