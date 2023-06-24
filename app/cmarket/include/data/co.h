// Copyright 05-06-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Company data.

#ifndef DATA_CO_H
  #define DATA_CO_H

typedef struct co_Co {
    char *nick;
    int is_selected;
  } Co;

/// Constructor.
///   nick: Identifier of company.
///   is_selected: 'TRUE' if company is selected to operate.
Co *co_new (char *nick, int is_selected);

///
char *co_to_js (Co *this);

///
Co *co_from_js (char *js);

#endif
