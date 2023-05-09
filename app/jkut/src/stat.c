// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "stat.h"
#include "kut/js.h"
#include "DEFS.h"
#include "fileix.h"

#undef TRY


enum stat_Stat_t {
  END, DO, AWAIT,
  ASSIGN, ADDAS, SUBAS, MULAS, DIVAS, MODAS, ANDAS, ORAS,
  FUNCTION, BLOCK,
  BLOCK_CLOSE, BREAK, CONTINUE,
  TRACE, TTHROW, RETURN,
  TRY, WHILE, IF, FOR, FOR_IX, FOR_R, FOR_RS, SWITCH,
  IMPORT, EXPORT
};

typedef enum stat_Stat_t Stat_t;

struct stat_Stat {
  Stat_t type;
  void *value;
  char *js;
};

static Stat *new(Stat_t type, void *value, char *js) {
  Stat *this = MALLOC(Stat);
  this->type = type;
  this->value = value;
  this->js = js;
  return this;
}

struct stat_StatCode {
  int file_ix;
  int nline;
  Stat *stat;
};

StatCode *stat_code_new(int file_ix, int nline, Stat *stat) {
  StatCode *this = MALLOC(StatCode);
  this->file_ix = file_ix;
  this->nline = nline;
  this->stat = stat;
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

char *stat_get_js(Stat *this) {
  return this->js;
}

void stat_set_js (Stat *this, char *js) {
  this->js = js;
}

static Stat end_stat = { .type = END, .value = "<end>", .js = "" };
Stat *stat_end (void) {
  return &end_stat;
}

int stat_is_end (Stat *this) {
  return this == &end_stat;
}

Stat *stat_do (char *js_inject, char *js) {
  return new(DO, js_inject, js);
}

// <Exp, Exp>
char *stat_get_do (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_do, "do", this);
  return this->value;
}

int stat_is_do (Stat *this) {
  return this->type == DO;
}

Stat *stat_await (Exp *fn_call, char *js) {
  return new(AWAIT, fn_call, js);
}

Exp *stat_get_await (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_await, "await", this);
  return this->value;
}

int stat_is_await (Stat *this) {
  return this->type == AWAIT;
}

Stat *stat_assign (Exp *left, Exp *right, char *js) {
  return new(ASSIGN, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_assign (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_assign, "assign", this);
  return this->value;
}

int stat_is_assign (Stat *this) {
  return this->type == ASSIGN;
}

Stat *stat_add_as (Exp *left, Exp *right, char *js) {
  return new(ADDAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_add_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_add_as, "add_as", this);
  return this->value;
}

int stat_is_add_as (Stat *this) {
  return this->type == ADDAS;
}

Stat *stat_sub_as (Exp *left, Exp *right, char *js) {
  return new(SUBAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_sub_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_sub_as, "sub_as", this);
  return this->value;
}

int stat_is_sub_as (Stat *this) {
  return this->type == SUBAS;
}

Stat *stat_mul_as (Exp *left, Exp *right, char *js) {
  return new(MULAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_mul_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_mul_as, "mul_as", this);
  return this->value;
}

int stat_is_mul_as (Stat *this) {
  return this->type == MULAS;
}

Stat *stat_div_as (Exp *left, Exp *right, char *js) {
  return new(DIVAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_div_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_div_as, "div_as", this);
  return this->value;
}

int stat_is_div_as (Stat *this) {
  return this->type == DIVAS;
}

Stat *stat_mod_as (Exp *left, Exp *right, char *js) {
  return new(MODAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_mod_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_mod_as, "mod_as", this);
  return this->value;
}

int stat_is_mod_as (Stat *this) {
  return this->type == MODAS;
}

Stat *stat_and_as (Exp *left, Exp *right, char *js) {
  return new(ANDAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_and_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_and_as, "and_as", this);
  return this->value;
}

int stat_is_and_as (Stat *this) {
  return this->type == ANDAS;
}

Stat *stat_or_as (Exp *left, Exp *right, char *js) {
  return new(ORAS, tp_new(left, right), js);
}

// <Exp, Exp>
Tp *stat_get_or_as (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_or_as, "or_as", this);
  return this->value;
}

int stat_is_or_as (Stat *this) {
  return this->type == ORAS;
}

Stat *stat_func (Exp *value, char *js) {
  return new(FUNCTION, value, js);
}

Exp *stat_get_func (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_func, "function", this);
  return this->value;
}

int stat_is_func (Stat *this) {
  return this->type == FUNCTION;
}


// stats is Arr<StatCode>
Stat *stat_block (Arr *stats, char *js) {
  return new(BLOCK, stats, js);
}

// <StatCode>
Arr *stat_get_block (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_block, "block", this);
  return this->value;
}

int stat_is_block (Stat *this) {
  return this->type == BLOCK;
}

Stat *stat_block_close (char *js) {
  return new(BLOCK_CLOSE, "", js);
}

int stat_is_block_close (Stat *this) {
  return this->type == BLOCK_CLOSE;
}

Stat *stat_break (char *js) {
  return new(BREAK, "", js);
}

int stat_is_break (Stat *this) {
  return this->type == BREAK;
}

Stat *stat_continue (char *js) {
  return new(CONTINUE, "", js);
}

int stat_is_continue (Stat *this) {
  return this->type == CONTINUE;
}

Stat *stat_trace (Exp *exp, char *js) {
  return new(TRACE, exp, js);
}

Exp *stat_get_trace (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_trace, "trace", this);
  return this->value;
}

int stat_is_trace (Stat *this) {
  return this->type == TRACE;
}

Stat *stat_throw (Exp *exp, char *js) {
  return new(TTHROW, exp, js);
}

Exp *stat_get_throw (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_throw, "throw", this);
  return this->value;
}

int stat_is_throw (Stat *this) {
  return this->type == TTHROW;
}

// exp is Opt<Exp>
Stat *stat_return (Opt *exp, char *js) {
  return new(RETURN, exp, js);
}

// Opt<Exp>
Opt *stat_get_return (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_return, "return", this);
  return this->value;
}

int stat_is_return (Stat *this) {
  return this->type == RETURN;
}

// finally is Opt<StatCode>
Stat *stat_try (
  StatCode *try, char *catch_var, StatCode *catch, Opt *finally, char *js
) {
  return new(TRY, arr_new_from(try, catch_var, catch, finally, NULL), js);
}

// [<StatCode>, <int>, <StatCode>, <Opt<StatCode>>]
Arr *stat_get_try (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_try, "try", this);
  return this->value;
}

int stat_is_try (Stat *this) {
  return this->type == TRY;
}

// cond is Opt<Exp>
Stat *stat_while (Opt *cond, StatCode *stat, char *js) {
  return new(WHILE, arr_new_from(cond, stat, NULL), js);
}

// Arr[Opt<Exp>, <StatCode>]
Arr *stat_get_while (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_while, "while", this);
  return this->value;
}

int stat_is_while (Stat *this) {
  return this->type == WHILE;
}

// 'else_stat' is Opt<StatCode>
Stat *stat_if (Exp *cond, StatCode *if_stat, Opt *else_stat, char *js) {
  return new(IF, arr_new_from(cond, if_stat, else_stat, NULL), js);
}

// [<Exp>, <StatCode>, Opt<StatCode>]
Arr *stat_get_if (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_if, "if", this);
  return this->value;
}

int stat_is_if (Stat *this) {
  return this->type == IF;
}

Stat *stat_for (char *var, Exp *collection, StatCode *stat, char *js) {
  return new(FOR, arr_new_from(var, collection, stat, NULL), js);
}

// [<char>, <Exp>, <StatCode>]
Arr *stat_get_for (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for, "for", this);
  return this->value;
}

int stat_is_for (Stat *this) {
  return this->type == FOR;
}

Stat *stat_for_ix (
  char *var_e, char *var_ix,  Exp *collection, StatCode *stat, char *js
) {
  return new(FOR_IX, arr_new_from(var_e, var_ix, collection, stat, NULL), js);
}

// [<char>, <char>, <Exp>, <StatCode>]
Arr *stat_get_for_ix (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for_ix, "for_ix", this);
  return this->value;
}

int stat_is_for_ix (Stat *this) {
  return this->type == FOR_IX;
}

Stat *stat_for_r (char *var, Exp *start, Exp *end, StatCode *stat, char *js) {
  return new(FOR_R, arr_new_from(var, start, end, stat, NULL), js);
}

// [<char>, <Exp>, <Exp>, <StatCode>]
Arr *stat_get_for_r (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for_r, "for_r", this);
  return this->value;
}

int stat_is_for_r (Stat *this) {
  return this->type == FOR_R;
}

Stat *stat_for_rs (
  char *var, Exp *start, Exp *end, Exp *step, StatCode *stat, char *js
) {
  return new(FOR_RS, arr_new_from(var, start, end, step, stat, NULL), js);
}

// [<char>, <Exp>, <Exp>, <Exp>, <StatCode>]
Arr *stat_get_for_rs (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_for_rs, "for_rs", this);
  return this->value;
}

int stat_is_for_rs (Stat *this) {
  return this->type == FOR_RS;
}

// 'entries' is Arr<Tp<Exp, StatCode>>
Stat *stat_switch (Exp *cond, Arr *entries, char *js) {
  return new(SWITCH, arr_new_from(cond, entries, NULL), js);
}

// [<Exp>, Arr<Tp<Exp, StatCode>>]
Arr *stat_get_switch (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_switch, "switch", this);
  return this->value;
}

int stat_is_switch (Stat *this) {
  return this->type == SWITCH;
}

Stat *stat_import (char *module_path, char *id, char *js) {
  return new(IMPORT, arr_new_from(module_path, id, NULL), js);
}

// [<char>, <char>]
Arr *stat_get_import (Stat *this) {
  TEST_STAT_TYPE_ERROR(stat_is_import, "import", this);
  return this->value;
}

int stat_is_import (Stat *this) {
  return this->type == IMPORT;
}

Stat *stat_export (char *js) {
  return new(EXPORT, "<export>", js);
}

int stat_is_export (Stat *this) {
  return this->type == EXPORT;
}

char *stat_type_to_str (Stat *this)  {
  switch (this->type) {
    case END: return "eof";
    case DO: return "do";
    case AWAIT: return "await";
    case ASSIGN: return "assign";
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
    case TTHROW: return "throw";
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
    char *fn_switch(Tp *tp) {
      return str_f("%s : %s", exp_to_str(tp_e1(tp)), stat_to_str(tp_e2(tp)));
    }

  switch (this->type) {
    case END: return "<eof>";
    case DO: return str_f("do %s", js_ws(this->value));
    case AWAIT: return str_f("await %s", exp_to_str(this->value));
    case ASSIGN: {
      Tp *v = stat_get_assign(this);
      return str_f("%s = %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case ADDAS: {
      Tp *v = stat_get_add_as(this);
      return str_f("%s += %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case SUBAS: {
      Tp *v = stat_get_sub_as(this);
      return str_f("%s -= %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case MULAS: {
      Tp *v = stat_get_mul_as(this);
      return str_f("%s *= %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case DIVAS: {
      Tp *v = stat_get_div_as(this);
      return str_f("%s /= %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case MODAS: {
      Tp *v = stat_get_mod_as(this);
      return str_f("%s %= %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case ANDAS: {
      Tp *v = stat_get_and_as(this);
      return str_f("%s &= %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case ORAS: {
      Tp *v = stat_get_or_as(this);
      return str_f("%s |= %s;",
        exp_to_str(tp_e1(v)), exp_to_str(tp_e2(v))
      );
    }
    case FUNCTION: return str_f("%s;", exp_to_str(stat_get_func(this)));
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
    case TRACE:
      return str_f("trace %s;", exp_to_str(stat_get_trace(this)));
    case TTHROW:
      return str_f("throw %s;", exp_to_str(stat_get_throw(this)));
    case RETURN: {
      Exp *e = opt_get(stat_get_return(this));
      return e ? str_f("return %s;", exp_to_str(e)): "return;";
    }
    case TRY: {
      Arr *v = stat_get_try(this);
      StatCode *finally = opt_get(arr_get(v, 3));
      if (finally)
        return str_f("try %s\ncatch(%s) %s\nfinally %s",
          stat_to_str(stat_code_stat(arr_get(v, 0))),
          arr_get(v, 1),
          stat_to_str(stat_code_stat(arr_get(v,2))),
          stat_to_str(stat_code_stat(finally))
        );
      else
        return str_f("try %s\ncatch(%s) %s",
          stat_to_str(stat_code_stat(arr_get(v, 0))),
          arr_get(v, 1),
          stat_to_str(stat_code_stat(arr_get(v,2)))
        );
    }
    case WHILE: {
      Arr *v = stat_get_while(this);
      Exp *cond = opt_get(arr_get(v, 0));
      return str_f("while (%s) %s",
        cond ? exp_to_str(cond) : "",
        stat_to_str(stat_code_stat(arr_get(v, 1)))
      );
    }
    case IF: {
      Arr *v = stat_get_if(this);
      StatCode *else_st = opt_get(arr_get(v, 2));
      if (else_st)
        return str_f("if (%s) %s\nelse %s",
          exp_to_str(arr_get(v, 0)),
          stat_to_str(stat_code_stat(arr_get(v, 1))),
          stat_to_str(stat_code_stat(else_st))
        );
      else
        return str_f("if (%s) %s",
          exp_to_str(arr_get(v, 0)),
          stat_to_str(stat_code_stat(arr_get(v, 1)))
        );

    }
    case FOR: {
      Arr *v = stat_get_for(this);
      return str_f("for (%s = %s) %s",
        arr_get(v, 0),
        exp_to_str(arr_get(v, 1)),
        stat_to_str(stat_code_stat(arr_get(v, 2)))
      );
    }
    case FOR_IX: {
      Arr *v = stat_get_for_ix(this);
      return str_f("for (%s, %s = %s) %s",
        arr_get(v, 0),
        arr_get(v, 1),
        exp_to_str(arr_get(v, 2)),
        stat_to_str(stat_code_stat(arr_get(v, 3)))
      );
    }
    case FOR_R: {
      Arr *v = stat_get_for_r(this);
      return str_f("for (%s = %s : %s) %s",
        arr_get(v, 0),
        exp_to_str(arr_get(v, 1)),
        exp_to_str(arr_get(v, 2)),
        stat_to_str(stat_code_stat(arr_get(v, 3)))
      );
    }
    case FOR_RS: {
      Arr *v = stat_get_for_rs(this);
      return str_f("for (%s = %s : %s : %s) %s",
        arr_get(v, 0),
        exp_to_str(arr_get(v, 1)),
        exp_to_str(arr_get(v, 2)),
        exp_to_str(arr_get(v, 3)),
        stat_to_str(stat_code_stat(arr_get(v, 4)))
      );
    }
    case SWITCH: {
      Arr *v = stat_get_switch(this);
      return str_f("switch (%s) {\n%s\n}",
        exp_to_str(arr_get(v, 0)),
        arr_join(arr_map(arr_get(v, 1), (FMAP)fn_switch), "\n")
      );
    }
    case IMPORT: {
      Arr *v = stat_get_import(this);
      char *file = fileix_to_str(*((int *)arr_get(v, 0)));
      char *alias = arr_get(v, 1);
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
