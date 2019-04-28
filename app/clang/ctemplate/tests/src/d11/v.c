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

struct v_D11V{
  int age;
  char *tmp;
  Arr *group;
  double cash;
  Darr *historic;
  Person *friend;
  int big;
};

static D11V *_d11V_new(int age) {
  D11V *this = MALLOC(D11V);
  this->age = age;
  this->tmp = "abc";
  this->group = arr_new();
  this->cash = 2.4;
  this->historic = darr_new();
  this->friend = person_new();
  this->big = false;
  return this;
}

int d11V_age(D11V *this) {
  return this->age;
}

char *d11V_tmp(D11V *this) {
  return this->tmp;
}

Arr *d11V_group(D11V *this) {
  return this->group;
}

double d11V_cash(D11V *this) {
  return this->cash;
}

Darr *d11V_historic(D11V *this) {
  return this->historic;
}

Person *d11V_friend(D11V *this) {
  return this->friend;
}

int d11V_big(D11V *this) {
  return this->big;
}

Js *d11V_to_js(D11V *this) {
  // Arr[Js]
  Arr *js = arr_new();
  arr_push(js, js_wi((int)this->age));
  arr_push(js, js_ws(this->tmp));
  arr_push(js, arr_to_js(this->group, (FTO)person_to_js));
  arr_push(js, js_wd(this->cash));
  arr_push(js, darr_to_js(this->historic));
  arr_push(js, person_to_js(this->friend));
  arr_push(js, js_wb(this->big));
  return js_wa(js);
}

D11V *d11V_from_js(Js *js) {
  // Arr[Js]
  Arr *a = js_ra(js);
  Js **p = (Js **)arr_start(a);
  D11V *this = MALLOC(D11V);
  this->age = (char)js_ri(*p++);
  this->tmp = js_rs(*p++);
  this->group = arr_from_js(*p++, (FFROM)person_to_js);
  this->cash = js_rd(*p++);
  this->historic = darr_from_js(*p++);
  this->friend = person_from_js(*p++);
  this->big = (char)js_ri(*p++);
  return this;
}

/*--*/

some text



