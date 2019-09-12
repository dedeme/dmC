// Copyright 11-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modit.h"
#include "Reader.h"

static Token *read (char *id, char *prg) {
  return reader_process(reader_new(str_f("[IT-%s]", id), prg, 1, 0));
}

Token *new () {
  return read("NEW",
    "prg =; stk.dup prg; prg:& lst.tp3"
  );
}

Token *empty () {
  return read("EMPTY",
    "0 () 0 lst.tp3"
  );
}

Token *from () {
  return read("FROM",
    "stk.dup size 0 lst.tp3 "
    "( Ob =; Ob 0 get Ls =; Ob 1 get sz =; Ob 2 get i = "
    "  (()) "
    "  (Ls i get lst.unary; Ob 2 i 1 + set; stk.pop;) "
    "  (i sz <) else if ) "
    "it.new"
  );
}

Token *unary () {
  return read("UNARY",
   "() stk.swap lst.unary () lst.tp3"
  );
}

Token *has () {
  return read("HAS",
    "1 get size"
  );
}

Token *peek () {
  return read("PEEK",
    "It =; (\"Iterator is empty\" fail) (It 1 get 0 get) (It it.has) else if"
  );
}

Token *next () {
  return read("NEXT",
    "It =; "
    "(\"Iterator is empty\" fail) "
    "(It 1 get 0 get; It 1 :It 0 get It 2 get run: set; stk.pop) "
    "(It it.has) else if"
  );
}

Token *plus () {
  return read("PLUS",
    "lst.tp "
    "( Its =; Its 0 get It1 =; Its 1 get It2 ="
    "  (())"
    "  (It2 it.next lst.unary) (It2 it.has) else"
    "  (It1 it.next lst.unary) (It1 it.has) else if ) "
    "it.new"
  );
}


Token *drop () {
  return read("PUSH",
    "n =; It =; 0 i =; "
    "(It it.next; stk.pop; (i) ++) (i n < It it.has &&) while; It"
  );
}

Token *dropf () {
  return read("PUSH",
    "fn =; It =;"
    "(It it.next; stk.pop) ((0) (It it.peek fn) (It it.has) else if) while; It"
  );
}

Token *filter () {
  return read("FILTER",
    "lst.tp "
    "( Tp =; Tp 0 get It =; Tp 1 get fn = "
    "  (It it.next; stk.pop) "
    "  ((0) (It it.peek fn !) (It it.has) else if) while "
    "  (()) (It it.next lst.unary) (It it.has) else if) "
    "it.new"
  );
}

Token *push () {
  return read("PUSH",
    "it.unary it.+"
  );
}

Token *push0 () {
  return read("PUSH0",
    "it.unary stk.swap it.+"
  );
}

Token *all () {
  return read("ALL",
    "fn =; It =; 1 r =; "
    "((0 r # break) (It it.next fn !) if) (It it.has) while; r"
  );
}

Token *any () {
  return read("ANY",
    "fn =; It =; 0 r =; "
    "((1 r # break) (It it.next fn) if) (It it.has) while; r"
  );
}

Token *count () {
  return read("COUNT",
    "0 (stk.pop 1 +) it.reduce"
  );
}

Token *duplicates () {
  return read("DUPLICATES",
    "fn =; It =; lst.new Dup =; lst.new Res =; 0 e = "
    "( (Res e lst.push; stk.pop) "
    "  ((Dup e lst.push; stk.pop) (Dup (e fn) lst.any !) if) "
    "  (It it.next e #; Res (e fn) lst.any) else if) "
    "(It it.has) "
    "while; Dup Res lst.tp"
  );
}

Token *each () {
  return read("EACH",
    "fn =; It =; (It it.next fn) (It it.has) while"
  );
}

Token *eachIx () {
  return read("EACHIX",
    "fn =; It =; 0 i =; (It it.next i fn; (i) ++) (It it.has) while"
  );
}

Token *equals () {
  return read("EQUALS",
    "(==) it.eq"
  );
}

Token *notequals () {
  return read("EQUALS",
    "(==) it.neq"
  );
}

Token *eq () {
  return read("EQ",
    "fn =; It2 =; It1 = ; 1 r = "
    "((0 r #; break) (It1 it.next It2 it.next fn !) if) "
    "(It1 it.has It2 it.has &&) while "
    "(0) (It1 it.has It2 it.has || !) (r) else if"
  );
}

Token *find () {
  return read("FIND",
    "fn =; lst.new; stk.swap (e =; (e lst.push) (e fn) if) it.each"
  );
}

Token *iindex () {
  return read("INDEX",
    "fn =; It =; -1; 0 c = "
    "(((c) ++) (stk.pop c; break) (It it.next fn) else if) (It it.has) while"
  );
}

Token *lastIndex () {
  return read("LASTINDEX",
    "fn =; It =; -1 r =; 0 c = "
    "((c r #) (It it.next fn) if; (c) ++) (It it.has) while; r"
  );
}

Token *neq () {
  return read("EQ",
    "it.eq !"
  );
}

Token *reduce () {
  return read("REDUCE",
    "fn =; Seed =; It =; (Seed It it.next fn Seed #) (It it.has) while; Seed"
  );
}

Token *to () {
  return read("TO",
    "lst.new stk.swap (lst.push) it.each"
  );
}

Map *modit_mk (void) {
  // Map<Token>
  Map *r = map_new();

  map_put(r, "it.new", new());
  map_put(r, "it.empty", empty());
  map_put(r, "it.from", from());
  map_put(r, "it.unary", unary());
  map_put(r, "it.has", has());
  map_put(r, "it.peek", peek());
  map_put(r, "it.next", next());

  map_put(r, "it.+", plus());
  map_put(r, "it.drop", drop());
  map_put(r, "it.dropf", dropf());
  map_put(r, "it.filter", filter());
  map_put(r, "it.push", push());
  map_put(r, "it.push0", push0());

  map_put(r, "it.all", all());
  map_put(r, "it.any", any());
  map_put(r, "it.count", count());
  map_put(r, "it.duplicates", duplicates());
  map_put(r, "it.each", each());
  map_put(r, "it.eachIx", eachIx());
  map_put(r, "it.==", equals());
  map_put(r, "it.!=", notequals());
  map_put(r, "it.eq", eq());
  map_put(r, "it.find", find());
  map_put(r, "it.index", iindex());
  map_put(r, "it.lastIndex", lastIndex());
  map_put(r, "it.neq", neq());
  map_put(r, "it.reduce", reduce());
  map_put(r, "it.to", to());

  return r;
}
