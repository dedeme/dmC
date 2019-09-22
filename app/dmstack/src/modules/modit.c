// Copyright 11-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modit.h"
#include "Reader.h"

static Token *read (char *id, char *prg) {
  return reader_process(reader_new(str_f("", id), prg, 1, 0));
}

static Token *new () {
  return read("NEW",
    "prg =; dup prg; prg:& tp,new3"
  );
}

static Token *empty () {
  return read("EMPTY",
    "0 () 0 tp,new3"
  );
}

static Token *unary () {
  return read("UNARY",
   "() swap lst,unary () tp,new3"
  );
}

static Token *from () {
  return read("FROM",
    "dup size 0 tp,new3 "
    "( Ob =; Ob; Ob 1 get; Ob 2 get " // Ob sz i
    "  (pop; pop; ()) else "
    "  (i =; Ob =; Ob 0 get i get lst,unary; Ob 2 i 1 + set; pop;) "
    "  (i =; sz =; i; i sz <) if ) "
    "it,new"
  );
}

static Token *range () {
  return read("RANGE",
   "tp,new2 "
   "( (pop; ()) "
   "  else "
   "  (Ob =; Ob 0 get r =; Ob 0 r 1 + set; pop; r lst,unary) "
   "  (Ob = Ob; Ob 0 get Ob 1 get <) "
   "  if) "
   "it,new"
  );
}

static Token *range0 () {
  return read("RANGE0",
   "0 swap it,range"
  );
}

static Token *has () {
  return read("HAS",
    "1 get size"
  );
}

static Token *peek () {
  return read("PEEK",
    "(\"Iterator is empty\" fail) else (1 get 0 get) (dup it,has?) if"
  );
}

static Token *next () {
  return read("NEXT",
    "(\"Iterator is empty\" fail) else "
    "(It =; It 1 get 0 get; It 1 :It 0 get It 2 get run: set; pop) "
    "(dup it,has?) if"
  );
}

static Token *plus () {
  return read("PLUS",
    "tp,new2 "
    "( Its =; Its 1 get; Its 0 get" // It2 It1
    "  ( pop; "
    "    (pop; ()) else "
    "    (it,next lst,unary) (dup it,has?) if "
    "  ) else "
    "  (it,next lst,unary; swap; pop) (dup it,has?) if) "
    "it,new"
  );
}


static Token *drop () {
  return read("PUSH",
    "swap; 0 " // i
    "(i =; dup; it,next; pop; 1 i +) "
    "(i =; It =; n =; n; It; i; i n < It it,has? &&) while; "
    "pop; swap; pop"
  );
}

static Token *dropf () {
  return read("PUSH",
    "swap "
    "(dup it,next; pop) "
    "( (0) else "
    "  (It =; fn =; fn:&; It; dup it,peek fn) (dup it,has?) if) "
    "while; swap; pop"
  );
}

static Token *filter () {
  return read("FILTER",
    "tp,new2 "
    "( Tp =; Tp 1 get; Tp 0 get "  // fn It
    "  (dup it,next; pop) "
    "  ( (0) else "
    "    (It =; fn =; It it,peek fn r =; fn:& It r !) (dup it,has?) if) "
    "  while "
    "  (pop; pop; ()) else "
    "  (It =; pop; It it,next lst,unary) (dup it,has?) if) "
    "it,new"
  );
}

static Token *map () {
  return read("MAP",
    "tp,new2 " // It fn
    "( "
    "  (pop; ()) "
    "  else "
    "  (Ob =; Ob 0 get it,next; Ob 1 get run R =; R lst,unary) "
    "  (dup; 0 get it,has?) "
    "  if) "
    "it,new"
  );
}

static Token *map2 () {
  return read("MAP2",
    "Fn2 = Fn1 = It = "
    "It (it,next it,unary) (dup it,has?) if It1 = "
    "It1 Fn1 it,map Itm1 = "
    "It Fn2 it,map Itm2 = "
    "Itm1 Itm2 it,+"
  );
}

static Token *push () {
  return read("PUSH",
    "it,unary it,+"
  );
}

static Token *push0 () {
  return read("PUSH0",
    "it,unary swap it,+"
  );
}

static Token *take () {
  return read("TAKE",
    "0 tp,new3 " // It n i
    "( (pop; ()) "
    "  else "
    "  (Ob =; Ob 2 :: Ob 2 get 1 + :: set; 0 get it,next lst,unary) "
    "  (Ob = Ob; Ob 2 get Ob 1 get < Ob 0 get it,has? &&) "
    "  if) "
    "it,new"
  );
}

static Token *takef () {
  return read("TAKEF",
    "tp,new2 " // It fn
    "( (pop; ()) "
    "  else "
    "  (Ob =; Ob 0 get it,next lst,unary) "
    "  ( (0) "
    "    else "
    "    (Ob =; Ob 0 get it,peek; Ob 1 get run r =; Ob r) "
    "    (dup 0 get it,has?) "
    "    if) "
    "  if) "
    "it,new"
  );
}

static Token *zip () {
  return read("ZIP",
    "tp,new2 " // It1 It2
    "( (pop; ())"
    "  else"
    "  (Ob =; Ob 0 get it,next; Ob 1 get it,next; tp,new2; lst,unary)"
    "  (Ob =; Ob; Ob 0 get it,has? Ob 1 get it,has? &&)"
    "  if)"
    "it,new "
  );
}

static Token *zip3 () {
  return read("ZIP3",
    "tp,new3 " // It1 It2 It3
    "( (pop; ())"
    "  else"
    "  ( Ob =; Ob 0 get it,next; Ob 1 get it,next; Ob 2 get it,next; "
    "    tp,new3; lst,unary)"
    "  (Ob =; Ob; Ob 0 get it,has? Ob 1 get it,has? && Ob 2 get it,has? &&)"
    "  if)"
    "it,new "
  );
}

static Token *all () {
  return read("ALL",
    "swap "
    "( (pop; pop; 1; break) "
    "  else "
    "  ( (pop; pop; 0; break) (It = fn? =; It it,next fn? r =; fn?:& It r !) "
    "    if) "
    "  (dup it,has?) "
    "  if)"
    "loop "
  );
}

static Token *any () {
  return read("ANY",
    "swap "
    "( (pop; pop; 0; break) "
    "  else "
    "  ( (pop; pop; 1; break) (It = fn? =; It it,next fn? r =; fn?:& It r) "
    "    if) "
    "  (dup it,has?) "
    "  if)"
    "loop "
  );
}

static Token *count () {
  return read("COUNT",
    "0 (pop 1 +) it,reduce"
  );
}

static Token *duplicates () {
  return read("DUPLICATES",
    "lst,new lst,new tp,new2 tp,new3 (0) + " // (It fn (Dup Res) ie) Ob =
    "( dup; Ob =; 3 Ob 0 get it,next set; "
    "  ( ( dup; Ob =; Ob 2 get 1 get; Ob 3 get; lst,push; pop) "
    "    else "
    "    ( dup; Ob =; Ob 2 get 0 get; Ob 3 get; lst,push; pop) "
    "    ( dup 2 get 1 get " // Res
    "      (e =; dup; Ob =; e; Ob 3 get; Ob 1 get; run) " // (e ie fn)
    "      lst,any? "
    "    ) "
    "    if "
    "  ) "
    "  ( dup 2 get 0 get " // Dup
    "    (e =; dup; Ob =; e; Ob 3 get; Ob 1 get; run) " // (e ie fn)
    "    lst,any? ! "
    "  ) if) "
    "(dup; 0 get; it,has?) "
    "while "
    "2 get"
  );
}

static Token *each () {
  return read("EACH",
    "swap "
    "(It =; fn =; It it,next fn; fn:&; It) (dup it,has?) while "
    "pop; pop"
  );
}

static Token *eachIx () {
  return read("EACHIX",
    "Fn = It = 0 Fn It "
    "(It = fn = i =; It it,next i fn; 1 i + fn:& It ) "
    "(dup it,has?) while "
    "pop; pop; pop"
  );
}

static Token *equals () {
  return read("EQUALS",
    "(==) it,eq?"
  );
}

static Token *notequals () {
  return read("NOTEQUALS",
    "(==) it,neq?"
  );
}

static Token *eq () {
  return read("EQ",
    "1 " // It1 It2 fn r
    "( (break) "
    "  else "
    "  ( "
    "    (pop; 0; break) "
    "    else "
    "    () "
    "    (r = fn = It2 = It1 =; It1 It2 fn:& r; It1 it,next It2 it,next fn) "
    "    if) "
    "  (r = Fn = It2 = It1 =; It1 It2 Fn r; It1 it,has? It2 it,has? &&) "
    "  if) "
    "loop "
    "(pop; pop; pop; 0) else "
    "(Fn = It2 = It1 =; It1 it,has? It2 it,has? || !) (nop) if"
  );
}

static Token *neq () {
  return read("NEQ",
    "it,eq? !"
  );
}

static Token *find () {
  return read("FIND",
    "( (pop; pop; (); break) "
    "  else "
    "  ( (Fn = It =; It Fn; It it,next pop)"
    "    else "
    "    (pop; It =; It it,peek lst,unary; break) "
    "    (fn? = It =; It it,peek fn? r =; It fn?:& r) "
    "    if) "
    "  (Fn = It =; It Fn; It it,has?) "
    "  if) "
    "loop"
  );
}

static Token *iindex () {
  return read("INDEX",
    "0 "
    "( (pop; pop; pop; -1; break) "
    "  else "
    "  ( (1 +)"
    "    else "
    "    (c =; pop; pop; c; break) "
    "    (c = fn? = It =; It fn?:& c; It it,next fn?) "
    "    if) "
    "  (c = Fn = It =; It Fn c; It it,has?) "
    "  if) "
    "loop"
  );
}

static Token *lastIndex () {
  return read("LASTINDEX",
    "Fn = It =; -1; 0; Fn "
    "It "
    "( (Fn =; 1 +; Fn) "
    "  else "
    "  (Fn =; c =; pop; c; c 1 +; Fn) "
    "  (e =; fn? =; fn?:&; e fn?) "
    "  if) "
    "it,each "
    "pop; pop"
  );
}

static Token *reduce () {
  return read("REDUCE",
    "(fn = Seed = It =; Seed It it,next fn R =; It R fn:&) "
    "(F = S = It =; It S F; It it,has?) "
    "while "
    "pop; swap; pop"
  );
}

static Token *to () {
  return read("TO",
    "lst,new (lst,push) it,reduce"
  );
}

static Token *shuffle () {
  return read("SUFFLE",
    "it,to lst,shuffle it,from"
  );
}

static Token *reverse () {
  return read("REVERSE",
    "it,to lst,reverse it,from"
  );
}

static Token *sort () {
  return read("SORT",
    "swap it,to swap lst,sort it,from"
  );
}

static Token *box () {
  return read("BOX",
    "dup lst,shuffle it,from tp,new2 " // List It
    "( "
    "  ( Ob = "
    "    Ob 1 Ob 0 get lst,shuffle it,from set "
    "      1 get it,next lst,unary) "
    "  else "
    "  (1 get it,next lst,unary) "
    "  (dup 1 get it,has?) "
    "  if) "
    "it,new"
  );
}

Map *modit_mk (void) {
  // Map<Token>
  Map *r = map_new();

  map_put(r, "new", new());
  map_put(r, "empty", empty());
  map_put(r, "unary", unary());
  map_put(r, "from", from());
  map_put(r, "range", range());
  map_put(r, "range0", range0());
  map_put(r, "has?", has());
  map_put(r, "peek", peek());
  map_put(r, "next", next());

  map_put(r, "+", plus());
  map_put(r, "drop", drop());
  map_put(r, "dropf", dropf());
  map_put(r, "filter", filter());
  map_put(r, "map", map());
  map_put(r, "map2", map2());
  map_put(r, "push", push());
  map_put(r, "push0", push0());
  map_put(r, "take", take());
  map_put(r, "takef", takef());
  map_put(r, "zip", zip());
  map_put(r, "zip3", zip3());

  map_put(r, "all?", all());
  map_put(r, "any?", any());
  map_put(r, "count", count());
  map_put(r, "duplicates", duplicates());
  map_put(r, "each", each());
  map_put(r, "eachIx", eachIx());
  map_put(r, "==", equals());
  map_put(r, "!=", notequals());
  map_put(r, "eq?", eq());
  map_put(r, "find", find());
  map_put(r, "index", iindex());
  map_put(r, "lastIndex", lastIndex());
  map_put(r, "neq?", neq());
  map_put(r, "reduce", reduce());
  map_put(r, "to", to());


  map_put(r, "shuffle", shuffle());
  map_put(r, "reverse", reverse());
  map_put(r, "sort", sort());

  map_put(r, "box", box());

  return r;
}
