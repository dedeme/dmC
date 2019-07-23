Some text

/* .
D11F: from
  ---
  ---
  void (*k)(Sell *this, char *s): ex_k
*/

some text

/*--*/

struct f_D11F {
  void (*k)(Sell *this, char *s);
};

D11F *d11F_new (Gc *gc) {
  D11F *this = gc_add(gc, malloc(sizeof(D11F)));
  this->k = ex_k;
  return this;
}

void (*d11F_k (D11F *this))(Sell *this, char *s) {
  return this->k;
}

D11F *d11F_from_js (Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  D11F *this = gc_add(gc, malloc(sizeof(D11F)));
  this->k = ex_k;
  return gc_clean(gcl, this);
}

/*--*/

some text



