// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests.h"

/// 'nicks' is an Arr[char]
void tests_list_nicks(Arr *nicks, char *nick_model, int qdays, int qnicks) {
  EACH(nicks, char, n)
    printf("%s-", n);
  _EACH
  printf("\nNick model: %s\n", nick_model);
  printf("Days number: %d\n", qdays);
  printf("Nicks number: %d\n", qnicks);
}
