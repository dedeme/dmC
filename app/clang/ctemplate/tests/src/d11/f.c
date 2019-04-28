Some text

/* .
D11F: from
  ---
  ---
  void (*k)(Sell *this, char *s): ex_k
*/

some text

/*--*/

struct f_D11F{
  void (*k)(Sell *this, char *s);
};

D11F *d11F_new() {
  D11F *this = MALLOC(D11F);
  this->k = ex_k;
  return this;
}

void (*d11F_k(D11F *this))(Sell *this, char *s) {
  return this->k;
}

D11F *d11F_from_js(Js *js) {
  // Arr[Js]
  Arr *a = js_ra(js);
  Js **p = (Js **)arr_start(a);
  D11F *this = MALLOC(D11F);
  this->k = ex_k;
  return this;
}

/*--*/

some text



