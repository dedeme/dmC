// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader.h"
#include "Def.h"
#include "StName.h"
#include "Arg.h"
#include "Var.h"
#include "Fun.h"
#include <assert.h>

enum ReadPos {HEAD, CONSTRUCTOR, VARIABLES, FUNCTIONS};

void reader_run(
  char **error_null,
  // Arr[Def]
  Arr *defs,
  Freader *slck,
  FileLck *tlck
) {
  *error_null = NULL;
  char *tmp = NULL;

  StName *stName_null = NULL;
  // Arr[Arg]
  Arr *args = arr_new((FPROC)arg_free);
  // Arr[Var]
  Arr *vars = arr_new((FPROC)var_free);
  // Arr[Fun]
  Arr *funs = arr_new((FPROC)fun_free);
  Buf *bf = buf_new();
  enum ReadPos pos = HEAD;
  for (;;) {
    char *line = freader_line_new(slck);

    if (!*line) {
      free(line);
      freader_msg(
        error_null, slck, "End of file reached reading template code"
      );
      break;
    }

    file_write_text(tlck, line);

    tmp = line;
    line = str_trim_new(tmp);
    free(tmp);
    if (str_eq(line, "*/")) {
      if (stName_null) {
        arr_push(defs, def_new(stName_null, args, vars, funs));
      }
      free(line);
      break;
    }

    if (!*line) {
      free(line);
      continue;
    }

    if (*line == '#') {
      tmp = line;
      line = str_right_new(tmp, 1);
      free(tmp);
      char *doc = str_f_new("/// %s\n", line);
      buf_add(bf, doc);
      free(doc);
      free(line);
      continue;
    }

    if (str_eq(line, "===")) {
      if (stName_null) {
        arr_push(defs, def_new(stName_null, args, vars, funs));

        stName_free(stName_null);
        arr_free(args);
        arr_free(vars);
        arr_free(funs);

        stName_null = NULL;
        args = arr_new((FPROC)arg_free);
        vars = arr_new((FPROC)var_free);
        funs = arr_new((FPROC)fun_free);
      }
      pos = HEAD;
      buf_reset(bf);
      free(line);
      continue;
    }

    if (pos == HEAD) {
      pos = CONSTRUCTOR;

      char *e_null = NULL;
      stName_from_str(&e_null, &stName_null, line);

      if (e_null) {
        freader_msg(error_null, slck, e_null);
        free(e_null);
        free(line);
        break;
      }

      if (*buf_str(bf)) {
        char *doc = buf_to_str_new(bf);
        stName_set_doc(stName_null, doc);
        free(doc);
        buf_reset(bf);
      }
    } else if ( pos == CONSTRUCTOR) {
      if (str_eq(line, "---")) {
        pos = VARIABLES;
      } else {
        char *e_null = NULL;
        Arg *arg_null = NULL;
        arg_from_str(&e_null, &arg_null, line);

        if (e_null) {
          freader_msg(error_null, slck, e_null);
          free(e_null);
          free(line);
          break;
        }

        if (*buf_str(bf)) {
          char *doc = buf_to_str_new(bf);
          arg_set_doc(arg_null, doc);
          free(doc);
          buf_reset(bf);
        }
        arr_push(args, arg_null);
      }
    } else if ( pos == VARIABLES) {
      if (str_eq(line, "---")) {
        pos = FUNCTIONS;
      } else {
        char *e_null = NULL;
        Var *var_null = NULL;
        var_from_str(&e_null, &var_null, line);
        if (e_null) {
          freader_msg(error_null, slck, e_null);
          free(e_null);
          free(line);
          break;
        }
        if (*buf_str(bf)) {
          char *doc = buf_to_str_new(bf);
          var_set_doc(var_null, doc);
          free(doc);
          buf_reset(bf);
        }
        arr_push(vars, var_null);
      }
    } else {
      char *e_null = NULL;
      Fun *fun_null = NULL;
      fun_from_str(&e_null, &fun_null, line);
      if (e_null) {
        freader_msg(error_null, slck, e_null);
        free(e_null);
        free(line);
        break;
      }
      if (*buf_str(bf)) {
        char *doc = buf_to_str_new(bf);
        fun_set_doc(fun_null, doc);
        free(doc);
        buf_reset(bf);
      }
      arr_push(funs, fun_null);
    }
    free(line);
  }
  stName_free(stName_null);
  arr_free(args);
  arr_free(vars);
  arr_free(funs);
  buf_free(bf);
}
