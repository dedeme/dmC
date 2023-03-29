// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef SHOP_H
  #define SHOP_H

/// Initializes shop.
void shop_init (void);

/// Returns TRUE if time is previous to open the barbery.
int shop_is_preopen(void);

/// Returns TRUE if time is after to open the barbery.
int shop_is_open(void);

/// Returns a representation of barbery sits.
char *shop_sits_to_str(void);

/// Returns TRUE if barbery is full.
int shop_is_full(void);

/// Returns TRUE if barbery is empty.
int shop_is_empty(void);

/// Open the barbery.
void shop_open(void);

/// Close the barbery.
void shop_close(void);

/// Asigns a sit to client.
/// NOTE: There must be at least an empty sit.
void shop_client_take_a_sit(char *client);

/// Barber take a client.
/// NOTE: There must be at least one client.
char *shop_barber_take_a_client(void);

#endif
