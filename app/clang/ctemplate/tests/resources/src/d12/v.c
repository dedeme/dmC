Some text

/* .
- D12V: serial
  age: int
  group: List - Iarr
  cash: float
  historic: Map - Darr
  ---
  tmp: char *: "abc"
  # Some people.
  #   Arr[Person]
  group2: Arr - Person : arr_new()
  cash2: double  : 2.4
  historic: Darr  : darr_new()
  # A good friend
  @ friend: Person : person_new()
  @ big: bool: false
  # Map[Darr]
  @ historic2: Map - Darr : map_new()
  ---
  int (*k2)(D12F2 *this): k2_ext
  @ Rs *(*test)(D12F2 *this, int money): test_ext
*/

some text

/*--*/

struct v_D12V {
  int age;
  List *group;
  float cash;
  Map *historic;
  char *tmp;
  Arr *group2;
  double cash2;
  Darr *historic;
  Person *friend;
  int big;
  Map *historic2;
  int (*k2)(D12F2 *this);
  Rs *(*test)(D12F2 *this, int money);
};

static D12V *_d12V_new (
  Gc *gc,
  int age,
  List *group,
  float cash,
  Map *historic
) {
  D12V *this = gc_add(gc, malloc(sizeof(D12V)));
  this->age = age;
  this->group = group;
  this->cash = cash;
  this->historic = historic;
  this->tmp = "abc";
  this->group2 = arr_new();
  this->cash2 = 2.4;
  this->historic = darr_new();
  this->friend = person_new();
  this->big = false;
  this->historic2 = map_new();
  this->k2 = k2_ext;
  this->test = test_ext;
  return this;
}

int d12V_age (D12V *this) {
  return this->age;
}

List *d12V_group (D12V *this) {
  return this->group;
}

float d12V_cash (D12V *this) {
  return this->cash;
}

Map *d12V_historic (D12V *this) {
  return this->historic;
}

char *d12V_tmp (D12V *this) {
  return this->tmp;
}

Arr *d12V_group2 (D12V *this) {
  return this->group2;
}

double d12V_cash2 (D12V *this) {
  return this->cash2;
}

Darr *d12V_historic (D12V *this) {
  return this->historic;
}

Person *d12V_friend (D12V *this) {
  return this->friend;
}

void d12V_set_friend (D12V *this, Person *value) {
  this->friend = value;
}

int d12V_big (D12V *this) {
  return this->big;
}

void d12V_set_big (D12V *this, int value) {
  this->big = value;
}

Map *d12V_historic2 (D12V *this) {
  return this->historic2;
}

void d12V_set_historic2 (D12V *this, Map *value) {
  this->historic2 = value;
}

int (*d12V_k2 (D12V *this))(D12F2 *this) {
  return this->k2;
}

Rs *(*d12V_test (D12V *this))(D12F2 *this, int money) {
  return this->test;
}

void d12V_set_test (D12V *this, Rs *(*value)(D12F2 *this, int money)) {
  this->test = value;
}

Js *d12V_to_js (Gc *gc, D12V *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  arr_push(js, js_wi(gcl, (int)this->age));
  arr_push(js, list_to_js(gcl, this->group, (FTO)iarr_to_js));
  arr_push(js, js_wd(gcl, (double)this->cash));
  arr_push(js, map_to_js(gcl, this->historic, (FTO)darr_to_js));
  arr_push(js, js_ws(gcl, this->tmp));
  arr_push(js, arr_to_js(gcl, this->group2, (FTO)person_to_js));
  arr_push(js, js_wd(gcl, this->cash2));
  arr_push(js, darr_to_js(gcl, this->historic));
  arr_push(js, person_to_js(gcl, this->friend));
  arr_push(js, js_wb(gcl, this->big));
  arr_push(js, map_to_js(gcl, this->historic2, (FTO)darr_to_js));
  return gc_clean(gcl, js_wa(gc, js));
}

D12V *d12V_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  D12V *this = gc_add(gc, malloc(sizeof(D12V)));
  this->age = js_ri(*p++);
  this->group = list_from_js(gc, *p++, (FFROM)iarr_from_js);
  this->cash = (float)js_rd(*p++);
  this->historic = map_from_js(gc, *p++, (FFROM)darr_from_js);
  this->tmp = js_rs(gc, *p++);
  this->group2 = arr_from_js(gc, *p++, (FFROM)person_from_js);
  this->cash2 = js_rd(*p++);
  this->historic = darr_from_js(gc, *p++);
  this->friend = person_from_js(gc, *p++);
  this->big = js_rb(*p++);
  this->historic2 = map_from_js(gc, *p++, (FFROM)darr_from_js);
  this->k2 = k2_ext;
  this->test = test_ext;
  return gc_clean(gcl, this);
}

/*--*/

some text



