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

struct f_Sell{
  size_t nick;
  char *tmp;
  void *(*fn)(Sell *this, int n);
};

static Sell *_sell_new(size_t nick) {
  Sell *this = MALLOC(Sell);
  this->nick = nick;
  this->tmp = "";
  this->fn = ext_fn;
  return this;
}

size_t sell_nick(Sell *this) {
  return this->nick;
}

char *sell_tmp(Sell *this) {
  return this->tmp;
}

void *(*sell_fn(Sell *this))(Sell *this, int n) {
  return this->fn;
}

void sell_set_fn(Sell *this, void *(*value)(Sell *this, int n)) {
  this->fn = value;
}

Js *sell_to_js(Sell *this) {
  // Arr[Js]
  Arr *js = arr_new();
  arr_push(js, js_wl((long)this->nick));
  arr_push(js, js_ws(this->tmp));
  return js_wa(js);
}

struct f_Buy{
  size_t nick;
  double quote;
  Opt-Person *friend;
};

Buy *buy_new(size_t nick, double quote, Opt-Person *friend) {
  Buy *this = MALLOC(Buy);
  this->nick = nick;
  this->quote = quote;
  this->friend = friend;
  return this;
}

double buy_quote(Buy *this) {
  return this->quote;
}

void buy_set_quote(Buy *this, double value) {
  this->quote = value;
}

Opt-Person *buy_friend(Buy *this) {
  return this->friend;
}

Js *buy_to_js(Buy *this) {
  // Arr[Js]
  Arr *js = arr_new();
  arr_push(js, js_wl((long)this->nick));
  arr_push(js, js_wd(this->quote, 4));
  arr_push(js, opt_is_empty(this->friend)
    ? js_wn()
    : person_to_js(this->friend)
  );
  return js_wa(js);
}

Buy *buy_fom_js(Js *js) {
  // Arr[Js]
  Arr *a = js_ra(js);
  Js **p = (Js **)arr_start(a);
  Buy *this = MALLOC(Buy);
  this->nick = (size_t)js_rl(*p++);
  this->quote = js_rd(*p++);
  this->friend = js_is_null(*p)
      ? p++? opt_empty(): NULL
      : opt_new(person_from_js(*p++))
  ;
  return this;
}

/*--*/

some text


