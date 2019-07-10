// Copyright 10-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Conf.h"

/* .
# Exercise configuration
ConfExe:
  page: int
  number: int
===
# Unit configuration
ConfUnit:
  name: char *
  # Arr[ConfExe]
  exercises: Arr - ConfExe
===
# Book configuration
ConfBook:
  sortName: char *
  longName: char *
  units: Map - ConfUnit
*/

/*--*/

struct Conf_ConfExe {
  int page;
  int number;
};

ConfExe *confExe_new (int page, int number) {
  ConfExe *this = MALLOC(ConfExe);
  this->page = page;
  this->number = number;
  return this;
}

int confExe_page (ConfExe *this) {
  return this->page;
}

int confExe_number (ConfExe *this) {
  return this->number;
}

struct Conf_ConfUnit {
  char *name;
  Arr *exercises;
};

ConfUnit *confUnit_new (char *name, Arr *exercises) {
  ConfUnit *this = MALLOC(ConfUnit);
  this->name = name;
  this->exercises = exercises;
  return this;
}

char *confUnit_name (ConfUnit *this) {
  return this->name;
}

Arr *confUnit_exercises (ConfUnit *this) {
  return this->exercises;
}

struct Conf_ConfBook {
  char *sortName;
  char *longName;
  Map *units;
};

ConfBook *confBook_new (char *sortName, char *longName, Map *units) {
  ConfBook *this = MALLOC(ConfBook);
  this->sortName = sortName;
  this->longName = longName;
  this->units = units;
  return this;
}

char *confBook_sortName (ConfBook *this) {
  return this->sortName;
}

char *confBook_longName (ConfBook *this) {
  return this->longName;
}

Map *confBook_units (ConfBook *this) {
  return this->units;
}

/*--*/

