// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "DEFS.h"
#include "error.h"
#include "record.h"

static void pute () {
  puts ("jstemplate:0: Error\n");
}

static void help () {
  puts(
    "Use jstemplate <file>\n"
    "  For example: jstemplate src/Server.js"
  );
}

static int parse_to_tmp(char *file) {
  char *ftmp = str_f("%s/tmp", sys_home());
  FileLck *target = file_wopen(ftmp);
  FileLck *source = file_ropen(file);

  enum TypeTemplate type_template = RECORD;
  enum State state = CODE0;
  char *l = NULL;
  char *errl = NULL;
  int nl = 0;
  int errnl = 0;
  for (;;) {
    l = file_read_line(source);
    ++nl;
    if (!*l) {
      break;
    }

    char *ltrim = str_trim(l);
    if (!*ltrim && state != TEMPLATE_SECOND_WRITE_MARK) {
      file_write_text(target, l);
      continue;
    }
    if (state == CODE || state == CODE0) {
      if (str_eq(ltrim, "/*.")) {
        errl = str_new(l);
        errnl = nl;

        char *tmp = l;
        l = str_replace(tmp, "/*.", "/* .");
        state = TEMPLATE_BEGIN;
      }
    } else if (state == TEMPLATE_BEGIN) {
      if (str_starts(ltrim, "_rc_")) {
        char *err = record_init(ltrim);
        if (!*err) {
          type_template = RECORD;
          state = TEMPLATE_PARAM;
        } else {
          error_puts(file, nl, l, err);
          state = ERROR;
          break;
        }
      }
    } else if (state == TEMPLATE_PARAM) {
      if (str_eq(ltrim, "*/")) {
        char *err = record_read_end();
        if (!*err) {
          errl = str_new(l);
          errnl = nl;
          state = TEMPLATE_FIRST_WRITE_MARK;
        } else {
          error_puts(file, nl, l, err);
          state = ERROR;
          break;
        }
      } else if (type_template == RECORD) {
        char *err = record_field(ltrim);
        if (*err) {
          error_puts(file, nl, l, err);
          state = ERROR;
          break;
        }
      } else
        EXC_ILLEGAL_STATE(str_f("'type_template' is '%d'", type_template))

    } else if (state == TEMPLATE_FIRST_WRITE_MARK) {
      if (str_eq(ltrim, "/*--*/")) {
        file_write_text(target, l);
        errl = str_new(l);
        errnl = nl;
        state = TEMPLATE_SECOND_WRITE_MARK;
      }
    } else if (state == TEMPLATE_SECOND_WRITE_MARK) {
      if (str_eq(ltrim, "/*--*/")) {
        char *code = NULL;
        if (type_template == RECORD) {
          code = record_code();
        } else
          EXC_ILLEGAL_STATE(str_f("'type_template' is '%d'", type_template))

//        puts(code);
        file_write_text(target, code);
        state = CODE;
      }
    } else
      EXC_ILLEGAL_STATE(str_f("Bad state '%d'", state))

    if (state != TEMPLATE_SECOND_WRITE_MARK) {
      file_write_text(target, l);
    }

  }

  if (state == TEMPLATE_BEGIN) {
    error_puts(file, errnl, errl, "Template is missing");
  } else if (state == TEMPLATE_PARAM) {
    error_puts(file, errnl, errl, "End template is missing");
  } else if (state == TEMPLATE_FIRST_WRITE_MARK) {
    error_puts(file, errnl, errl, "First '/*--*/' is missing");
  } else if (state == TEMPLATE_SECOND_WRITE_MARK) {
    error_puts(file, errnl, errl, "Second '/*--*/' is missing");
  }

  file_close(source);
  file_close(target);
  return state == CODE;
}

/*
static void copy_from_tmp_test(char *file) {
  char *file_test = str_f("%s_test.js", file);
  char *ftmp = str_f("%s/tmp", sys_home());
  file_copy(ftmp, file_test);
}
*/

static void copy_from_tmp(char *file) {
  char *ftmp = str_f("%s/tmp", sys_home());
  file_copy(ftmp, file);
}

int main (int argc, char* argv[]) {
  if (argc != 2) {
    pute();
    help();
    return 1;
  }

  sys_init("jstemplate");

  if (!file_exists(argv[1])) {
    pute();
    printf("File '%s' not found\n", argv[1]);
    return 1;
  }

  if (parse_to_tmp(argv[1])) {
    copy_from_tmp(argv[1]);
//    copy_from_tmp_test(argv[1]);
  }

  return 0;
}
