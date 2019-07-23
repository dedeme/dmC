// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader.h"
#include "Tpl.h"
#include "Log.h"

#define RS struct reader_Result
enum reader_ResultType {reader_ERROR, reader_MORE, reader_END};

RS {
  enum reader_ResultType type;
  Tpl *tpl;
  Gc *gc;
};

static RS *reader_result_new(Gc *gc) {
  RS *this = gc_add(gc, malloc(sizeof(RS)));
  this->type = reader_ERROR;
  this->tpl = tpl_new(gc);
  this->gc = gc;
  return this;
}

static RS *rfunctions (Log *log, FileLck *f, RS *rs) {
  GC_NEW

  Tpl *tpl = rs->tpl;
  char *doc = "";
  char *line, *l;
  char start;
  while (*(line = file_read_line(gc, f))) {
    log_inc(log);
    l = str_trim(gc, line);
    if (!*l) {
      doc = "";
      continue;
    }
    start = *l;

    if (start == '#') {
      doc = str_f(tpl_gc(tpl), "%s%s\n", doc, str_right(gc, l, 1));
      continue;
    }

    if (str_eq(l, "===") || str_eq(l, "*/")) {
      rs->type = start == '=' ? reader_MORE : reader_END;
      break;
    }

    TplVariable *fun = tplVariable_new(tpl);

    if (*doc) {
      tplVariable_set_comment(fun, str_new(tpl_gc(tpl), doc));
      doc = "";
    }

    if (start == '-') {
      tplVariable_set_access(fun, NO_ACCESS);
      l = str_right(gc, l, 1);
    } else if (start == '@') {
      tplVariable_set_access(fun, GETTER_SETTER);
      l = str_right(gc, l, 1);
    } else {
      tplVariable_set_access(fun, GETTER);
    }

    // Arr[char]
    Arr *parts = str_csplit_trim(tpl_gc(tpl), l, ':');
    int parts_sz = arr_size(parts);
    if (parts_sz == 2) { // ----- Ok
      char *err = tplVariable_set_function(
        fun, arr_get(parts, 0), arr_get(parts, 1)
      );
      if (!*err) {
        tpl_add_function(tpl, fun);
        continue;
      } else {
        log_msg(log, line, err);
        break;
      }
    }

    // ----- Fail

    if (parts_sz > 2) {
      log_msg(log, line, "Too much ':'");
    } else {
      log_msg(log, line, "':' is missing");
    }

    break;
  }

  return GC_CLEAN(rs);
}

static RS *rvariables (Log *log, FileLck *f, RS *rs) {
  GC_NEW

  Tpl *tpl = rs->tpl;
  char *doc = "";
  char *line, *l;
  char start;
  while (*(line = file_read_line(gc, f))) {
    log_inc(log);
    l = str_trim(gc, line);
    if (!*l) {
      doc = "";
      continue;
    }
    start = *l;

    if (start == '#') {
      doc = str_f(gc, "%s%s\n", doc, str_right(gc, l, 1));
      continue;
    }

    if (str_eq(l, "---")) {
      rs = rfunctions (log, f, rs); //---------------------------- go rfunctions
      break;
    }

    if (str_eq(l, "===") || str_eq(l, "*/")) {
      rs->type = start == '=' ? reader_MORE : reader_END;
      break;
    }

    TplVariable *var = tplVariable_new(tpl);

    if (*doc) {
      tplVariable_set_comment(var, str_new(tpl_gc(tpl), doc));
      doc = "";
    }

    if (start == '-') {
      tplVariable_set_access(var, NO_ACCESS);
      l = str_right(gc, l, 1);
    } else if (start == '@') {
      tplVariable_set_access(var, GETTER_SETTER);
      l = str_right(gc, l, 1);
    } else {
      tplVariable_set_access(var, GETTER);
    }

    // Arr[char]
    Arr *parts = str_csplit_trim(tpl_gc(tpl), l, ':');
    int parts_sz = arr_size(parts);
    if (parts_sz == 3) { // ----- Ok
      char *err = tplVariable_set_name_type_value(gc,
        var, arr_get(parts, 0), arr_get(parts, 1), arr_get(parts, 2)
      );
      if (!*err) {
        tpl_add_variable(tpl, var);
        continue;
      } else {
        log_msg(log, line, err);
        break;
      }
    }

    // ----- Fail

    if (parts_sz > 3) {
      log_msg(log, line, "Too much ':'");
    } else {
      log_msg(log, line, "At least one ':' is missing");
    }

    break;

  }

  return GC_CLEAN(rs);
}

static RS *rarguments (Log *log, FileLck *f, RS *rs) {
  GC_NEW

  Tpl *tpl = rs->tpl;
  char *doc = "";
  char *line, *l;
  char start;
  while (*(line = file_read_line(gc, f))) {
    log_inc(log);
    l = str_trim(gc, line);
    if (!*l) {
      doc = "";
      continue;
    }
    start = *l;

    if (start == '#') {
      doc = str_f(tpl_gc(tpl), "%s%s\n", doc, str_right(gc, l, 1));
      continue;
    }

    if (str_eq(l, "---")) {
      rs = rvariables (log, f, rs); //---------------------------- go rvariables
      break;
    }

    if (str_eq(l, "===") || str_eq(l, "*/")) {
      if (tpl_arguments_size(tpl)) {
        rs->type = start == '=' ? reader_MORE : reader_END;
      } else {
        log_msg(log, line, str_f(gc,
          "structure '%s' is empty", tpl_struct_name(rs->tpl)
        ));
      }
      break;
    }

    TplArgument *ar = tplArgument_new(tpl);

    if (*doc) {
      tplArgument_set_comment(ar, str_new(tpl_gc(tpl), doc));
      doc = "";
    }

    if (start == '-') {
      tplArgument_set_access(ar, NO_ACCESS);
      l = str_right(gc, l, 1);
    } else if (start == '@') {
      tplArgument_set_access(ar, GETTER_SETTER);
      l = str_right(gc, l, 1);
    } else {
      tplArgument_set_access(ar, GETTER);
    }

    // Arr[char]
    Arr *parts = str_csplit_trim(tpl_gc(tpl), l, ':');
    int parts_sz = arr_size(parts);
    if (parts_sz == 2) { // ----- Ok
      char *err = tplArgument_set_name_type(gc,
        ar, arr_get(parts, 0), arr_get(parts, 1)
      );
      if (!*err) {
        tpl_add_argument(tpl, ar);
        continue;
      } else {
        log_msg(log, line, err);
        break;
      }
    }

    // ----- Fail

    if (parts_sz > 2) {
      log_msg(log, line, "Too much ':'");
    } else {
      log_msg(log, line, "':' is missing");
    }

    break;
  }

  return GC_CLEAN(rs);
}

static RS *rstruct (Gc *gc, Log *log, FileLck *f) {
  GCL_NEW

  RS *r = reader_result_new(gc);
  char *doc = "";
  char *line, *l;
  char start;
  while (*(line = file_read_line(gcl, f))) {
    log_inc(log);
    l = str_trim(gcl, line);
    if (!*l) {
      doc = "";
      continue;
    }
    start = *l;

    if (start == '#') {
      doc = str_f(gcl, "%s%s\n", doc, str_right(gcl, l, 1));
      continue;
    }

    break;
  }

  Tpl *tpl = r->tpl;

  if (*doc) {
    tpl_set_struct_comment(tpl, str_new(tpl_gc(tpl), doc));
  }

  if (start == '-') {
    tpl_set_struct_type(tpl, PRIVATE);
    l = str_ltrim(gcl, str_right(gcl, l, 1));
  } else if (start == '=') {
    tpl_set_struct_type(tpl, HIDDEN);
    l = str_ltrim(gcl, str_right(gcl, l, 1));
  } else {
    tpl_set_struct_type(tpl, PUBLIC);
  }

  tpl_set_constructor_type(tpl, NORMAL);

  // Arr[char]
  Arr *parts = str_csplit_trim(tpl_gc(tpl), l, ':');
  int parts_sz = arr_size(parts);

  if (parts_sz == 2 || parts_sz == 1) { // ----- Ok
    tpl_set_serial_type(tpl, NONE);
    if (parts_sz == 2) {
      char *s = arr_get(parts, 1);
      if (str_eq(s, "serial")) {
        tpl_set_serial_type(tpl, SERIAL);
      } else if (str_eq(s, "SERIAL")) {
        tpl_set_serial_type(tpl, SERIAL);
        tpl_set_constructor_type(tpl, ONLY_SERIAL);
      } else if (str_eq(s, "to")) {
        tpl_set_serial_type(tpl, TO);
      } else if (str_eq(s, "from")) {
        tpl_set_serial_type(tpl, FROM);
      } else if (str_eq(s, "FROM")) {
        tpl_set_serial_type(tpl, FROM);
        tpl_set_constructor_type(tpl, ONLY_SERIAL);
      } else {
        log_msg(
          log, line, "Serialization type only can be: to | from | serial"
        );
        return GCL_CLEAN(r); // ----- Fail
      }
    }

    char *name = arr_get(parts, 0);
    if (!*name) {
      log_msg(log, line, "Struct name is missing");
      return GCL_CLEAN(r); // ----- Fail
    }
    tpl_set_struct_name(tpl, name);

    return GCL_CLEAN(rarguments(log, f, r)); // ----------------- go rarguments
  }

  // ----- Fail

  if (parts_sz) {
    log_msg(log, line, "Too much ':'");
  } else {
    log_msg(log, line, "Struct name is missing");
  };

  return GCL_CLEAN(r);
}

static Arr *rtpls (Gc *gc, Log *log, FileLck *f) {
  // Arr[Tpl]
  Arr *r = arr_new(gc);
  for (;;) {
    RS *rs = rstruct(gc, log, f);
    if (rs->type == reader_ERROR) {
      r = arr_new(gc);
      break;
    } else {
      if (tpl_struct_type(rs->tpl) == HIDDEN) {
        EACH(tpl_arguments(rs->tpl), TplArgument, a)
          tplArgument_set_access(a, NO_ACCESS);
        _EACH
        EACH(tpl_variables(rs->tpl), TplVariable, v)
          tplVariable_set_access(v, NO_ACCESS);
        _EACH
        EACH(tpl_functions(rs->tpl), TplVariable, f)
          tplVariable_set_access(f, NO_ACCESS);
        _EACH
      }
      arr_push(r, rs->tpl);
      if (rs->type == reader_END) {
        break;
      }
    }
  }
  return r;
}

// Returns Arr[Tpl]
Arr *reader_read(Gc *gc, char *fsrc) {
  GCL_NEW

  // Arr[Tpl]
  Arr *r = arr_new(gc);
  Log *log = log_new(gcl, fsrc);
  FileLck *f = file_ropen(gcl, fsrc);
  char *l;
  while (*(l = file_read_line(gcl, f))) {
    log_inc(log);
    if (str_eq(str_trim(gcl, l), "/*.")) {
      r = rtpls(gc, log, f);
      break;
    }
  }
  file_close(f);

  return GCL_CLEAN(r);
}

#undef RS
