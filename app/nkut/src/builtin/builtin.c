// Copyright 06-Sep-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/builtin.h"
#include "kut/str.h"
#include "kut/file.h"
#include "kut/path.h"
#include "builtin/math.h"
#include "builtin/js.h"
#include "builtin/arr.h"
#include "builtin/file.h"
#include "builtin/bytes.h"
#include "builtin/str.h"
#include "builtin/thread.h"
#include "builtin/dic.h"
#include "builtin/timer.h"
#include "builtin/time.h"
#include "builtin/b64.h"
#include "builtin/sys.h"
#include "builtin/iter.h"
#include "builtin/cryp.h"

char *builtin_module_head (char *to_root) {
  return str_replace(
    "import * as math from '#_js/math.js';"
    "import * as js from '#_js/js.js';"
    "import * as arr from '#_js/arr.js';"
    "import * as file from '#_js/file.js';"
    "import * as bytes from '#_js/bytes.js';"
    "import * as str from '#_js/str.js';"
    "import * as thread from '#_js/thread.js';"
    "import * as dic from '#_js/dic.js';"
    "import * as timer from '#_js/timer.js';"
    "import * as time from '#_js/time.js';"
    "import * as b64 from '#_js/b64.js';"
    "import * as sys from '#_js/sys.js';"
    "import * as iter from '#_js/iter.js';"
    "import * as cryp from '#_js/cryp.js';"
    ,
    "#", to_root
  );
}

void builtin_generate (char *path) {
  file_mkdir(path);

    //--
    void write_builtin (char *id, char *(*fn)(void)) {
      file_write(path_cat(path, str_f("%s.js", id), NULL), fn());
    };
  write_builtin("math", math_bget);
  write_builtin("js", js_bget);
  write_builtin("arr", arr_bget);
  write_builtin("file", file_bget);
  write_builtin("bytes", bytes_bget);
  write_builtin("str", str_bget);
  write_builtin("thread", thread_bget);
  write_builtin("dic", dic_bget);
  write_builtin("timer", timer_bget);
  write_builtin("time", time_bget);
  write_builtin("b64", b64_bget);
  write_builtin("sys", sys_bget);
  write_builtin("iter", iter_bget);
  write_builtin("cryp", cryp_bget);
}
