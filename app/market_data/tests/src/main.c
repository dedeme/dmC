// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "expansion_tests.h"
#include "finanzas_tests.h"
#include "invertia_tests.h"
#include "estrategias_tests.h"
#include "infobolsa_tests.h"
#include "eleconomista_tests.h"
#include "libremercado_tests.h"
#include "pf_tests.h"
#include "trading_tests.h"


int main(int argc, char* args[]) {
  sys_init("market_data");

  expansion_tests();
  finanzas_tests();
  invertia_tests();
  estrategias_tests();
  infobolsa_tests();
  eleconomista_tests();
  libremercado_tests();
  pf_tests();
  trading_tests();

  sys_end();
  return 0;
}
