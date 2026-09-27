// Copyright 17-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/arr.h"
#include "kut/js.h"
#include "data/docE.h"

static DocE *new (char type, char *id, char *code, int ln, char *doc) {
  DocE *this = MALLOC(DocE);
  this->type = type;
  this->id = id;
  this->code = code;
  this->ln = ln;
  this->doc = doc;
  return this;
}

DocE *docE_new_import (char *id, char *code, int ln, char *link) {
  return new ('i', id, code, ln, link);
}

DocE *docE_new_summary (char *text) {
  return new ('s', "", "", 0, text);
}

DocE *docE_new_type (char *id, char *code, int ln, char *doc) {
  return new ('t', id, code, ln, doc);
}

DocE *docE_new_value (char *id, char *code, int ln, char *doc) {
  return new ('v', id, code, ln, doc);
}

DocE *docE_new_enum (char *id, char *code, int ln, char *doc) {
  return new ('e', id, code, ln, doc);
}

DocE *docE_new_indexed (char *id, char *code, int ln, char *doc) {
  return new ('x', id, code, ln, doc);
}

DocE *docE_new_function (char *id, char *code, int ln, char *doc) {
  return new ('f', id, code, ln, doc);
}

DocE *docE_new_error (int ln, char *message) {
  return new ('r', "", "", ln, message);
}

char *docE_to_js (DocE *this) {
  return js_wa(arr_new_from(
    js_ws(str_new_c(this->type)),
    js_ws(this->id),
    js_ws(this->code),
    js_wi(this->ln),
    js_ws(this->doc),
    NULL
  ));
}

