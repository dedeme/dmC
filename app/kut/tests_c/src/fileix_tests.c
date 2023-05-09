// Copyright 01-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fileix_tests.h"
#include "kut/DEFS.h"
#include "fileix.h"
void fileix_tests () {
  puts (">>> fileix:");

  fileix_set_root("data");
  TEST(fileix_get_root(), "data");

  int main_ix = fileix_add(-1, "main");
  int t0_ix1 = fileix_add(main_ix, "t0");
  int l1_ix1 = fileix_add(main_ix, "lib/l1");
  int l2_ix1 = fileix_add(main_ix, "lib/l2");
  int l1_ix2 = fileix_add(l2_ix1, "l1");
  int l1_ix3 = fileix_add(l2_ix1, "lib/l1");


  TEST(fileix_to_str(main_ix), "/home/deme/dm/dmC/app/kut/tests_c/data/main");
  TEST(fileix_to_fail(main_ix), "/home/deme/dm/dmC/app/kut/tests_c/data/main.kut");
  TEST(fileix_to_str(t0_ix1), "/home/deme/dm/dmC/app/kut/tests_c/data/t0");
  TEST(fileix_to_fail(t0_ix1), "/home/deme/dm/dmC/app/kut/tests_c/data/t0.kut");
  TEST(fileix_to_str(l1_ix1), "/home/deme/dm/dmC/app/kut/tests_c/data/lib/l1");
  TEST(fileix_to_fail(l1_ix1), "/home/deme/dm/dmC/app/kut/tests_c/data/lib/l1.kut");

  TESTI(l1_ix1, l1_ix2);
  TESTI(l1_ix1, l1_ix3);
//  assert(str_index(rs_error(fileix_read(2215)), "Index out") != -1);
  TRY {
    fileix_read(2215);
    assert(FALSE);
  } CATCH(e) {
    assert(str_index(exc_msg(e), "Index out") != -1);
  } _TRY
  TEST(fileix_read(l1_ix1), "// l1\n");

  puts ("  ... Finished");
}
