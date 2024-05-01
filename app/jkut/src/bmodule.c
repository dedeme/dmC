// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "bmodule.h"

  // NOTE: arr, dic and str are used in st_reader/read_symbol
  //       and all bmods in reader/read_main_block;
static char *bmods[] = {
  "arr", "b64", "bytes", "client", "cryp", "dic", "domo", "iter",
  "js", "math", "storage", "str", "sys", "time", "timer", "ui",
  NULL
};

static char *md_arr[] = {
  "all", "any", "cat", "concat", "clear", "copy", "drop", "dropWhile",
  "duplicates", "each", "eachIx", "eachSync", "filter", "filterIn", "find",
  "fromIter", "index", "insert", "insertArr", "join", "map", "mk", "peek",
  "pop", "reduce", "remove", "removeRange", "push", "reverse", "reverseIn",
  "shift", "shuffle", "size", "sort", "take", "takeWhile", "toIter",
  "unshift",
  NULL
};

static char *md_b64[] = {
  "decode", "decodeBytes", "encode", "encodeBytes",
  NULL
};

static char *md_bytes[] = {
  "add", "drop", "fromArr", "fromStr", "mk", "size", "take",
  "toArr", "toStr",
  NULL
};

static char *md_client[] = {
  "authentication", "connect", "crypPass", "init", "level",
  "longRun", "sessionId", "send", "userName",
  NULL
};

static char *md_cryp[] = {
  "genK", "key", "encode", "decode",
  NULL
};

static char *md_dic[] = {
  "copy", "fromArr", "fromIter", "get", "hasKey", "keys", "put", "remove",
  "size", "toArr", "toIter", "values",
  NULL
};

static char *md_domo[] = {
  "mk",
  NULL
};

static char *md_iter[] = {
  "all", "any", "cat", "count", "drop", "dropWhile", "empty", "each", "eachIx",
  "eachSync", "filter", "find", "index", "map", "hasNext", "mk", "next",
  "reduce", "take", "takeWhile", "toStr", "unary",
  NULL
};

static char *md_js[] = {
  "r", "w",
  NULL
};

static char *md_math[] = {
  "eq", "fromEn", "fromHex", "fromIso", "fromStr", "isDigits", "rnd", "rndi",
  "round", "toEn", "toFix", "toHex", "toHexFix", "toInt", "toIso", "toStr",
  NULL
};

static char *md_storage[] = {
  "del", "get", "keys", "put",
  NULL
};

static char *md_str[] = {
  "great", "ends", "escape", "fmt", "fromCodePoints", "fromUtf8", "index",
  "indexFrom", "lastIndex", "len", "less", "ltrim", "replace", "rtrim",
  "split", "splitTrim", "starts", "toCodePoints", "toLower", "toUpper",
  "toUtf8", "trim", "unescape",
  NULL
};

static char *md_sys[] = {
  "asBool", "assert", "test", "toStr", "type",
  NULL
};

static char *md_time[] = {
  "addDays", "day", "dfDays", "eqDay", "format", "fromClock", "fromClockOp",
  "fromDate", "fromEn", "fromEnOp", "fromIso", "fromIsoOp", "fromStr",
  "fromStrOp", "hour", "millisecond", "minute", "month", "mk", "mkDate", "now",
  "second", "toDate", "toEn", "toIso", "toStr", "weekday", "year", "yearDay",
  NULL
};

static char *md_timer[] = {
  "delay", "mk", "run", "stop",
  NULL
};

static char *md_ui[] = {
  "alert", "beep", "changePoint", "confirm", "field", "hrule", "ifiles",
  "img", "led", "lightImg", "link", "mouseX", "mouseY", "pass", "prompt",
  "q", "qOp", "qq", "select", "upload", "upTop", "url",
  NULL
};

static int exists (char **arr, char *value) {
  char **p = arr;
  while (*p) if (!strcmp(*p++, value)) return TRUE;
  return FALSE;
}

int bmodule_exists (char *md_sym) {
  return exists(bmods, md_sym);
}

int bmodule_has (char *md_sym, char *fn_sym) {
  if (!strcmp(md_sym, "arr") && exists(md_arr, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "b64") && exists(md_b64, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "bytes") && exists(md_bytes, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "client") && exists(md_client, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "cryp") && exists(md_cryp, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "dic") && exists(md_dic, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "domo") && exists(md_domo, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "iter") && exists(md_iter, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "js") && exists(md_js, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "math") && exists(md_math, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "storage") && exists(md_storage, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "str") && exists(md_str, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "sys") && exists(md_sys, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "time") && exists(md_time, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "timer") && exists(md_timer, fn_sym)) return TRUE;
  if (!strcmp(md_sym, "ui") && exists(md_ui, fn_sym)) return TRUE;
  return FALSE;
}

