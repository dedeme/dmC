// Copyright 25-Jul-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "checker/tpchecker.h"
#include "kut/DEFS.h"
#include "kut/opt.h"
#include "module.h"
#include "modules.h"
#include "fileix.h"

// VISITED ---------------------------------------
// Visited file structure.

#define Visited tpchecker_Visited
typedef struct {
  int fix; // File index
} Visited;

static Visited *visited_new (int fix) {
  Visited *this = MALLOC(Visited);
  this->fix = fix;
  return this;
}

// Returns 'true' if 'visiteds' contains 'fix'.
//  visiteds: Arr<Visited>.
static int visiteds_contains (Arr *visiteds, int fix) {
  EACH(visiteds, Visited, e) {
    if (e->fix == fix) return TRUE;
  }_EACH
  return FALSE;
}

// FILE POSITION ---------------------------------
// File - postion register.

#define Pos tpchecker_Pos
typedef struct {
  int fix; // File index
  int nline; // Number of line
  char *line; // Line text
} Pos;

static Pos *pos_new (int fix, int nline, char *line) {
  Pos *this = MALLOC(Pos);
  this->fix = fix;
  this->nline = nline;
  this->line = line;
  return this;
}

static char *pos_to_str (Pos *pos) {
  return str_f("%s:%d: %s", fileix_to_fail(pos->fix), pos->nline, pos->line);
};

// TYPE ------------------------------------------
// type register.

#define Type tpchecker_Type
typedef struct {
  char *type; // Type identifier (without '<' and '>')
  Arr *poss; // Arr<Pos> Positions where 'type' is used.
             // If 'type' is defined it is an empty array.
} Type;

// Do not use directly. Use extypes_add_use and extypes_add_def.
// 'poss' is Arr<Pos>
static Type *type_new (char *type, Arr *poss) {
  Type *this = MALLOC(Type);
  this->type = type;
  this->poss = poss;
  return this;
}

// Types is Arr<Type>
static void types_add_use (Arr *types, char *type, Pos *pos) {
  EACH(types, Type, tp) {
    if (!strcmp(tp->type, type)) {
      if (arr_size(tp->poss)) arr_push(tp->poss, pos);
      return;
    }
  }_EACH
  arr_push(types, type_new(type, arr_new_from(pos, NULL)));
}

// Types is Arr<Type>
static void types_add_def (Arr *types, char *type) {
  EACH(types, Type, tp) {
    if (!strcmp(tp->type, type)) {
      if (arr_size(tp->poss)) arr_clear(tp->poss);
      return;
    }
  }_EACH
  arr_push(types, type_new(type, arr_new()));
}

// MAIN program ----------------------------------
// -----------------------------------------------

static char *read_type (Arr *types, Pos *pos, char *code);

static int is_letter (char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static int is_letter_or_number (char ch) {
  return is_letter(ch) || (ch >= '0' && ch <= '9');
}

static int id_check (char *id) {
  if (*id) {
    char *pid = id;
    char ch = *pid++;
    if (is_letter(ch)) {
      while (*pid) {
        ch = *pid++;
        if (!is_letter_or_number(ch)) return FALSE;
      }
      return TRUE;
    }
  }
  return FALSE;
}

// 'types' is Arr<Type>
static char *read_obj (Arr *types, Pos *pos, char *code) {
  int ix = str_cindex(code, '>');
  if (ix == -1) {
    printf("Missing '>':\n  %s\n", pos_to_str(pos));
    return "#";
  }
  char *t = str_left(code, ix);
  if (id_check(t)) {
    types_add_use(types, t, pos);
    return str_right(code, ix + 1);
  }
  printf("Bad type <%s>:\n  %s\n", t, pos_to_str(pos));
  return "#";
}

// 'types' is Arr<Type>
static char *read_array (Arr *types, Pos *pos, char *code) {
  if (*code == ']') return str_right(code, 1);

  char *rest = read_type(types, pos, code);
  char ch = *rest;
  switch (ch) {
    case '@': {
      printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
      return "#";
    }
    case '#': return "#";
    case ']': return str_right(rest, 1);
    case '.': {
      if (rest[1] != ']') {
        printf(
          "Expected '.]', found '%s':\n  %s\n",
          str_left(rest, -1), pos_to_str(pos)
        );
        return "#";
      }
      return str_right(rest, 2);
    }
    default: for (;;) {
      if (ch == ']') return str_right(rest, 1);
      if (*rest != ',') {
        printf(
          "Expected ',', found '%s':\n  %s\n",
          str_left(rest, -1), pos_to_str(pos));
        return "#";
      }
      rest = read_type(types, pos, str_right(rest, 1));
      ch = *rest;
      if (ch == '@') {
        printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
        return "#";
      } else if (ch == '#') {
        return "#";
      }
    }
  }
}

// 'types' is Arr<Type>
static char *read_dic (Arr *types, Pos *pos, char *code) {
  if (*code == '}') return str_right(code, 1);

  int ix = str_cindex(code, ':');
  if (ix != -1 && id_check(str_left(code, ix))) {
    char *rest = read_type(types, pos, str_right(code, ix + 1));
    for (;;) {
      char ch = *rest;
      switch (ch) {
        case '@': {
          printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
          return "#";
        }
        case '#': return "#";
        case '}': return str_right(rest, 1);
        default: {
          if (ch ==  ',') {
            rest = str_right(rest, 1);
            ix = str_cindex(rest, ':');
            if (ix == -1 || !id_check(str_left(rest, ix))) {
              printf(
                "Expected dictionary key, found '%s':\n  %s\n",
                str_left(rest, -1), pos_to_str(pos)
              );
              return "#";
            }
            rest = read_type(types, pos, str_right(rest, ix + 1));
          } else {
            printf(
              "Expected ',', found '%s':\n  %s\n",
              str_left(rest, -1), pos_to_str(pos)
            );
            return "#";
          }
        }
      }
    }
  }

  char *rest = read_type(types, pos, code);
  char ch = *rest;
  switch (ch) {
    case '@': {
      printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
      return "#";
    }
    case '#': return "#";
    case '}': return str_right(rest, 1);
    case '.': {
      if (rest[1] != '}') {
        printf(
          "Expected '.}', found '%s':\n  %s\n",
          str_left(rest, -1), pos_to_str(pos)
        );
        return "#";
      }
      return str_right(rest, 2);
    }
    default: for (;;) {
      if (ch == '}') return str_right(rest, 1);
      if (*rest != ',') {
        printf(
          "Expected ',', found '%s':\n  %s\n",
          str_left(rest, -1), pos_to_str(pos));
        return "#";
      }
      rest = read_type(types, pos, str_right(rest, 1));
      ch = *rest;
      if (ch == '@') {
        printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
        return "#";
      } else if (ch == '#') {
        return "#";
      }
    }
  }
}

// 'types' is Arr<Type>
static char *read_func (Arr *types, Pos *pos, char *code) {
  if (*code == '-') {
    if (code[1] != '>') {
      printf(
        "Expected '->', found '%s':\n  %s\n",
        str_left(code, -1), pos_to_str(pos));
      return "#";
    }
    code = str_right(code, 2);
  } else {
    for (;;) {
      code = read_type(types, pos, code);
      char ch = *code;
      if (ch == '@') {
        printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
        return "#";
      } else if (ch == '#') {
        return "#";
      } else if (ch == '-') {
        if (code[1] != '>') {
          printf(
            "Expected '->', found '%s':\n  %s\n",
            str_left(code, -1), pos_to_str(pos));
          return "#";
        }
        code = str_right(code, 2);
        break;
      } else if (ch == ',') {
        code = str_right(code, 1);
      } else {
        printf(
          "Expected ',', found '%s':\n  %s\n",
          str_left(code, -1), pos_to_str(pos));
        return "#";
      }
    }
  }
  if (code[0] == '(' && code[1] == ')')
    return str_right(code, 2);
  return read_type(types, pos, code);
}

// 'types' is Arr<Type>
static char *read_par (Arr *types, Pos *pos, char *code) {
  if (*code == ')') {
    printf("'()' found in not return position:\n  %s\n", pos_to_str(pos));
    return "#";
  }

  char *rest = read_type(types, pos, code);
  char ch = *rest;
  switch (ch) {
    case '@': {
      printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
      return "#";
    }
    case '#': return "#";
    case ')': return str_right(rest, 1);
    default: for (;;) {
      if (ch == ')') return str_right(rest, 1);
      if (*rest != '|') {
        printf(
          "Expected '|', found '%s':\n  %s\n",
          str_left(rest, -1), pos_to_str(pos));
        return "#";
      }
      rest = read_type(types, pos, str_right(rest, 1));
      ch = *rest;
      if (ch == '@') {
        printf("Unexpected end of type:\n  %s\n", pos_to_str(pos));
        return "#";
      } else if (ch == '#') {
        return "#";
      }
    }
  }
}

// 'types' is Arr<Type>
// Returns '#' if error, '@' if end of type, other not empty string, if
//   it rests more code for reading.
static char *read_type (Arr *types, Pos *pos, char *code) {
  char ch = *code;
  switch (ch) {
    case 'b':
    case 'i':
    case 'f':
    case 's':
    case '*': return str_right(code, 1);
    case '<': return read_obj(types, pos, str_right(code, 1));
    case '[': return read_array(types, pos, str_right(code, 1));
    case '{': return read_dic(types, pos, str_right(code, 1));
    case '\\': return read_func(types, pos, str_right(code, 1));
    case '(': return read_par(types, pos, str_right(code, 1));
    case '@': {
      printf("Type missing:\n  %s\n", pos_to_str(pos));
      return "#";
    }
    default: {
      printf("Unexpected type '%c':\n  %s\n", ch, pos_to_str(pos));
      return "#";
    }
  }
}

// 'types' is Arr<Type>
static char *read_type0 (Arr *types, Pos *pos, char *code) {
  int ix = str_last_index(code, "->");
  if (ix != -1) {
    char *otype = str_trim(str_right(code, ix + 2));
    if (*otype == '<' && str_ends(otype, ">")) {
      char *id = str_right(str_left(otype, -1), 1);
      if (id_check(id)) types_add_def(types, id);
    }
  }
  char *code2 = ATOMIC(strlen(code) + 2);
  char *pcode = code;
  char *pcode2 = code2;
  while (*pcode) {
    if (*pcode <= ' ') ++pcode;
    else *pcode2++ = *pcode++;
  }
  *pcode2++ = '@';
  *pcode2 = 0;
  return read_type(types, pos, code2);
}

// 'types' is Arr<Type>
static void lcheck (Arr *types, int fix, char *l, int nline, char *code) {
  Pos *pos = pos_new(fix, nline, l);
  char *rest = read_type0(types, pos, code);
  char ch = *rest;
  switch (ch) {
    case '@':
    case '#': return;
    default:
      printf(
        "Extra caracters '%s' (Initial '\\' is missing?):\n  %s\n",
        str_left(rest, -1), pos_to_str(pos)
      );
      return;
  }
}

// 'types' is Arr<Type>
static void fcheck (Arr *types, int fix) {
  char *code = fileix_read(fix);
  char *previous_icomment = "";
  int nprevious_icomment = 0;
  char *previous_comment = "";
  int nprevious_comment = 0;
  EACH(str_csplit(code, '\n'), char, l) {
    int nl = _i + 1;
    char *l2 = str_ltrim(l);
    if (str_starts(l2, "///")) {
      if (l2[3] == '-' && *previous_comment) {
        previous_comment = str_f(
          "%s%s", previous_comment, str_trim(str_right(l2, 4))
        );
        continue;
      }
      previous_comment = l;
      nprevious_comment = nl;
      continue;
    }
    if (str_starts(l2, "//:")) {
      if (l2[3] == '-' && *previous_icomment) {
        previous_icomment = str_f(
          "%s%s", previous_icomment, str_trim(str_right(l2, 4))
        );
        continue;
      }
      previous_icomment = l;
      nprevious_icomment = nl;
      continue;
    } else if (*l2) {
      if (*previous_icomment) {
        l = previous_icomment;
        nl = nprevious_icomment;
        l2 = str_ltrim(l);
        lcheck(types, fix, l, nl, str_trim(str_right(l2, 3)));
        previous_icomment = "";
      }
      if (*previous_comment) {
        l = previous_comment;
        nl = nprevious_comment;
        l2 = str_ltrim(l);
        lcheck(types, fix, l, nl, str_trim(str_right(l2, 3)));
        previous_comment = "";
      }
    } else {
      previous_icomment = "";
      previous_comment = "";
    }
  }_EACH
}

// 'visiteds' is Arr<Visited>
// 'types'is Arr<Type>
static void run (Arr *visiteds, Arr *types, int fix) {
  Module *md = opt_get(opt_get(modules_get(fix)));
  EACH(imports_get_array(module_get_imports(md)), ImportsEntry, im) {
    int ifix = imports_entry_fix(im);
    if (!visiteds_contains(visiteds, ifix)) {
      arr_push(visiteds, visited_new(ifix));
      run(visiteds, types, ifix);
    }
  }_EACH
  fcheck(types, fix);
};

void tpchecker_run (int fix) {
  Visited *v = visited_new(fix);

  // <Type>
  Arr *types = arr_new();
  Arr *sys_types = arr_new_from(
    "bytes", "iter", "file", "tcpServer", "tcpConn", "thread", NULL
  );
  EACH(sys_types, char, t) {
    types_add_def(types, t);
  }_EACH

  run(arr_new_from(v, NULL), types, fix);

  EACH(types, Type, tp) {
    if (arr_size(tp->poss)) {
      Arr *apos = arr_new();
      EACH(tp->poss, Pos, p) {
        arr_push(apos, str_f("  %s", pos_to_str(p)));
      }_EACH
      printf(
        "Undefined type <%s>:\n  %s\n",
        tp->type,
        arr_cjoin(apos, '\n')
      );
    }
  }_EACH
}
