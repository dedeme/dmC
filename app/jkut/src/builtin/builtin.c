// Copyright 30-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/builtin.h"
#include "kut/str.h"
#include "kut/file.h"
#include "kut/path.h"
#include "builtin/iter.h"
#include "builtin/str.h"
#include "builtin/bytes.h"
#include "builtin/cryp.h"
#include "builtin/dic.h"
#include "builtin/timer.h"
#include "builtin/js.h"
#include "builtin/storage.h"
#include "builtin/sys.h"
#include "builtin/math.h"
#include "builtin/domo.h"
#include "builtin/ui.h"
#include "builtin/arr.h"
#include "builtin/time.h"
#include "builtin/client.h"
#include "builtin/b64.h"

char *builtin_module_head (char *to_root) {
  return str_replace(
    "import * as iter from '#_js/iter.js';"
    "import * as str from '#_js/str.js';"
    "import * as bytes from '#_js/bytes.js';"
    "import * as cryp from '#_js/cryp.js';"
    "import * as dic from '#_js/dic.js';"
    "import * as timer from '#_js/timer.js';"
    "import * as js from '#_js/js.js';"
    "import * as storage from '#_js/storage.js';"
    "import * as sys from '#_js/sys.js';"
    "import * as math from '#_js/math.js';"
    "import * as domo from '#_js/domo.js';"
    "import * as ui from '#_js/ui.js';"
    "import * as arr from '#_js/arr.js';"
    "import * as time from '#_js/time.js';"
    "import * as client from '#_js/client.js';"
    "import * as b64 from '#_js/b64.js';"
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
  write_builtin("iter", iter_bget);
  write_builtin("str", str_bget);
  write_builtin("bytes", bytes_bget);
  write_builtin("cryp", cryp_bget);
  write_builtin("dic", dic_bget);
  write_builtin("timer", timer_bget);
  write_builtin("js", js_bget);
  write_builtin("storage", storage_bget);
  write_builtin("sys", sys_bget);
  write_builtin("math", math_bget);
  write_builtin("domo", domo_bget);
  write_builtin("ui", ui_bget);
  write_builtin("arr", arr_bget);
  write_builtin("time", time_bget);
  write_builtin("client", client_bget);
  write_builtin("b64", b64_bget);
}
