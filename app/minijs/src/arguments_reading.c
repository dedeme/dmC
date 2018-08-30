// Copyright 03-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "arguments_reading.h"
#include "stdio.h"
#include "dmc/str.h"
#include "dmc/file.h"
#include "dmc/path.h"
#include "dmc/DEFS.h"
#include "dmc/ct/Lchar.h"
#include "dmc/ct/Achar.h"
#include "types/Either.h"
#include "io.h"
#include "types/Cpath.h"
#include "types/UserData.h"
#include "types/ScanResult.h"
#include "scan/token.h"

void print_error(char *msg) {
  puts(msg);
  puts("\n"
    "Use:\n"
		"  minijs [OPTIONS] <main_file> \n"
		"  or\n"
		"  minijs -h\n\n"
    "OPTIONS:\n"
    "  -js <path> File '.js'.\n"
    "      Default: index.js in the same directory that <main_file>\n"
    "  -p <paths> List of source paths separated with ':'\n"
    "      Automaticaly added 'src' directory.\n\n"
    "main_file:\n"
    "  mini file\n\n"
    "Examples:\n"
    "  minijs MyProgram.mini\n"
    "  minijs -p /home/my/lib1:lib2 MyProgram.mini -js www/index.js\n"
  );
}

Either/*Lchar*/ *roots_control(Lchar *roots) {
  bool exists_src = false;
  EACHL(roots, char, path) {
    if (!*path) {
      return either_fail("There is a blank path in '-p <paths>'.");
    }
    if (!file_is_directory(path)) {
      return either_fail(str_printf(
        "'%s' is not a directory in '-p <paths>'.", path
      ));
    }
    if (str_eq(path, "src")) {
      exists_src = true;
    }
  }_EACH
  if (!exists_src) {
    roots = lchar_cons(roots, "src");
  }
  return either_ok(roots);
}

Either/*Cpath*/ *source_control(Lchar *roots, char *source) {
  Either/*Cpath*/ *ecpath = io_cpath(roots, source);
  if (either_is_fail(ecpath)) {
    return ecpath;
  }
  Cpath *source_path = either_value(ecpath);
  char *className = cpath_name(source_path);
  Scanner *sc = scanner_new_text(className);
  ScanResult *scr = token_id(opt_null(), sc);

  if (
    scanResult_is_error(scr) ||
    !str_eq(className, (char *) scanResult_result(scr))
  ) {
    return either_fail(str_printf(
      "Source '%s' is not a valid class name.", className
    ));
  }
  char *source_com = cpath_complete(source_path);
  if (file_is_directory(source_com)) {
    return either_fail(str_printf("Source '%s' is a directory.", source_com));
  }
  if (!file_exists(source_com)) {
    return either_fail(str_printf("Source '%s' not found.", source_com));
  }
  return either_ok(source_path);
}

Either/*char*/ *target_control(char *target, char *alternative) {
  if (!*target) {
    return either_ok(alternative);
  }
  if (file_is_directory(target)) {
    return either_fail(str_printf("Target '%s' is a directory.", target));
  }
  if (!file_is_directory(path_parent(target))) {
    return either_fail(str_printf(
      "Parent of target '%s' not found.", target
    ));
  }
  return either_ok(target);
}

Opt/*UserData*/ *arguments_reader_run(int argc, char **args) {
  Lchar *roots = lchar_new();
  char *target = "";
  char *source = "";
  bool p_processed = false;
  bool js_processed = false;
  Opt/*UserData*/ *result = opt_null();

  if (argc == 1) {
    print_error ("Arguments are missing.");
    return result;
  }

  if (argc == 2 && str_eq(args[1], "-h")) {
    print_error ("minijs help.");
    return result;
  }

  int i = 1;

  while (true) {
    char *arg = args[i++];
    if (str_eq(arg, "-p")) {
      if (p_processed) {
        print_error("Argument '-p' is duplicated.");
        break;
      }
      p_processed = true;
      if (argc < i + 1) {
        print_error("Expected paths argument of '-js'.");
        break;
      }
      Achar *paths = str_csplit(args[i++], ':');
      roots = lchar_from_it(achar_to_it(paths));
      if (argc < i + 1) {
        break;
      }
    } else if (str_eq(arg, "-js")) {
      if (js_processed) {
        print_error("Argument '-js' is duplicated.");
        break;
      }
      js_processed = true;
      if (argc < i + 1) {
        print_error("Expected path argument of '-p'.");
        break;
      }
      target = args[i++];
      if (argc < i + 1) {
        break;
      }
    } else {
      source = arg;
      if (argc < i + 1) {
        Either/*Lchar*/ *r = roots_control(roots);
        if (either_is_fail(r)) {
          print_error(either_msg(r));
          break;
        }
        roots = either_value(r);

        /*Cpath*/r = source_control(roots, source);
        if (either_is_fail(r)) {
          print_error(either_msg(r));
          break;
        }
        Cpath *source_path = either_value(r);
        char *source_com = cpath_complete(source_path);

        char *alternative = str_printf("%s/%s.js",
          path_parent(source_com), path_only_name(source_com)
        );
        /*char*/r = target_control(target, alternative);
        if (either_is_fail(r)) {
          print_error(either_msg(r));
          break;
        }
        target = either_value(r);

        result = opt_new(userData_new(roots, source_path, target));
        break;
      }
    }
  }

  return result;
}
