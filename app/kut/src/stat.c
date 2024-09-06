// Copyright 05-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "stat.h"
#include "fileix.h"
#include "symix.h"
#include "kut/buf.h"

#undef TRY


enum stat_Stat_t {
  END,
  INDEXED, ASSIGN, ARR_MULTI, DIC_MULTI,
  ADDAS, SUBAS, MULAS, DIVAS, MODAS, ANDAS, ORAS,
  FUNCTION, BLOCK,
  BLOCK_CLOSE, BREAK, CONTINUE,
  TRACE, RETURN,
  TRY, WHILE, IF, FOR, FOR_IX, FOR_R, FOR_RS, SWITCH,
  IMPORT, EXPORT
};

typedef enum stat_Stat_t Stat_t;

struct stat_Stat {
  void *value;
  Stat_t type;
};

static Stat *new(Stat_t type, void *value) {
  Stat *this = MALLOC(Stat);
  this->value = value;
  this->type = type;
  return this;
}

struct stat_StatCode {
  Stat *stat;
  int file_ix;
  int nline;
};

StatCode *stat_code_new(int file_ix, int nline, Stat *stat) {
  StatCode *this = MALLOC(StatCode);
  this->stat = stat;
  this->file_ix = file_ix;
  this->nline = nline;
  return this;
}

int stat_code_file_ix(StatCode *this) {
  return this->file_ix;
}

int stat_code_line(StatCode *this) {
  return this->nline;
}

Stat *stat_code_stat(StatCode *this) {
  return this->stat;
}

static Stat end_stat = { .type = END, .value = "<end>" };
Stat *stat_end (void) {
  return &end_stat;
}

int stat_is_end (Stat *this) {
  return this == &end_stat;
}

Stat *stat_indexed (Arr *syms) {
  if (arr_size(syms) < 2)
    EXC_ILLEGAL_ARGUMENT(
      "Bad elements number of 'syms'", "2", str_f("%d", arr_size(syms))
    );

  return new(INDEXED, syms);
}

Arr *stat_get_indexed (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_indexed, "index", this);
  return this->value;
}

int stat_is_indexed (Stat *this) {
  return this->type == INDEXED;
}

Stat *stat_assign (Exp *left, Exp *right) {
  return new(ASSIGN, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_assign (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_assign, "assign", this);
  return this->value;
}

int stat_is_assign (Stat *this) {
  return this->type == ASSIGN;
}

Stat *stat_arr_multi (Exp *new_sym, Arr *syms, Exp *exp) {
  return new(ARR_MULTI, tp3_new(new_sym, syms, exp));
}

Tp3 *stat_get_arr_multi (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_arr_multi, "arr_multi", this);
  return this->value;
}

int stat_is_arr_multi (Stat *this) {
  return this->type == ARR_MULTI;
}

Stat *stat_dic_multi (Exp *new_sym, Arr *syms, Exp *exp) {
  return new(DIC_MULTI, tp3_new(new_sym, syms, exp));
}

Tp3 *stat_get_dic_multi (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_dic_multi, "dic_multi", this);
  return this->value;
}

int stat_is_dic_multi (Stat *this) {
  return this->type == DIC_MULTI;
}


Stat *stat_add_as (Exp *left, Exp *right) {
  return new(ADDAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_add_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_add_as, "add_as", this);
  return this->value;
}

int stat_is_add_as (Stat *this) {
  return this->type == ADDAS;
}

Stat *stat_sub_as (Exp *left, Exp *right) {
  return new(SUBAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_sub_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_sub_as, "sub_as", this);
  return this->value;
}

int stat_is_sub_as (Stat *this) {
  return this->type == SUBAS;
}

Stat *stat_mul_as (Exp *left, Exp *right) {
  return new(MULAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_mul_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_mul_as, "mul_as", this);
  return this->value;
}

int stat_is_mul_as (Stat *this) {
  return this->type == MULAS;
}

Stat *stat_div_as (Exp *left, Exp *right) {
  return new(DIVAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_div_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_div_as, "div_as", this);
  return this->value;
}

int stat_is_div_as (Stat *this) {
  return this->type == DIVAS;
}

Stat *stat_mod_as (Exp *left, Exp *right) {
  return new(MODAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_mod_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_mod_as, "mod_as", this);
  return this->value;
}

int stat_is_mod_as (Stat *this) {
  return this->type == MODAS;
}

Stat *stat_and_as (Exp *left, Exp *right) {
  return new(ANDAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_and_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_and_as, "and_as", this);
  return this->value;
}

int stat_is_and_as (Stat *this) {
  return this->type == ANDAS;
}

Stat *stat_or_as (Exp *left, Exp *right) {
  return new(ORAS, tp_new(left, right));
}

// <Exp, Exp>
Tp *stat_get_or_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_or_as, "or_as", this);
  return this->value;
}

int stat_is_or_as (Stat *this) {
  return this->type == ORAS;
}

Stat *stat_func (Exp *value) {
  return new(FUNCTION, value);
}

Exp *stat_get_func (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_func, "function", this);
  return this->value;
}

int stat_is_func (Stat *this) {
  return this->type == FUNCTION;
}

// stats is Arr<StatCode>
Stat *stat_block (Arr *stats) {
  return new(BLOCK, stats);
}

// <StatCode>
Arr *stat_get_block (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_block, "block", this);
  return this->value;
}

int stat_is_block (Stat *this) {
  return this->type == BLOCK;
}

Stat *stat_block_close (void) {
  return new(BLOCK_CLOSE, "");
}

int stat_is_block_close (Stat *this) {
  return this->type == BLOCK_CLOSE;
}

Stat *stat_break (void) {
  return new(BREAK, "");
}

int stat_is_break (Stat *this) {
  return this->type == BREAK;
}

Stat *stat_continue (void) {
  return new(CONTINUE, "");
}

int stat_is_continue (Stat *this) {
  return this->type == CONTINUE;
}

Stat *stat_trace (int is_complete, Exp *exp) {
  int *is = ATOMIC(sizeof(int));
  *is = is_complete;
  return new(TRACE, tp_new(is, exp));
}

// <int, Exp>
Tp *stat_get_trace (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_trace, "trace", this);
  return this->value;
}

int stat_is_trace (Stat *this) {
  return this->type == TRACE;
}

Stat *stat_return (Exp *exp) {
  return new(RETURN, exp);
}

Exp *stat_get_return (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_return, "return", this);
  return this->value;
}

int stat_is_return (Stat *this) {
  return this->type == RETURN;
}

// finally is Opt<StatCode>
Stat *stat_try (StatCode *try, int catch_var, StatCode *catch, Opt *finally) {
  int *var = ATOMIC(sizeof(int));
  *var = catch_var;
  return new(TRY, arr_new_from(try, var, catch, finally, NULL));
}

// [<StatCode>, <int>, <StatCode>, <Opt<StatCode>>]
Arr *stat_get_try (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_try, "try", this);
  return this->value;
}

int stat_is_try (Stat *this) {
  return this->type == TRY;
}

Stat *stat_while (Exp *cond, StatCode *stat) {
  return new(WHILE, arr_new_from(cond, stat, NULL));
}

Arr *stat_get_while (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_while, "while", this);
  return this->value;
}

int stat_is_while (Stat *this) {
  return this->type == WHILE;
}

// 'else_stat' is Opt<StatCode>
Stat *stat_if (Exp *cond, StatCode *if_stat, Opt *else_stat) {
  return new(IF, arr_new_from(cond, if_stat, else_stat, NULL));
}

// [<Exp>, <StatCode>, Opt<StatCode>]
Arr *stat_get_if (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_if, "if", this);
  return this->value;
}

int stat_is_if (Stat *this) {
  return this->type == IF;
}

Stat *stat_for (int var, Exp *collection, StatCode *stat) {
  int *v = ATOMIC(sizeof(int));
  *v = var;
  return new(FOR, arr_new_from(v, collection, stat, NULL));
}

// [<int>, <Exp>, <StatCode>]
Arr *stat_get_for (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for, "for", this);
  return this->value;
}

int stat_is_for (Stat *this) {
  return this->type == FOR;
}

Stat *stat_for_ix (int var_e, int var_ix,  Exp *collection, StatCode *stat) {
  int *v1 = ATOMIC(sizeof(int));
  *v1 = var_e;
  int *v2 = ATOMIC(sizeof(int));
  *v2 = var_ix;
  return new(FOR_IX, arr_new_from(v1, v2, collection, stat, NULL));
}

// [<int>, <int>, <Exp>, <StatCode>]
Arr *stat_get_for_ix (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for_ix, "for_ix", this);
  return this->value;
}

int stat_is_for_ix (Stat *this) {
  return this->type == FOR_IX;
}

Stat *stat_for_r (int var, Exp *start, Exp *end, StatCode *stat) {
  int *v = ATOMIC(sizeof(int));
  *v = var;
  return new(FOR_R, arr_new_from(v, start, end, stat, NULL));
}

// [<int>, <Exp>, <Exp>, <StatCode>]
Arr *stat_get_for_r (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for_r, "for_r", this);
  return this->value;
}

int stat_is_for_r (Stat *this) {
  return this->type == FOR_R;
}

Stat *stat_for_rs (int var, Exp *start, Exp *end, Exp *step, StatCode *stat) {
  int *v = ATOMIC(sizeof(int));
  *v = var;
  return new(FOR_RS, arr_new_from(v, start, end, step, stat, NULL));
}

// [<int>, <Exp>, <Exp>, <Exp>, <StatCode>]
Arr *stat_get_for_rs (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for_rs, "for_rs", this);
  return this->value;
}

int stat_is_for_rs (Stat *this) {
  return this->type == FOR_RS;
}

// 'entries' is Arr<Tp<Arr<Exp>, StatCode>>
Stat *stat_switch (Exp *cond, Arr *entries) {
  return new(SWITCH, arr_new_from(cond, entries, NULL));
}

// [<Exp>, Arr<Tp<Arr<Exp>, StatCode>>]
Arr *stat_get_switch (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_switch, "switch", this);
  return this->value;
}

int stat_is_switch (Stat *this) {
  return this->type == SWITCH;
}

Stat *stat_import (char *module_path, int id) {
  int *id2 = ATOMIC(sizeof(int *));
  *id2 = id;
  return new(IMPORT, arr_new_from(module_path, id2, NULL));
}

// [<char>, <int>]
Arr *stat_get_import (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_import, "import", this);
  return this->value;
}

int stat_is_import (Stat *this) {
  return this->type == IMPORT;
}

static Stat export_stat = { .type = EXPORT, .value = "<export>" };
Stat *stat_export (void) {
  return &export_stat;
}

int stat_is_export (Stat *this) {
  return this == &export_stat;
}

char *stat_type_to_str (Stat *this)  {
  switch (this->type) {
    case END: return "eof";
    case INDEXED: return "indexed";
    case ASSIGN: return "assign";
    case ARR_MULTI: return "arr_multi";
    case DIC_MULTI: return "dic_multi";
    case ADDAS: return "add_as";
    case SUBAS: return "sub_as";
    case MULAS: return "mul_as";
    case DIVAS: return "div_as";
    case MODAS: return "mod_as";
    case ANDAS: return "and_as";
    case ORAS: return "or_as";
    case FUNCTION: return "function";
    case BLOCK: return "block";
    case BLOCK_CLOSE: return "block_close";
    case BREAK: return "break";
    case CONTINUE: return "continue";
    case TRACE: return "trace";
    case RETURN: return "return";
    case TRY: return "try";
    case WHILE: return "while";
    case IF: return "if";
    case FOR: return "for";
    case FOR_IX: return "for_ix";
    case FOR_R: return "for_r";
    case FOR_RS: return "for_rs";
    case SWITCH: return "switch";
    case IMPORT: return "import";
    case EXPORT: return "export";
  }
  EXC_ILLEGAL_ARGUMENT("Bad statement type identifier",
    str_f("(0 to %d)", EXPORT), str_f("%d", this->type)
  );
  return NULL;
}

char *stat_to_str (Stat *this) {
    //--
    // tp is Tp<Arr<Exp>, Stat>
    char *fn_switch(Tp *tp) {
      /// Exp
      Arr *conds = tp_e1(tp);
        //--
        char *fn_map(Exp *exp) { return exp_to_js(exp); }
      char *sconds = arr_join(arr_map(conds, (FMAP)fn_map), ", ");
      return str_f("%s : %s", sconds, stat_to_str(tp_e2(tp)));
    }

  switch (this->type) {
    case END: return "<eof>";
    case INDEXED: {
      Exp **syms = (Exp**)arr_begin(this->value);
      Buf *bf = buf_new();
      buf_add(bf, exp_to_js(syms[0]));
      buf_add(bf, " : ");
      int i;
      for (i = 1; i < arr_size((Arr *)this->value) - 1; ++i) {
        buf_add(bf, exp_to_js(syms[i]));
        buf_add(bf, ", ");
      }
      buf_add(bf, exp_to_js(syms[i]));
      buf_cadd(bf, ';');
      return buf_str(bf);
    }
    case ASSIGN: {
      Tp *v = stat_get_assign(this);
      return str_f("%s = %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case ARR_MULTI: {
      Tp3 *v = stat_get_arr_multi(this);
      Arr *syms = tp3_e2(v);
      Buf *bf = buf_new();
      for (int i = 0; i < arr_size(syms); ++i) {
        if (i > 0) buf_add(bf, ", ");
        buf_add(bf, symix_get(exp_get_sym(arr_get(syms, i))));
      }
      return str_f("%s = %s;",
        buf_str(bf), exp_to_js(tp3_e3(v))
      );
    }
    case DIC_MULTI: {
      Tp3 *v = stat_get_dic_multi(this);
      Arr *syms = tp3_e2(v);
      Buf *bf = buf_new();
      for (int i = 0; i < arr_size(syms); ++i) {
        if (i > 0) buf_add(bf, ", ");
        buf_add(bf, symix_get(exp_get_sym(arr_get(syms, i))));
      }
      return str_f("%s = %s;",
        buf_str(bf), exp_to_js(tp3_e3(v))
      );
    }
    case ADDAS: {
      Tp *v = stat_get_add_as(this);
      return str_f("%s += %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case SUBAS: {
      Tp *v = stat_get_sub_as(this);
      return str_f("%s -= %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case MULAS: {
      Tp *v = stat_get_mul_as(this);
      return str_f("%s *= %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case DIVAS: {
      Tp *v = stat_get_div_as(this);
      return str_f("%s /= %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case MODAS: {
      Tp *v = stat_get_mod_as(this);
      return str_f("%s %= %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case ANDAS: {
      Tp *v = stat_get_and_as(this);
      return str_f("%s &= %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case ORAS: {
      Tp *v = stat_get_or_as(this);
      return str_f("%s |= %s;",
        exp_to_js(tp_e1(v)), exp_to_js(tp_e2(v))
      );
    }
    case FUNCTION: return str_f("%s;", exp_to_js(stat_get_func(this)));
    case BLOCK: return str_f(
      "{%s}",
      arr_join(arr_map(
        arr_map(stat_get_block(this), (FMAP)stat_code_stat),
        (FMAP)stat_to_str
      ), "")
    );
    case BLOCK_CLOSE: return "}";
    case BREAK: return "break;";
    case CONTINUE: return "continue;";
    case TRACE: {
      Tp *v = stat_get_trace(this);
      return str_f("trace %s%s;",
        *((int *)tp_e1(v)) ? ":" : "", exp_to_js(tp_e2(v))
      );
    }
    case RETURN: {
      Exp *e = stat_get_return(this);
      if (exp_is_empty(e)) return "return;";
      else return str_f("return %s;", exp_to_js(stat_get_return(this)));
    }
    case TRY: {
      Arr *v = stat_get_try(this);
      StatCode *finally = opt_get(arr_get(v, 3));
      if (finally)
        return str_f("try %s\ncatch(%s) %s\nfinally %s",
          stat_to_str(stat_code_stat(arr_get(v, 0))),
          symix_get(*((int *)arr_get(v, 1))),
          stat_to_str(stat_code_stat(arr_get(v,2))),
          stat_to_str(stat_code_stat(finally))
        );
      else
        return str_f("try %s\ncatch(%s) %s",
          stat_to_str(stat_code_stat(arr_get(v, 0))),
          symix_get(*((int *)arr_get(v, 1))),
          stat_to_str(stat_code_stat(arr_get(v,2)))
        );
    }
    case WHILE: {
      Arr *v = stat_get_while(this);
      Exp *cond = arr_get(v, 0);
      return str_f("while (%s) %s",
        exp_is_empty(cond) ? "" : exp_to_js(cond),
        stat_to_str(stat_code_stat(arr_get(v, 1)))
      );
    }
    case IF: {
      Arr *v = stat_get_if(this);
      StatCode *else_st = opt_get(arr_get(v, 2));
      if (else_st)
        return str_f("if (%s) %s\nelse %s",
          exp_to_js(arr_get(v, 0)),
          stat_to_str(stat_code_stat(arr_get(v, 1))),
          stat_to_str(stat_code_stat(else_st))
        );
      else
        return str_f("if (%s) %s",
          exp_to_js(arr_get(v, 0)),
          stat_to_str(stat_code_stat(arr_get(v, 1)))
        );

    }
    case FOR: {
      Arr *v = stat_get_for(this);
      return str_f("for (%s = %s) %s",
        symix_get(*((int *)arr_get(v, 0))),
        exp_to_js(arr_get(v, 1)),
        stat_to_str(stat_code_stat(arr_get(v, 2)))
      );
    }
    case FOR_IX: {
      Arr *v = stat_get_for_ix(this);
      return str_f("for (%s, %s = %s) %s",
        symix_get(*((int *)arr_get(v, 0))),
        symix_get(*((int *)arr_get(v, 1))),
        exp_to_js(arr_get(v, 2)),
        stat_to_str(stat_code_stat(arr_get(v, 3)))
      );
    }
    case FOR_R: {
      Arr *v = stat_get_for_r(this);
      return str_f("for (%s = %s : %s) %s",
        symix_get(*((int *)arr_get(v, 0))),
        exp_to_js(arr_get(v, 1)),
        exp_to_js(arr_get(v, 2)),
        stat_to_str(stat_code_stat(arr_get(v, 3)))
      );
    }
    case FOR_RS: {
      Arr *v = stat_get_for_rs(this);
      return str_f("for (%s = %s : %s : %s) %s",
        symix_get(*((int *)arr_get(v, 0))),
        exp_to_js(arr_get(v, 1)),
        exp_to_js(arr_get(v, 2)),
        exp_to_js(arr_get(v, 3)),
        stat_to_str(stat_code_stat(arr_get(v, 4)))
      );
    }
    case SWITCH: {
      Arr *v = stat_get_switch(this);
      return str_f("switch (%s) {\n%s\n}",
        exp_to_js(arr_get(v, 0)),
        arr_join(arr_map(arr_get(v, 1), (FMAP)fn_switch), "\n")
      );
    }
    case IMPORT: {
      Arr *v = stat_get_import(this);
      char *file = arr_get(v, 0);
      char *alias = symix_get(*((int *)arr_get(v, 1)));
      if (*alias) return str_f("import \"%s\" : %s;", file, alias);
      else return str_f("import \"%s\";", file);
    }
    case EXPORT: return "<export>";
  }
  EXC_ILLEGAL_ARGUMENT("Bad statement type identifier",
    str_f("(0 to %d)", EXPORT), str_f("%d", this->type)
  );
  return NULL;
}
