// Copyright 5-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rnumber.h"
#include "ast/Value.h"
#include "lexer/rnumber.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

static void check_byte(Value *v, char *n) {
  assert(value_vtype(v) == VBYTE);
  assert(type_type(value_type(v)) == DATA);
  assert(!strcmp(type_id(value_type(v)), "Byte"));
  assert(arr_size(value_attachs(v)) == 0);

  if (strcmp(value_data(v), n)) {
    printf("Expected: %s\nActual:%s\n", n, value_data(v));
    assert(false);
  }
}

static void check_int(Value *v, char *n) {
  assert(value_vtype(v) == VINT);
  assert(type_type(value_type(v)) == DATA);
  assert(!strcmp(type_id(value_type(v)), "Int"));
  assert(arr_size(value_attachs(v)) == 0);

  if (strcmp(value_data(v), n)) {
    printf("Expected: %s\nActual:%s\n", n, value_data(v));
    assert(false);
  }
}

static void check_float(Value *v, char *n) {
  assert(value_vtype(v) == VFLOAT);
  assert(type_type(value_type(v)) == DATA);
  assert(!strcmp(type_id(value_type(v)), "Float"));
  assert(arr_size(value_attachs(v)) == 0);

  if (strcmp(value_data(v), n)) {
    printf("Expected: %s\nActual:%s\n", n, value_data(v));
    assert(false);
  }
}

void tests_rnumber() {
  puts("Reader: number");
  Tx *tx;
  Value *v;

  puts("--> Byte");
  tx = mk_tx("0t");
  rnumber(&v, tx);
  check_byte(v, "0");

  tx = mk_tx("1t");
  rnumber(&v, tx);
  check_byte(v, "1");

  tx = mk_tx("1234567890t");
  rnumber(&v, tx);
  check_byte(v, "1234567890");

  tx = mk_tx("1_234_567_890t");
  rnumber(&v, tx);
  check_byte(v, "1234567890");

  tx = mk_tx("00t");
  rnumber(&v, tx);
  check_byte(v, "0o0");

  tx = mk_tx("001234567t");
  rnumber(&v, tx);
  check_byte(v, "0o01234567");

  tx = mk_tx("0x0t");
  rnumber(&v, tx);
  check_byte(v, "0x0");

  tx = mk_tx("0x012456789abcdefABCDEFt");
  rnumber(&v, tx);
  check_byte(v, "0x012456789abcdefABCDEF");

  tx = mk_tx("0b0t");
  rnumber(&v, tx);
  check_byte(v, "0b0");

  tx = mk_tx("0b01t");
  rnumber(&v, tx);
  check_byte(v, "0b01");

  puts("--> Int");
  tx = mk_tx("0");
  rnumber(&v, tx);
  check_int(v, "0");

  tx = mk_tx("1");
  rnumber(&v, tx);
  check_int(v, "1");

  tx = mk_tx("1_234_567_890");
  rnumber(&v, tx);
  check_int(v, "1234567890");

  tx = mk_tx("1234567890");
  rnumber(&v, tx);
  check_int(v, "1234567890");

  tx = mk_tx("00");
  rnumber(&v, tx);
  check_int(v, "0o0");

  tx = mk_tx("001234567");
  rnumber(&v, tx);
  check_int(v, "0o01234567");

  tx = mk_tx("0x0");
  rnumber(&v, tx);
  check_int(v, "0x0");

  tx = mk_tx("0x012456789abcdefABCDEF");
  rnumber(&v, tx);
  check_int(v, "0x012456789abcdefABCDEF");

  tx = mk_tx("0b0");
  rnumber(&v, tx);
  check_int(v, "0b0");

  tx = mk_tx("0b01");
  rnumber(&v, tx);
  check_int(v, "0b01");

  puts("--> Float");
  tx = mk_tx(".");
  assert(tx_eq(tx, rnumber(&v, tx)));

  tx = mk_tx("_12");
  assert(tx_eq(tx, rnumber(&v, tx)));

  tx = mk_tx("0.e0");
  rnumber(&v, tx);
  check_float(v, "0.0e0");

  tx = mk_tx("0.E0");
  rnumber(&v, tx);
  check_float(v, "0.0e0");

  tx = mk_tx("0.e+0");
  rnumber(&v, tx);
  check_float(v, "0.0e+0");

  tx = mk_tx("0.E+0");
  rnumber(&v, tx);
  check_float(v, "0.0e+0");

  tx = mk_tx("0.e-0");
  rnumber(&v, tx);
  check_float(v, "0.0e-0");

  tx = mk_tx("0.e-112");
  rnumber(&v, tx);
  check_float(v, "0.0e-112");

  tx = mk_tx("0.1e0");
  rnumber(&v, tx);
  check_float(v, "0.1e0");

  tx = mk_tx("0.12E0");
  rnumber(&v, tx);
  check_float(v, "0.12e0");

  tx = mk_tx("0.10e+0");
  rnumber(&v, tx);
  check_float(v, "0.10e+0");

  tx = mk_tx("0.10E+0");
  rnumber(&v, tx);
  check_float(v, "0.10e+0");

  tx = mk_tx("0.1230e-0");
  rnumber(&v, tx);
  check_float(v, "0.1230e-0");

  tx = mk_tx("0.032134e-112");
  rnumber(&v, tx);
  check_float(v, "0.032134e-112");

  tx = mk_tx("10.e0");
  rnumber(&v, tx);
  check_float(v, "10.0e0");

  tx = mk_tx("1.E0");
  rnumber(&v, tx);
  check_float(v, "1.0e0");

  tx = mk_tx("34230.e+0");
  rnumber(&v, tx);
  check_float(v, "34230.0e+0");

  tx = mk_tx("34_230.e+0");
  rnumber(&v, tx);
  check_float(v, "34230.0e+0");

  tx = mk_tx("3450.E+0");
  rnumber(&v, tx);
  check_float(v, "3450.0e+0");

  tx = mk_tx("3_450.E+0");
  rnumber(&v, tx);
  check_float(v, "3450.0e+0");

  tx = mk_tx("1.e-0");
  rnumber(&v, tx);
  check_float(v, "1.0e-0");

  tx = mk_tx("22.e-112");
  rnumber(&v, tx);
  check_float(v, "22.0e-112");

  tx = mk_tx("1_111.1e0");
  rnumber(&v, tx);
  check_float(v, "1111.1e0");

  tx = mk_tx("1111.1e0");
  rnumber(&v, tx);
  check_float(v, "1111.1e0");

  tx = mk_tx("21.12E0");
  rnumber(&v, tx);
  check_float(v, "21.12e0");

  tx = mk_tx("0.10e+0");
  rnumber(&v, tx);
  check_float(v, "0.10e+0");

  tx = mk_tx("2222.10E+0");
  rnumber(&v, tx);
  check_float(v, "2222.10e+0");

  tx = mk_tx("6666666.1230e-0");
  rnumber(&v, tx);
  check_float(v, "6666666.1230e-0");

  tx = mk_tx("1_234_567_890.0123456789e-109");
  rnumber(&v, tx);
  check_float(v, "1234567890.0123456789e-109");

  puts("    Finished");
}
