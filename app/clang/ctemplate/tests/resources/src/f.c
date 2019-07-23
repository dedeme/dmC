Some text

/* .
# Commented line 1
# Commented line 2
- Sell :to
  nick: size_t
  ---
  tmp: char * : ""
  ---
  @ void *(*fn)(Sell *this, int n): ext_fn
===

Buy : serial
  - nick: size_t
  # Market quote
  @ quote: double 4
  friend: Opt - Person
*/

some text

/*--*/

struct f_Sell {
  size_t nick;
  char *tmp;
  void *(*fn)(Sell *this, int n);
};

static Sell *_sell_new (Gc *gc, size_t nick) {
  Sell *this = gc_add(gc, malloc(sizeof(Sell)));
  this->nick = nick;
  this->tmp = "";
  this->fn = ext_fn;
  return this;
}

size_t sell_nick (Sell *this) {
  return this->nick;
}

char *sell_tmp (Sell *this) {
  return this->tmp;
}

void *(*sell_fn (Sell *this))(Sell *this, int n) {
  return this->fn;
}

void sell_set_fn (Sell *this, void *(*value)(Sell *this, int n)) {
  this->fn = value;
}

Js *sell_to_js (Gc *gc, Sell *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  arr_push(js, js_wl(gcl, (long)this->nick));
  arr_push(js, js_ws(gcl, this->tmp));
  return gc_clean(gcl, js_wa(gc, js));
}

struct f_Buy {
  size_t nick;
  double4 *quote;
  Opt *friend;
};

Buy *buy_new (Gc *gc, size_t nick, double4 *quote, Opt *friend) {
  Buy *this = gc_add(gc, malloc(sizeof(Buy)));
  this->nick = nick;
  this->quote = quote;
  this->friend = friend;
  return this;
}

double4 *buy_quote (Buy *this) {
  return this->quote;
}

void buy_set_quote (Buy *this, double4 *value) {
  this->quote = value;
}

Opt *buy_friend (Buy *this) {
  return this->friend;
}

Js *buy_to_js (Gc *gc, Buy *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  arr_push(js, js_wl(gcl, (long)this->nick));
  arr_push(js, double4_to_js(gcl, this->quote));
  arr_push(js, opt_is_empty(this->friend)
    ? js_wn()
    : person_to_js(gcl, opt_get(this->friend))
  );
  return gc_clean(gcl, js_wa(gc, js));
}

Buy *buy_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  Buy *this = gc_add(gc, malloc(sizeof(Buy)));
  this->nick = (size_t)js_rl(*p++);
  this->quote = double4_from_js(gc, *p++);
  this->friend = js_is_null(*p)
      ? p++? opt_empty(): NULL
      : opt_new(person_from_js(gc, *p++))
  ;
  return gc_clean(gcl, this);
}

/*--*/

some text


