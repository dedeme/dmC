// Copyright 21-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/StringReader.h"

struct StringReader {
  lineReader_Nline fnline;
  lineReader_Next_new fnext_new;
  FPROC ffree;
  int nchar;
  int nline;
  const char *text;
};

int fnline (void *reader) {
  struct StringReader *this = reader;
  return this->nline;
}

char *fnext_new(void *reader) {
  struct StringReader *this = reader;
  int nchar = this->nchar;
  const char *text = this->text;
  if (text[nchar]) {
    Buf *bf = buf_new();
    ++this->nline;
    int ix = str_cindex_from(text, '\n', nchar);
    if (ix == -1) {
      int len = strlen(text);
      buf_add(bf, text + nchar, len - nchar);
      buf_cadd(bf, '\n');
      this->nchar = strlen(text);
      return r;
    }
    ++ix;
    char *r = str_new(text);
    str_sub(&r, nchar, ix);
    this->nchar = ix;
    return r;
  }
  return str_new("");
}

LineReader *stringReader_reader(const char *s) {
  struct StringReader *this = malloc(sizeof(struct StringReader));
  this->fnline = fnline;
  this->fnext_new = fnext_new;
  this->ffree = ffree;
  this->nchar = 0;
  this->nline = 0;
  this->text = s;
  return (LineReader *)this;
}
