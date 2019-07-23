Some text

/* .
D12F: to
  @c: char
  ---
  -i: int: 3
  ---
  -void (*k)(D12F *this): k_ext
===
D12F2: serial
  ---
  ---
  int (*k2)(D12F2 *this): k2_ext
  @ Rs *(*test)(D12F2 *this, int money): test_ext
*/

some text

/*--*/

struct f_D12F {
  char c;
  int i;
  void (*k)(D12F *this);
};

D12F *d12F_new (Gc *gc, char c) {
  D12F *this = gc_add(gc, malloc(sizeof(D12F)));
  this->c = c;
  this->i = 3;
  this->k = k_ext;
  return this;
}

char d12F_c (D12F *this) {
  return this->c;
}

void d12F_set_c (D12F *this, char value) {
  this->c = value;
}

Js *d12F_to_js (Gc *gc, D12F *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  arr_push(js, js_wi(gcl, (int)this->c));
  arr_push(js, js_wi(gcl, (int)this->i));
  return gc_clean(gcl, js_wa(gc, js));
}

struct f_D12F2 {
  int (*k2)(D12F2 *this);
  Rs *(*test)(D12F2 *this, int money);
};

D12F2 *d12F2_new (Gc *gc) {
  D12F2 *this = gc_add(gc, malloc(sizeof(D12F2)));
  this->k2 = k2_ext;
  this->test = test_ext;
  return this;
}

int (*d12F2_k2 (D12F2 *this))(D12F2 *this) {
  return this->k2;
}

Rs *(*d12F2_test (D12F2 *this))(D12F2 *this, int money) {
  return this->test;
}

void d12F2_set_test (D12F2 *this, Rs *(*value)(D12F2 *this, int money)) {
  this->test = value;
}

Js *d12F2_to_js (Gc *gc, D12F2 *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *js = arr_new(gcl);
  return gc_clean(gcl, js_wa(gc, js));
}

D12F2 *d12F2_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  D12F2 *this = gc_add(gc, malloc(sizeof(D12F2)));
  this->k2 = k2_ext;
  this->test = test_ext;
  return gc_clean(gcl, this);
}

/*--*/

some text



