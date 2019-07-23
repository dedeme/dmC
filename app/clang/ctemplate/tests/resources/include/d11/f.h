data

/*--*/

///
///   Functions:
typedef struct f_D11F D11F;

///
D11F *d11F_new (Gc *gc);

///
void (*d11F_k (D11F *this))(Sell *this, char *s);

///
D11F *d11F_from_js (Gc *gc, Js *js);

/*--*/

data
