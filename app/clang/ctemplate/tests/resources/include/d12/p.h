data

/*--*/

///
///   Arguments:
///     tmp: time_t
///     group: List-Iarr
///     cash: float
///     historic: Map-Darr
///     wait: long
typedef struct p_D12P D12P;

///
time_t d12P_tmp (D12P *this);

///
List *d12P_group (D12P *this);

///
float d12P_cash (D12P *this);

///
void d12P_set_cash (D12P *this, float value);

///
Map *d12P_historic (D12P *this);

///
long d12P_wait (D12P *this);

///
D12P *d12P_from_js (Gc *gc, Js *js);

///
///   Arguments:
///     tmp: time_t
///     group: List-Iarr
///     cash: float
///     historic: Map-Darr
///     wait: long
typedef struct p_D12P2 D12P2;

///
D12P2 *d12P2_new (
  Gc *gc,
  time_t tmp,
  List *group,
  float cash,
  Map *historic,
  long wait
);

///
time_t d12P2_tmp (D12P2 *this);

/// Some people
List *d12P2_group (D12P2 *this);

///
float d12P2_cash (D12P2 *this);

///
Map *d12P2_historic (D12P2 *this);

///
long d12P2_wait (D12P2 *this);

///
Js *d12P2_to_js (Gc *gc, D12P2 *this);

///
D12P2 *d12P2_from_js (Gc *gc, Js *js);

/*--*/

data
