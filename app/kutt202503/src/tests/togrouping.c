// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "tests/togrouping.h"
#include "kut/arr.h"
#include "kut/buf.h"
#include "data/tp.h"
#include "comp/ogrouping.h"


char *to_str(Arr *a) {
  Buf *bf = buf_new();
  EACH(a, Tp, t) {
    Buf *bf2 = buf_new();
    EACH(t->e2, char, s) {
      buf_add(bf2, s);
    }_EACH
    buf_add(bf, str_f("  [%s[%s]]\n", t->e1, buf_str(bf2)));
  }_EACH
  return str_f("[\n%s]", buf_str(bf));
}

void togrouping_run() {
  puts("togrouping:");

  TEST(
    to_str(ogrouping_run(arr_new_from(
      tp_mk("A", arr_new_from("B", "C", NULL)),
      tp_mk("B", arr_new_from("D", NULL)),
      tp_mk("C", arr_new_from("E", "F", NULL)),
      tp_mk("D", arr_new()),
      tp_mk("E", arr_new_from("G", NULL)),
      tp_mk("F", arr_new()),
      tp_mk("G", arr_new()),
      NULL
    ))),
    to_str(arr_new_from(
      tp_mk("A", arr_new_from("A", NULL)),
      tp_mk("B", arr_new_from("B", "A", NULL)),
      tp_mk("C", arr_new_from("C", "A", NULL)),
      tp_mk("D", arr_new_from("D", "B", "A", NULL)),
      tp_mk("E", arr_new_from("E", "C", "A", NULL)),
      tp_mk("F", arr_new_from("F", "C", "A", NULL)),
      tp_mk("G", arr_new_from("G", "E", "C", "A", NULL)),
      NULL
    ))
  );

  TEST(
    to_str(ogrouping_run(arr_new_from(
      tp_mk("A", arr_new_from("B", "F", NULL)),
      tp_mk("B", arr_new_from("C", NULL)),
      tp_mk("C", arr_new_from("D", "E", NULL)),
      tp_mk("D", arr_new()),
      tp_mk("E", arr_new_from("D", NULL)),
      tp_mk("F", arr_new_from("B", "G", NULL)),
      tp_mk("G", arr_new()),
      NULL
    ))),
    to_str(arr_new_from(
      tp_mk("A", arr_new_from("A", NULL)),
      tp_mk("B", arr_new_from("B", "A", "F", NULL)),
      tp_mk("C", arr_new_from("C","B", "A", "F", NULL)),
      tp_mk("D", arr_new_from("D","C", "E", "B", "A", "F", NULL)),
      tp_mk("E", arr_new_from("E", "C", "B", "A", "F", NULL)),
      tp_mk("F", arr_new_from("F", "A", NULL)),
      tp_mk("G", arr_new_from("G", "F", "A", NULL)),
      NULL
    ))
  );

  TEST(
    to_str(ogrouping_run(arr_new_from(
      tp_mk("A", arr_new_from("B", "F", NULL)),
      tp_mk("B", arr_new_from("C", NULL)),
      tp_mk("C", arr_new_from("D", "E", NULL)),
      tp_mk("D", arr_new()),
      tp_mk("E", arr_new_from("G", NULL)),
      tp_mk("F", arr_new_from("B", "G", NULL)),
      tp_mk("G", arr_new_from("C", NULL)),
      NULL
    ))),
    to_str(arr_new_from(
      tp_mk("A", arr_new_from("A", NULL)),
      tp_mk("B", arr_new_from("B", "A" "F", NULL)),
      tp_mk("C", arr_new_from("C", "B", "G", "A", "F", "E", NULL)),
      tp_mk("D", arr_new_from("D", "C", "B", "G", "A", "F", "E", NULL)),
      tp_mk("E", arr_new_from("E", "C", "B", "G", "A", "F", NULL)),
      tp_mk("F", arr_new_from("F", "A", NULL)),
      tp_mk("G", arr_new_from("G", "E", "F", "C", "B", "A", NULL)),
      NULL
    ))
  );


  TEST(
    to_str(ogrouping_run(arr_new_from(
      tp_mk("A", arr_new_from("B", "F", "H", NULL)),
      tp_mk("B", arr_new_from("I", "C", NULL)),
      tp_mk("C", arr_new_from("D", "E", NULL)),
      tp_mk("D", arr_new()),
      tp_mk("E", arr_new_from("G", NULL)),
      tp_mk("F", arr_new_from("B", "G", NULL)),
      tp_mk("G", arr_new_from("C", NULL)),
      tp_mk("H", arr_new_from("D", "I", NULL)),
      tp_mk("I", arr_new_from("F", NULL)),
      NULL
    ))),
    to_str(arr_new_from(
      tp_mk("A", arr_new_from("A", NULL)),
      tp_mk("B", arr_new_from("B","A","F","I","H", NULL)),
      tp_mk("C", arr_new_from("C","B","G","A","F","I","H","E", NULL)),
      tp_mk("D", arr_new_from("D","C","H","B","G","A","F","I","E", NULL)),
      tp_mk("E", arr_new_from("E","C","B","G","A","F","I","H", NULL)),
      tp_mk("F", arr_new_from("F","A","I","B","H", NULL)),
      tp_mk("G", arr_new_from("G","E","F","C","B","A","I","H", NULL)),
      tp_mk("H", arr_new_from("H", "A", NULL)),
      tp_mk("I", arr_new_from("I","B","H","A","F", NULL)),
      NULL
    ))
  );

  TEST(
    to_str(ogrouping_run(arr_new_from(
      tp_mk("Main", arr_new_from("Clock", "Client", "Barber", NULL)),
      tp_mk("Clock", arr_new_from("Cts", NULL)),
      tp_mk("Cts", arr_new()),
      tp_mk("Client", arr_new_from("Cts", "Shop", NULL)),
      tp_mk("Shop", arr_new()),
      tp_mk("Barber", arr_new_from("Shop", "Clock", "Cts", NULL)),
      NULL
    ))),
    to_str(arr_new_from(
      tp_mk("Main", arr_new_from("Main", NULL)),
      tp_mk("Clock", arr_new_from("Clock", "Main", "Barber", NULL)),
      tp_mk("Cts", arr_new_from("Cts", "Clock", "Client", "Barber", "Main", NULL)),
      tp_mk("Client", arr_new_from("Client", "Main", NULL)),
      tp_mk("Shop", arr_new_from("Shop", "Client", "Barber", "Main", NULL)),
      tp_mk("Barber", arr_new_from("Barber", "Main", NULL)),
      NULL
    ))
  );

  puts("  finished");
}
