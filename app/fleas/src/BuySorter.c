// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "BuySorter.h"

typedef struct buySorter_BsEntry BsEntry;
struct buySorter_BsEntry {
  double ponderation;
  Buy *buy;
};

static BsEntry *bsEntry_new(Buy *buy, double ponderation) {
  BsEntry *this = malloc(sizeof(BsEntry));
  this->buy = buy;
  this->ponderation = ponderation;
  return this;
}

struct buySorter_BuySorter {
  BsEntry **entries;
};

static BuySorter *_buySorter_new(BsEntry **entries) {
  BuySorter *this = malloc(sizeof(BuySorter));
  this->entries = entries;
  return this;
}

static void buySorter_free(BuySorter *this) {
  BsEntry **p = this->entries;
  while (*p) {
    free(*p++);
  }
  free(this->entries);
  free(this);
};

BuySorter *buySorter_new(int companies_number) {
  BsEntry **entries = calloc(companies_number + 1, sizeof(BsEntry*));
  return _buySorter_new(entries);
}

void buySorter_add(BuySorter *this, Buy *buy, int ponderation) {
  BsEntry *new = bsEntry_new(buy, ponderation);
  BsEntry **p = this->entries;
  while (*p && (*p)->ponderation > ponderation) {
    ++p;
  }
  if (*p) {
    BsEntry **q = p;
    while (*q) {
      ++q;
    }
    while (q > p) {
      *q = *(q - 1);
      --q;
    }
  }
  *p = new;
}

// Returns an Arr[Buy]
Arr *buySorter_free_new(BuySorter *this) {
  // Arr[Buy]
  Arr *r = arr_new((FPROC)buy_free);
  BsEntry **p = this->entries;
  while (*p) {
    arr_push(r, (*p++)->buy);
  }
  buySorter_free(this);
  return r;
}
