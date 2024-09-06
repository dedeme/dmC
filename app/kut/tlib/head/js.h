typedef struct tarr_Arrs Arrs;

static int js_is_null (char *json);
static int js_rb (char *json);
static int64_t js_ri (char *json);
static double js_rf (char *json);
static char *js_rs (char *json);
static Arrs *js_ra (char *json);
static Arrs *js_ro (char *json);
static char *js_wb(int value);
static char *js_wi(int64_t n);
static char *js_wf(double n);
static char *js_wf2(double n, int64_t scale);
static char *js_ws(char *s);
static char *js_wa(Arrs *a);
static char *js_wo(Arrs *m);
