// Copyright 30-Apr-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_all.h"
#include "Cpath.h"
#include "tests_imported.h"
#include "tests_program.h"
#include "tests_type.h"
#include "tests_builtins.h"
#include "tests_cpath.h"
#include "tests_rempty.h"
#include "tests_rimports.h"
#include "tests_rgenerics.h"
#include "tests_rextend.h"
#include "tests_rtype.h"
#include "tests_rnull.h"
#include "tests_rbool.h"
#include "tests_rnumber.h"
#include "tests_rstring.h"
#include "tests_rarr.h"
#include "tests_rmap.h"
#include "tests_rid.h"
#include "tests_rcast.h"
#include "tests_rgroup.h"
#include "tests_rnary.h"
#include "tests_rwith.h"

int main() {
  sys_init("minijs_tests");
  Achar *roots = achar_new();
  achar_add(roots, "data");
  cpath_init(roots);

  tests_imported();
  tests_program();
  tests_type();
  tests_builtins();
  tests_cpath();
  tests_rempty();
  tests_rimports();
  tests_rgenerics();
  tests_rextend();
  tests_rtype();
  tests_rnull();
  tests_rbool();
  tests_rnumber();
  tests_rstring();
  tests_rarr();
  tests_rmap();
  tests_rid();
  tests_rcast();
  tests_rgroup();
  tests_rnary();
  tests_rwith();

  return 0;
}
