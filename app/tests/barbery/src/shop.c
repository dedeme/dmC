// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "shop.h"
#include "kut/DEFS.h"
#include "kut/arr.h"
#include "kut/sys.h"

static int preopen = TRUE;
static int open = FALSE;
// <char>
static Arr *clients = NULL;
// <char>
static Arr *sits = NULL;

void shop_init (void) {
  clients = arr_new();
  sits = arr_new_from("", "", "", "", NULL);
}

int shop_is_preopen(void) {
  return preopen;
}

int shop_is_open(void) {
  return open;
}

char *shop_sits_to_str(void) {
  //
    char *sit_to_str(char *sit) {
      return *sit ? sit : "-";
    }
  return str_f("|%s|", arr_join(arr_map(sits, (FMAP)sit_to_str), "|"));
}

int shop_is_full(void) {
  return arr_size(clients) == 4;
}

int shop_is_empty(void) {
  return arr_empty(clients);
}

void shop_open(void) {
  preopen = FALSE;
  open = TRUE;
}

void shop_close(void) {
  open = FALSE;
}

void shop_client_take_a_sit(char *client) {
  int n_clients = arr_size(clients);
  if (n_clients >= 4)
    EXC_ILLEGAL_STATE("Barbery is full");

  int n = sys_rnd_i(4 - n_clients);

  int j = 0;
  int r = -1;
  for (int i = 0; i < arr_size(sits); ++i) {
    if (str_eq(arr_get(sits, i), "")) {
      if (j == n) {
        r = i;
        break;
      }
      ++j;
    }
  }
  if (r == -1)
    EXC_ILLEGAL_STATE("Sit not found");

  arr_push(clients, client);
  arr_set(sits, r, client);

  puts(str_f("Shop: %s", shop_sits_to_str()));
}

char *shop_barber_take_a_client(void) {
	if (arr_empty(clients))
		EXC_ILLEGAL_STATE("Barbery is empty");

	char *cl = arr_get(clients, 0);
  arr_remove(clients, 0);
  EACH(sits, char, c) {
    if (str_eq(c, cl)) {
      arr_set(sits, _i, "");
      break;
    }
  }_EACH

	return cl;
}

