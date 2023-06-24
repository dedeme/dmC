// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/qsv.h"
#include "kut/DEFS.h"

struct qsv_Qsv {
  char *(*id)(void);
  char *(*name)(void);
  int (*with_diary)(void);
  // Rs<char>
  Rs *(*download_diary)(void);
  // <Arr<Kv<double>>, Arr<char>>
  Tp *(*read_diary)(char *web_page);
  int (*with_historic)(void);
  // Rs<char>
  Rs *(*download_historic)(char *co_code);
  // <Opt<Qs>, Arr<char>>
  Tp *(*read_historic)(char *web_page);
};

Qsv *qsv_new (
  char *(*id)(void),
  char *(*name)(void),
  int (*with_diary)(void),
  // Rs<char>
  Rs *(*download_diary)(void),
  // <Arr<Kv<double>>, Arr<char>>
  Tp *(*read_diary)(char *web_page),
  int (*with_historic)(void),
  // Rs<char>
  Rs *(*download_historic)(char *co_code),
  // <Opt<Qs>, Arr<char>>
  Tp *(*read_historic)(char *web_page)
) {
  Qsv *this = MALLOC(Qsv);
  this->id = id;
  this->name = name;
  this->with_diary = with_diary;
  this->download_diary = download_diary;
  this->read_diary = read_diary;
  this->with_historic = with_historic;
  this->download_historic = download_historic;
  this->read_historic = read_historic;
  return this;
}

char *qsv_id (Qsv *this) {
  return this->id();
}

char *qsv_name (Qsv *this) {
  return this->name();
}

int qsv_with_diary (Qsv *this) {
  return this->with_diary();
}

// Rs<char>
Rs *qsv_download_diary (Qsv *this) {
  return this->download_diary();
}

// <Arr<Kv<double>>, Arr<char>>  [[[co, value]...], [err...]]
Tp *qsv_read_diary (Qsv *this, char *web_page) {
  return this->read_diary(web_page);
}

int qsv_with_historic (Qsv *this) {
  return this->with_historic();
}

// Rs<char>
Rs *qsv_download_historic (Qsv *this, char *co_code) {
  return this->download_historic(co_code);
}

// <Opt<Qs>, Arr<char>> [Quotes, [err...]]
Tp *qsv_read_historic (Qsv *this, char *web_page) {
  return this->read_historic(web_page);
}
