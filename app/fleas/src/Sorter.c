// Copyright 21-Nov-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Sorter.h"

typedef struct sorter_Entry Entry;
struct sorter_Entry {
  double ponderation;
  void *element;
};

static Entry *entry_new(void *element, double ponderation) {
  Entry *this = malloc(sizeof(Entry));
  this->element = element;
  this->ponderation = ponderation;
  return this;
}

struct sorter_Sorter {
  Entry **entries;
  int size;
  void (*ffree)(void *);
};

Sorter *sorter_new(int size, void (*ffree)(void *)) {
  Sorter *this = malloc(sizeof(Sorter));
  this->entries = calloc(size, sizeof(Entry *));
  this->size = size;
  this->ffree = ffree;
  return this;
}

void sorter_add(Sorter *this, void *element, double ponderation) {
  Entry **es = this->entries;
  if (*es && (*es)->ponderation > ponderation) {
    this->ffree(element);
    return;
  }

  int c = 0;
  int size = this->size;
  for (;;) {
    ++c;
    ++es;
    if (c < size && (!*es || (*es)->ponderation < ponderation)) {
      *(es - 1) = *es;
    } else {
      *(es - 1) = entry_new(element, ponderation);
      break;
    }
  }
}

Arr *sorter_free_new(Sorter *this) {
  int size = this->size;
  Arr *r = arr_bf_new(size + 1, this->ffree);
  Entry **es = this->entries;
  REPEAT(size)
    if (*es) {
      arr_push(r, (*es)->element);
      free(*es);
    }
    ++es;
  _REPEAT
  free(this->entries);
  free(this);
  return r;
}
