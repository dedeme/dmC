Some text

/* .
- D12P: from


  tmp: time_t
  # Some people


  group: List - Iarr
  @cash: float
  historic: Map - Darr
  wait: long


  ===

 D12P2: serial


  tmp: time_t

  # Some people
  group: List - Iarr
  cash: float

  historic: Map - Darr
  wait: long
*/

some text

/*--*/

struct p_D12P {
  time_t tmp;
  List *group;
  float cash;
  Map *historic;
  long wait;
};

static D12P *_d12P_new (
  Gc *gc,
  time_t tmp,
  List *group,
  float cash,
  Map *historic,
  long wait
) {
  D12P *this = gc_add(gc, malloc(sizeof(D12P)));
  this->tmp = tmp;
  this->group = group;
  this->cash = cash;
  this->historic = historic;
  this->wait = wait;
  return this;
}

time_t d12P_tmp (D12P *this) {
  return this->tmp;
}

List *d12P_group (D12P *this) {
  return this->group;
}

float d12P_cash (D12P *this) {
  return this->cash;
}

void d12P_set_cash (D12P *this, float value) {
  this->cash = value;
}

Map *d12P_historic (D12P *this) {
  return this->historic;
}

long d12P_wait (D12P *this) {
  return this->wait;
}

D12P *d12P_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  D12P *this = gc_add(gc, malloc(sizeof(D12P)));
  this->tmp = (time_t)js_rl(*p++);
  this->group = list_from_js(gc, *p++, (FFROM)iarr_from_js);
  this->cash = (float)js_rd(*p++);
  this->historic = map_from_js(gc, *p++, (FFROM)darr_from_js);
  this->wait = (long)js_rl(*p++);
  return gc_clean(gcl, this);
}

struct p_D12P2 {
  time_t tmp;
  List *group;
  float cash;
  Map *historic;
  long wait;
};

D12P2 *d12P2_new (
  Gc *gc,
  time_t tmp,
  List *group,
  float cash,
  Map *historic,
  long wait
) {
  D12P2 *this = gc_add(gc, malloc(sizeof(D12P2)));
  this->tmp = tmp;
  this->group = group;
  this->cash = cash;
  this->historic = historic;
  this->wait = wait;
  return this;
}

time_t d12P2_tmp (D12P2 *this) {
  return this->tmp;
}

List *d12P2_group (D12P2 *this) {
  return this->group;
}

float d12P2_cash (D12P2 *this) {
  return this->cash;
}

Map *d12P2_historic (D12P2 *this) {
  return this->historic;
}

long d12P2_wait (D12P2 *this) {
  return this->wait;
}

Js *d12P2_to_js (Gc *gc, D12P2 *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  arr_push(js, js_wl(gcl, (long)this->tmp));
  arr_push(js, list_to_js(gcl, this->group, (FTO)iarr_to_js));
  arr_push(js, js_wd(gcl, (double)this->cash));
  arr_push(js, map_to_js(gcl, this->historic, (FTO)darr_to_js));
  arr_push(js, js_wl(gcl, (long)this->wait));
  return gc_clean(gcl, js_wa(gc, js));
}

D12P2 *d12P2_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  D12P2 *this = gc_add(gc, malloc(sizeof(D12P2)));
  this->tmp = (time_t)js_rl(*p++);
  this->group = list_from_js(gc, *p++, (FFROM)iarr_from_js);
  this->cash = (float)js_rd(*p++);
  this->historic = map_from_js(gc, *p++, (FFROM)darr_from_js);
  this->wait = (long)js_rl(*p++);
  return gc_clean(gcl, this);
}

/*--*/

some text



