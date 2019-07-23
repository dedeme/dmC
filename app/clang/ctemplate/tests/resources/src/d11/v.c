Some text

/* .
- D11V: serial
  age: int
  ---
  tmp: char *: "abc"
  # Some people
  group: Arr - Person : arr_new()
  cash: double : 2.4
  historic: Darr : darr_new()
  # A good friend
  friend: Person : person_new()
  big: bool: false
*/

some text

/*--*/

struct v_D11V {
  int age;
  char *tmp;
  Arr *group;
  double cash;
  Darr *historic;
  Person *friend;
  int big;
};

static D11V *_d11V_new (Gc *gc, int age) {
  D11V *this = gc_add(gc, malloc(sizeof(D11V)));
  this->age = age;
  this->tmp = "abc";
  this->group = arr_new();
  this->cash = 2.4;
  this->historic = darr_new();
  this->friend = person_new();
  this->big = false;
  return this;
}

int d11V_age (D11V *this) {
  return this->age;
}

char *d11V_tmp (D11V *this) {
  return this->tmp;
}

Arr *d11V_group (D11V *this) {
  return this->group;
}

double d11V_cash (D11V *this) {
  return this->cash;
}

Darr *d11V_historic (D11V *this) {
  return this->historic;
}

Person *d11V_friend (D11V *this) {
  return this->friend;
}

int d11V_big (D11V *this) {
  return this->big;
}

Js *d11V_to_js (Gc *gc, D11V *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  arr_push(js, js_wi(gcl, (int)this->age));
  arr_push(js, js_ws(gcl, this->tmp));
  arr_push(js, arr_to_js(gcl, this->group, (FTO)person_to_js));
  arr_push(js, js_wd(gcl, this->cash));
  arr_push(js, darr_to_js(gcl, this->historic));
  arr_push(js, person_to_js(gcl, this->friend));
  arr_push(js, js_wb(gcl, this->big));
  return gc_clean(gcl, js_wa(gc, js));
}

D11V *d11V_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  D11V *this = gc_add(gc, malloc(sizeof(D11V)));
  this->age = js_ri(*p++);
  this->tmp = js_rs(gc, *p++);
  this->group = arr_from_js(gc, *p++, (FFROM)person_from_js);
  this->cash = js_rd(*p++);
  this->historic = darr_from_js(gc, *p++);
  this->friend = person_from_js(gc, *p++);
  this->big = js_rb(*p++);
  return gc_clean(gcl, this);
}

/*--*/

some text



