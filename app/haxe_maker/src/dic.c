/*
 * Copyright 08-Mar-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include <dm/dm.h>
#include "../include/compile.h"

typedef struct {
  char *file;
  int line;
} Pos;
Pos *Pos_new(char *file, int line) {
  Pos *this = MALLOC(Pos);
  this->file = file;
  this->line = line;
  return this;
}

typedef struct {
  char *ter;
  Arr/*Pos*/ *poss;
} Dic_entry;
Dic_entry *Dic_entry_new(char *ter, Arr/*Pos*/ *poss) {
  Dic_entry *this = MALLOC(Dic_entry);
  this->ter = ter;
  this->poss = poss;
  return this;
}

typedef struct {
  char *ter;
  char *trans;
} Old_dic_entry;
Old_dic_entry *Old_dic_entry_new(char *ter, char *trans) {
  Old_dic_entry *this = MALLOC(Old_dic_entry);
  this->ter = ter;
  this->trans = trans;
  return this;
}

typedef struct {
  char *ter;
  char *trans;
  Arr/*Pos*/ *poss;
} New_dic_entry;
New_dic_entry *New_dic_entry_new(char *ter, char *trans, Arr/*Pos*/ *poss) {
  New_dic_entry *this = MALLOC(New_dic_entry);
  this->ter = ter;
  this->trans = trans;
  this->poss = poss;
  return this;
}

void _dic_add (Arr/*Dic_entry*/ *dic, char *ter, char *file, int line) {
  FNP(find, Dic_entry, p) {
    return !strcmp(p->ter, ter);
  }_FN

  Dic_entry *e = it_find(it_from(dic), find);
  if (e) {
    arr_add(e->poss, Pos_new(file, line));
  } else {
    Arr/*Pos*/ *poss = arr_new();
    arr_add(poss, Pos_new(file, line));
    arr_add(dic, Dic_entry_new(ter, poss));
  }
}

void _dic_file (char *f, Arr/*Dic_entry*/ *dic) {
  char *name = path_name(f);
  char *f2 = path_parent(f);
  char *name2 = path_name(f2);
  while (*name2 && strcmp(name2, "src")) {
    name = path_cat(name2, name, NULL);
    f2 = path_parent(f2);
    name2 = path_name(f2);
  }

  enum State { CODE, LCOMMENT, DQUOTE, SQUOTE, ENTRY };

  int line = 0;
  enum State status = CODE;
  FNX(add_dic, char, l) {
    void process () {
      if (status == CODE) {
        enum Finds {
          F_NOTHING, F_LCOMMENT, F_SCOMMENT, F_DQUOTE, F_SQUOTE, F_ENTRY
        };

        enum Finds find = F_NOTHING;
        int pos = 10000;

        int ix = str_index(l, "/*");
        if (ix != -1 && ix < pos) {
          find = F_LCOMMENT;
          pos = ix;
        }

        ix = str_index(l, "//");
        if (ix != -1 && ix < pos) {
          find = F_SCOMMENT;
          pos = ix;
        }

        ix = str_cindex(l, '"');
        if (ix != -1 && ix < pos) {
          find = F_DQUOTE;
          pos = ix;
        }

        ix = str_cindex(l, '\'');
        if (ix != -1 && ix < pos) {
          find = F_SQUOTE;
          pos = ix;
        }

        ix = str_index(l, "_(");
        if (ix != -1 && ix < pos) {
          find = F_ENTRY;
          pos = ix;
        }

        ix = pos;
        if (find == F_LCOMMENT) {
          status = LCOMMENT;
          l = str_sub(l, ix + 2, strlen(l));
          process();
        } else if (find == F_DQUOTE) {
          status = DQUOTE;
          l = str_sub(l, ix + 1, strlen(l));
          process();
        } else if (find == F_SQUOTE) {
          status = SQUOTE;
          l = str_sub(l, ix + 1, strlen(l));
          process();
        } else if (find == F_ENTRY) {
          status = ENTRY;
          l = str_sub(l, ix + 1, strlen(l));
          process();
        }

      } else if (status == LCOMMENT) {
        int ix = str_index(l, "*/");
        if (ix != -1) {
          status = CODE;
          l = str_sub(l, ix + 2, strlen(l));
          process();
        }
      } else if (status == DQUOTE) {
        int ix = str_index(l, "\\\"");
        if (ix != -1) {
          l = str_sub(l, ix + 2, strlen(l));
          process();
        } else {
          status = CODE;
          ix = str_cindex(l, '"');
          if (ix != -1) {
            l = str_sub(l, ix + 1, strlen(l));
            process();
          }
        }
      } else if (status == SQUOTE) {
        int ix = str_index(l, "\\'");
        if (ix != -1) {
          l = str_sub(l, ix + 2, strlen(l));
          process();
        } else {
          status = CODE;
          ix = str_cindex(l, '\'');
          if (ix != -1) {
            l = str_sub(l, ix + 1, strlen(l));
            process();
          }
        }
      } else if (status == ENTRY) {
        status = CODE;
        int ix = str_cindex(l, '"');
        if (ix != -1) {
          l = str_sub(l, ix + 1, strlen(l));
          ix = str_cindex(l, '"');
          Buf *bf = buf_new();
          while (ix != -1) {
            if (ix > 0 && l[ix - 1] == '\\') {
              buf_add(bf, str_sub(l, 0, ix + 1));
              l = str_sub(l, ix + 1, strlen(l));
              ix = str_cindex(l, '"');
              continue;
            }
            buf_add(bf, str_sub(l, 0, ix));
            l = str_sub(l, ix + 1, strlen(l));
            _dic_add(dic, buf_to_str(bf), name, line);
            break;
          }
        }
      } else {
        THROW "Bad value in 'status'" _THROW
      }
    }
    ++line;
    process();
  }_FN
  it_each(it_from(str_csplit(file_read(f), '\n')), add_dic);
}

Arr/*Dic_entry*/ *_dic_extract(char *root) {
  Arr/*Dic_entry*/ *dic = arr_new();

  void extract(char *dir) {
    FNX(file_process, char, f) {
      if (file_is_directory(f)) {
        extract(f);
      }
      if (str_ends(f, ".hx")) _dic_file(f, dic);
    }_FN
    it_each(it_from(file_dir(dir)), file_process);
  }
  extract(path_cat(root, "src", NULL));

  return dic;
}

Arr/*Old_dic_entry*/ *_dic_old_extract(char *f) {
  Arr/*Old_dic_entry*/ *dic = arr_new();

  FNX(process, char, l) {
    l = str_trim(l);
    if (*l && *l != '#') {
      int ix = str_cindex(l, '=');
      if (ix != -1) {
        char *ter = str_trim(str_sub(l, 0, ix));
        char *trans = str_trim(str_sub_end(l, ix + 1));
        if (*ter && *trans) {
          arr_add(dic, Old_dic_entry_new(ter, trans));
        }
      }
    }
  }_FN

  if (file_exists(f)) {
    it_each(it_from(str_csplit(file_read(f), '\n')), process);
  }

  return dic;
}

void _dic_mix(char *root, Arr/*Dic_entry*/ *dic) {
  Buf *tbf = buf_new();
  buf_add(
    tbf, "// File automatically generated by haxe_maker. Don't modify.\n\n"
  );
  buf_add(tbf, "class I18nData {\n");

  char *i18n = path_cat(root, "i18n", NULL);
  void mk(char *lang) {
    char *f = path_cat(i18n, str_printf("%s.txt", lang), NULL);
    Arr/*Old_dic_entry*/ *old_dic = _dic_old_extract(f);

    Arr/*Old_dic_entry*/ *orphan = arr_new();
    Arr/*New_dic_entry*/ *ok = arr_new();
    FNX(mk_orphan_ok, Old_dic_entry, oe) {
      FNP(find, Dic_entry, ne) {
        return !strcmp(oe->ter, ne->ter);
      }_FN
      Dic_entry *ne = it_find(it_from(dic), find);
      if (ne) {
        arr_add(ok, New_dic_entry_new(ne->ter, oe->trans, ne->poss));
      } else {
        arr_add(orphan, oe);
      }
    }_FN
    it_each(it_from(old_dic), mk_orphan_ok);

    FNP(mk_todo, Dic_entry, e) {
      FNP(find, New_dic_entry, ne) {
        return !strcmp(e->ter, ne->ter);
      }_FN
      return it_find(it_from(ok), find) == NULL;
    }_FN
    Arr/*Dic_entry*/ *todo = it_to(it_filter(it_from(dic), mk_todo));

    Buf *bf = buf_new();
    buf_add(bf, "# File generated file by haxe_maker\n\n");

    FNE(sort_orphan, Old_dic_entry, e1, e2) {
      return strcmp(str_to_lower(e1->ter), str_to_lower(e2->ter)) > 0;
    }_FN
    arr_sort(orphan, sort_orphan);
    FNX(w_orphan, Old_dic_entry, e) {
      buf_add(bf, "# ORPHAN\n");
      buf_add(bf, str_printf("%s = %s\n\n", e->ter, e->trans));
    }_FN
    it_each(it_from(orphan), w_orphan);

    FNE(sort_todo, Dic_entry, e1, e2) {
      return strcmp(str_to_lower(e1->ter), str_to_lower(e2->ter)) > 0;
    }_FN
    arr_sort(todo, sort_todo);
    FNX(w_todo, Dic_entry, e) {
      buf_add(bf, "# TODO\n");
      FNX(w_poss, Pos, p) {
        buf_add(bf, str_printf("# %s : %d\n", p->file, p->line));
      }_FN
      it_each(it_from(e->poss), w_poss);
      buf_add(bf, str_printf("%s = \n\n", e->ter));
    }_FN
    it_each(it_from(todo), w_todo);

    FNE(sort_ok, New_dic_entry, e1, e2) {
      return strcmp(str_to_lower(e1->ter), str_to_lower(e2->ter)) > 0;
    }_FN
    arr_sort(ok, sort_ok);
    FNX(w_ok, New_dic_entry, e) {
      FNX(w_poss, Pos, p) {
        buf_add(bf, str_printf("# %s : %d\n", p->file, p->line));
      }_FN
      it_each(it_from(e->poss), w_poss);
      buf_add(bf, str_printf("%s = %s\n\n", e->ter, e->trans));
    }_FN
    it_each(it_from(ok), w_ok);

    file_write(f, buf_str(bf));

    FNX(w_target, New_dic_entry, e) {
      buf_add(tbf, str_printf("      \"%s = %s\",\n", e->ter, e->trans));
    }_FN
    it_each(it_from(ok), w_target);
  }

  buf_add(tbf, "  public static function en():Array<String> {\n");
  buf_add(tbf, "    return [\n");
  mk("en");
  buf_add(tbf, "      \"\"];\n");
  buf_add(tbf, "  }\n\n");

  buf_add(tbf, "  public static function es():Array<String> {\n");
  buf_add(tbf, "    return [\n");
  mk("es");
  buf_add(tbf, "      \"\"];\n");
  buf_add(tbf, "  }\n\n");

  buf_add(tbf, "}\n");

  char *target = path_cat(root, "src", "I18nData.hx", NULL);
  file_write(target, buf_str(tbf));
}

void dic_make() {
  char *tmp = realpath("./", NULL);
  char *root = str_copy(tmp);
  free(tmp);

  Arr/*Dic_entry*/ *dic = _dic_extract(root);
  _dic_mix(root, dic);
}

