data

/*--*/

///
///   Arguments:
///     c: char
///   Variables:
///     i: int
///   Functions:
///     i: int
typedef struct f_D12F D12F;

///
D12F *d12F_new (Gc *gc, char c);

///
char d12F_c (D12F *this);

///
void d12F_set_c (D12F *this, char value);

///
Js *d12F_to_js (Gc *gc, D12F *this);

///
///   Functions:
typedef struct f_D12F2 D12F2;

///
D12F2 *d12F2_new (Gc *gc);

///
int (*d12F2_k2 (D12F2 *this))(D12F2 *this);

///
Rs *(*d12F2_test (D12F2 *this))(D12F2 *this, int money);

///
void d12F2_set_test (D12F2 *this, Rs *(*value)(D12F2 *this, int money));

///
Js *d12F2_to_js (Gc *gc, D12F2 *this);

///
D12F2 *d12F2_from_js (Gc *gc, Js *js);

/*--*/

data
