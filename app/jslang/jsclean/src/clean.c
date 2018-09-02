// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "clean.h"
#include "dot.h"
#include "tp/Rs.h"

int min(int n, int values[]) {
  int r = -1;
  int vsel = -1;
  RANGE0(i, n) {
    int v = values[i];
    if (v != -1 && (r == -1 || v < vsel)){
      r = i;
      vsel = v;
    }
  }_RANGE
  return r;
}

static Rs *process_comment(char *line) {
  int i = str_index(line, "*/");
  if (i == -1) {
    return rs_new(COMMENT, "\n", "");
  }
  return rs_new(CODE, "", str_sub_end(line, i + 2));
}

static Rs *process_quote(enum St st, char *line) {
  char *q = st == SINGLE ? "'" : st == BACK ? "`" : "\"";
  int iq = str_index(line, q);
  int ib2 = str_index(line, "\\\n");
  int ib = str_index(line, "\\");
  int ix = min(3, (int[]){iq, ib2, ib});
  switch (ix) {
    case 0/*"*/: return rs_new(
        CODE, str_sub(line, 0, iq + 1), str_sub_end(line, iq + 1)
      );
    case 1/*\endLine*/: return rs_new(st, line, "");
    case 2/*\*/: return rs_new(
        st, str_sub(line, 0, ib + 2), str_sub_end(line, ib + 2)
      );
    default: return rs_new(st, line, "");
  }
  exc_illegal_state("Unreachable");
  return NULL;
}

static Rs *process_code(char *line) {
  int ic = str_index(line, "//");
  int ic2 = str_index(line, "/*");
  int iq1 = str_index(line, "'");
  int iq2 = str_index(line, "\"");
  int iq3 = str_index(line, "`");
  int ix = min(5, (int[]){ic, ic2, iq1, iq2, iq3});
  switch (ix) {
    case 0/*ic*/: return rs_new(
        CODE, str_printf("%s\n", str_rtrim(str_sub(line, 0, ic))), ""
      );
    case 1/*ic2*/: return rs_new(
        COMMENT,
        str_printf("%s ", str_rtrim(str_sub(line, 0, ic2))),
        str_sub_end(line, ic2 + 2)
      );
    case 2/*'*/: return rs_new(
        SINGLE,
        str_printf("%s'", str_sub(line, 0, iq1)),
        str_sub_end(line, iq1 + 1)
      );
    case 3/*"*/: return rs_new(
        DOUBLE,
        str_printf("%s\"", str_sub(line, 0, iq2)),
        str_sub_end(line, iq2 + 1)
      );
    case 4/*`*/: return rs_new(
        BACK,
        str_printf("%s`", str_sub(line, 0, iq3)),
        str_sub_end(line, iq3 + 1)
      );
    default: return rs_new(CODE, line, "");
  }
  exc_illegal_state("Unreachable");
  return NULL;
}

static Rs *process_line(char *line, enum St st) {
  switch (st) {
    case CODE: return process_code(line);
    case COMMENT: return process_comment(line);
    case SINGLE:
    case DOUBLE:
    case BACK: return process_quote(st, line);
  }
  exc_illegal_state("Unreachable");
  return NULL;
}

void clean_run(P *file, char *target) {
  enum St st = CODE;

  Achar *old_props = dot_read(file);
  Achar *new_props = achar_new();

  int nline = 0;
  void *map(char *line) {
    ++nline;
    Buf *bf = buf_new();
    while(true) {
      Rs *rs = process_line(line, st);
      char *read = rs_read(rs);
      if (st == CODE) {
        dot_print_and_update(file, old_props, new_props, read, nline);
      }
      buf_add(bf, read);
      st = rs_state(rs);
      line = rs_rest(rs);
      if (!*line) {
        break;
      }
    }
    line = buf_to_str(bf);
    if (!*str_trim(line)) {
      return "";
    }

    return line;
  }

  bool filter(char *line) {
    return *line;
  }

  LckFile *lck = file_ropen(p_absolute(file));
  Ichar *lines = file_to_it(lck);
  lines = (Ichar *)ichar_map(lines, map);
  lines = (Ichar *)ichar_filter(lines, filter);

  char *ftarget = path_cat(target, p_relative(file), NULL);
  char *parent = path_parent(ftarget);
  if (!file_exists(parent)) {
    file_mkdir(parent);
  }
  file_from_it(ftarget, lines);
  file_close(lck);

  dot_write(file, new_props);
}
