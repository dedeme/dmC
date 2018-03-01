// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dm/dm.h>

enum Status {
  CODE,
  QUOTE,
  LONG
};

static char quote_char = '"';

// returns -1 if x and y are -1; 0 if x is != -1 and lesser and 1 if
// y is != 1 and lesser
static int min(int x, int y) {
  return x == -1
    ? y == -1 ? -1 : 1
    : y == -1
      ? 0
      : (x < y) ? 0 : 1
  ;
}

// returns -1 if x, y and < are -1; 0 if x is != -1 and lesser, 1 if
// y is != 1 and lesser and 2 if z is != -1
static int min3(int x, int y, int z) {
  int m = min(x, y);
  return m == -1
    ? z == -1 ? -1 : 2
    : m == 0
      ? min(x, z) == 1 ? 2 : 0
      : min(y ,z) == 1 ? 2 : 1
  ;
}

static void help() {
	puts(
		"Use:\n"
		"  jscontrol <dir>\n"
		"For example:\n"
		"  jscontrol /deme/dmjs18\n"
  );
}

void process_point(
  char *file,
  uint nline,
  char *start_code,
  char *code
 ) {
  while (*code) {
    char ch = *code;
    if (
      !(
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z') ||
        ch == ' ' || ch == '$'
      )
    ) {
      break;
    }
    ++code;
  }
  if (*code) {
    if (!(str_starts(code, "/**/") || *code == '(')) {
      printf("[jscontrol]: Error in '%s'\n    %s:%d\n",
        str_trim(start_code), file, nline);
    }
  }
}

void process_code(
  char *file,
  uint nline,
  char *code,
  uint code_len
) {
  int ix = str_cindex(code, '.');
  if (ix != -1 && ix < code_len) {
    ++ix;
    char *new_code = code + ix;
    int new_len = code_len - ix;
    if (
      str_starts(new_code, "_") ||
      str_starts(new_code, "length") ||
      (*new_code >= '0' && *new_code <= '9')
    ) {
      process_code(file, nline, new_code, new_len);
    } else if (str_starts(new_code, "..")) {
      process_code(file, nline, new_code + 2, new_len - 2);
    } else {
      process_point(file, nline, code, new_code);
      process_code(file, nline, new_code, new_len);
    }
  }
}

enum Status process_line(
  enum Status st,
  char *file,
  uint nline,
  char *line
) {
  if (st == QUOTE) {
    int ibar = str_cindex(line, '\\');
    int iquote = str_cindex(line, quote_char);
    int m = min(ibar, iquote);
    if (m == 0) {
      st = process_line(st, file, nline, line + ibar + 2);
    } else if (m == 1) {
      st = process_line(CODE, file, nline, line + iquote + 1);
    } else {
      st = CODE;
    }
  } else if (st == LONG) {
    int iend = str_index(line, "*/");
    if (iend != -1) {
      st = process_line(CODE, file, nline, line + iend + 2);
    }
  } else {
    int ilong = str_index(line, "/*"); // sel 1
    int ishort = str_index(line, "//"); // sel 2
    int iquote = str_cindex(line, '"');  // sel 3
    int iquote2 = str_cindex(line, '\'');
    int iquote3 = str_cindex(line, '`');
    int m = min3(iquote, iquote2, iquote3);
    int sel = 0; // CODE
    if (m == -1) {
      sel = min(ilong, ishort) + 1;
    } else {
      if (m == 0) {
        quote_char = '"';
      } else if (m == 1) {
        quote_char = '\'';
        iquote = iquote2;
      } else {
        quote_char = '`';
        iquote = iquote3;
      }
      sel = min3(iquote, ilong, ishort);
      if (sel == 0) {
        sel = 3;
      }
    }

    if (sel == 1) {
      process_code(file, nline, line, ilong);
      st = process_line(LONG, file, nline, line + ilong + 2);
    } else if (sel == 2) {
      process_code(file, nline, line, ishort);
      st = CODE;
    } else if (sel == 3) {
      process_code(file, nline, line, iquote);
      st = process_line(QUOTE, file, nline, line + iquote + 1);
    } else {
      process_code(file, nline, line, strlen(line));
      st = CODE;
    }
  }
  return st;
}

void process_file(char *file) {
  LckFile *lck = file_ropen(file);

  char *line = file_read_line(lck);
  if (!str_starts(line, "/*nojscontrol*/")) {
    enum Status st = CODE;
    uint nline = 1;
    while (*line) {
      st = process_line(st, file, nline++, line);
      line = file_read_line(lck);
    }
  }
  free(line);

  file_close(lck);
}

static void process(char *file) {
  if (file_is_directory(file)) {
    char *name = path_name(file);
    if (!strcmp(name, "www")) {
      return;
    }

    Arr/*char*/ *list = file_dir(file);
    EACH(list, char, f) {
      process(f);
    }_EACH
  } else {
    if (str_ends(file, ".js")) {
      process_file(file);
    }
  }
}

int main (int argc, char **argv) {
  exc_init();

  if (argc != 2) {
    puts("Wrong number of parameters calling jscontrol");
    help();
    return 0;
  }

  process(argv[1]);
  return 0;
}
