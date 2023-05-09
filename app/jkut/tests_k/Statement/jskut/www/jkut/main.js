import * as iter from './_js/iter.js';import * as str from './_js/str.js';import * as bytes from './_js/bytes.js';import * as cryp from './_js/cryp.js';import * as dic from './_js/dic.js';import * as timer from './_js/timer.js';import * as js from './_js/js.js';import * as sys from './_js/sys.js';import * as math from './_js/math.js';import * as domo from './_js/domo.js';import * as ui from './_js/ui.js';import * as arr from './_js/arr.js';import * as time from './_js/time.js';import * as b64 from './_js/b64.js';
const A = sys.$checkNull([0]);

try{ A[0] =sys.$checkExists(A[0], sys.$checkNull(1));}
catch (e){ A[0] =sys.$checkExists(A[0], sys.$checkNull(2));}
sys.test(A[0], 1);

A[0] =sys.$checkExists(A[0], sys.$checkNull(0));
try{ {
  A[0] =sys.$checkExists(A[0], sys.$checkNull(1));
  throw new Error( "here");
}} catch (e){ {
  A[0] =sys.$checkExists(A[0], sys.$checkNull(2));
}}
sys.test(A[0], 2);















A[0] =sys.$checkExists(A[0], sys.$checkNull(0));
try{ A[0] =sys.$checkExists(A[0], sys.$checkNull(1));}
catch (e){ A[0] =sys.$checkExists(A[0], sys.$checkNull(2));}
finally{ A[0] =sys.$checkExists(A[0], sys.$checkNull(3));}
sys.test(A[0], 3);

A[0] =sys.$checkExists(A[0], sys.$checkNull(0));
try{ {
  A[0] =sys.$checkExists(A[0], sys.$checkNull(1));
  throw new Error( "here");
}} catch (e){ {
  A[0] =sys.$checkExists(A[0], sys.$checkNull(2));
}} finally{ A[0] =sys.$checkExists(A[0], sys.$checkNull(3));}
sys.test(A[0], 3);















function fn1  () {
  try{ return 1;}
  catch (e){ return 2;}
};
sys.test(fn1(), 1);

function fn2  () {
  try{ throw new Error( "here");}
  catch (e){ return 2;}
};
sys.test(fn2(), 2);









function fn4  () {
  try{ return 1;}
  catch (e){ return 2;}
  finally{ return 3;}
};
sys.test(fn4(), 3);

function fn5  () {
  try{ throw new Error( ("here"));}
  catch (e){ return 2;}
  finally{ return 4;}
};
sys.test(fn5(), 4);

const fnV = sys.$checkNull([0]);
function fn6  () {
  try{ return 1;}
  catch (e){ return 2;}
  finally{ fnV[0] =sys.$checkExists(fnV[0], sys.$checkNull(5));}
};
sys.test(fn6(), 1);
sys.test(fnV[0], 5);

fnV[0] =sys.$checkExists(fnV[0], sys.$checkNull(0));
function fn7  () {
  try{ throw new Error( ("here"));}
  catch (e){ return 2;}
  finally{ fnV[0] =sys.$checkExists(fnV[0], sys.$checkNull(5));}
};
sys.test(fn7(), 2);
sys.test(fnV[0], 5);

const Sum = sys.$checkNull([0]);
const C1 = sys.$checkNull([0]);
while (true) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] >= 5)) break;
  continue;
}
sys.test(Sum[0], 10);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (true) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] < 5)) continue;
  break;
}
sys.test(Sum[0], 10);

function fnw1  ()  {
  const C = sys.$checkNull([0]);
  while (true) {
    if (sys.asBool(sys.$eq(C[0] , 4))) return C[0];
    C[0] +=sys.$checkExists(C[0], sys.$checkNull(1));
  }
  return  -4;
};
sys.test(fnw1(), 4);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool(C1[0] < 5)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
}
sys.test(Sum[0], 10);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool(C1[0] < 25)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
}
sys.test(Sum[0], 300);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool(C1[0] < 25)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] >= 5)) break;
}
sys.test(Sum[0], 10);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool([1])) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] < 5)) continue;
  break;
}
sys.test(Sum[0], 10);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool([])) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] < 5)) continue;
  break;
}
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool("a")) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] < 5)) continue;
  break;
}
sys.test(Sum[0], 10);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
C1[0] =sys.$checkExists(C1[0], sys.$checkNull(0));
while (sys.asBool("")) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(C1[0]));
  C1[0] +=sys.$checkExists(C1[0], sys.$checkNull(1));
  if (sys.asBool(C1[0] < 5)) continue;
  break;
}
sys.test(Sum[0], 0);

function fnw2  ()  {
  const C = sys.$checkNull([0]);
  while (sys.asBool(C[0] < 4)) C[0] +=sys.$checkExists(C[0], sys.$checkNull(1));
  return C[0];
};
sys.test(fnw1(), 4);

function fnw3  ()  {
  const C = sys.$checkNull([0]);
  while (sys.asBool("1")) {
    if (sys.asBool(sys.$eq(C[0] , 4))) return C[0];
    C[0] +=sys.$checkExists(C[0], sys.$checkNull(1));
  }
  return  -4;
};
sys.test(fnw1(), 4);

const C = sys.$checkNull([23]);
if (sys.asBool([])) C[0] =sys.$checkExists(C[0], sys.$checkNull(0));
else C[0] =sys.$checkExists(C[0], sys.$checkNull(1));
sys.test(C[0], 1);

C[0] =sys.$checkExists(C[0], sys.$checkNull(23));
if (sys.asBool([1, 4])) C[0] =sys.$checkExists(C[0], sys.$checkNull(1));
else C[0] =sys.$checkExists(C[0], sys.$checkNull(0));
sys.test(C[0], 1);

C[0] =sys.$checkExists(C[0], sys.$checkNull(23));
if (sys.asBool("")) C[0] =sys.$checkExists(C[0], sys.$checkNull(0));
else C[0] =sys.$checkExists(C[0], sys.$checkNull(1));
sys.test(C[0], 1);

C[0] =sys.$checkExists(C[0], sys.$checkNull(23));
if (sys.asBool("")) throw new Error( ("fail"));
else C[0] =sys.$checkExists(C[0], sys.$checkNull(1));
sys.test(C[0], 1);

C[0] =sys.$checkExists(C[0], sys.$checkNull(23));
if (sys.asBool("  ")) {
  C[0] =sys.$checkExists(C[0], sys.$checkNull(1));
} else {
  throw new Error( ("fail"));
}
sys.test(C[0], 1);

function fni1  (a, b)  { if (sys.asBool(a > b)) return "up"; else return "down";};
sys.test(fni1(3, 2), "up");
sys.test(fni1("a", "z"), "down");

function fni2  (a, b)  {
  if (sys.asBool(a > b)) {
    return "up";
  } else if (sys.asBool(sys.$eq(a , b))) {
    return "eq";
  } else {
    return "down";
  }
};
sys.test(fni2(3.5, 2.03), "up");
sys.test(fni2("a", "z"), "down");
sys.test(fni2(1, 1), "eq");

const A0 = sys.$checkNull([]);
const A1 = sys.$checkNull([1]);
const A3 = sys.$checkNull([1, 2, 3]);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( A0)) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( A1)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
}
sys.test(Sum[0], 1);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( A3)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
}
sys.test(Sum[0], 6);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( A3)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , 2))) break;
}
sys.test(Sum[0], 3);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( A3)) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(e < 2)) continue;
  break;
}
sys.test(Sum[0], 3);

function fnf1  () {
  const Sum = sys.$checkNull([0]);
  for (let e  of sys.$forObject( A3)) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e < 2)) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf1(), 3);

function fnf3  () {
  const Sum = sys.$checkNull([0]);
  for (let e  of sys.$forObject( iter.$range(1 , 4))) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e < 2)) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf3(), 3);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i  of sys.$forObject( iter.$range(5 , 4))) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i  of sys.$forObject( iter.$range(4 , 4))) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i  of sys.$forObject( iter.$range_step(1 , 3 , 1))) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 6);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( iter.$range_step(1 , 3 , 1))) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , 2))) break;
}
sys.test(Sum[0], 3);

function fnf4  () {
  const Sum = sys.$checkNull([0]);
  for (let e  of sys.$forObject( iter.$range_step(1 , 3 , 1))) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e < 2)) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf4(), 3);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i  of sys.$forObject( iter.$range_step(4 , 20 ,  -2))) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i  of sys.$forObject( iter.$range_step(4 , 1 ,  -2))) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 6);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e  of sys.$forObject( iter.$range_step(4 , 1 ,  -2))) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , 2))) break;
}
sys.test(Sum[0], 6);

function fnf5  () {
  const Sum = sys.$checkNull([0]);
  for (let e  of sys.$forObject( iter.$range_step(4 , 1 ,  -2))) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e > 2)) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf5(), 6);

const Str = sys.$checkNull([""]);
for (let e  of sys.$forObject( "")) Str[0] +=sys.$checkExists(Str[0], sys.$checkNull(e));
sys.test(Str[0], "");

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
for (let e  of sys.$forObject( "abc")) Str[0] +=sys.$checkExists(Str[0], sys.$checkNull(e));
sys.test(Str[0], "abc");

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
for (let e  of sys.$forObject( "abc")) {
  Str[0] +=sys.$checkExists(Str[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , "b"))) break;
}
sys.test(Str[0], "ab");

function fnf6  () {
  Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
  for (let e  of sys.$forObject( "abc")) {
    Str[0] +=sys.$checkExists(Str[0], sys.$checkNull(e));
    if (sys.asBool(e < "b")) continue;
    return Str[0];
  }
  return "";
};
sys.test(fnf6(), "ab");

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(""));
for (let i  of sys.$forObject( {})) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], "");

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(""));
for (let i  of sys.$forObject( {"a": 1, "b": 2, "c": 3})) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], "abc");

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(""));
for (let e  of sys.$forObject( {"a": 1, "b": 2, "c": 3})) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , "b"))) break;
}
sys.test(Sum[0], "ab");

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
const Dic = sys.$checkNull({"a": 1, "b": 2, "c": 3});
for (let k  of sys.$forObject( Dic)) {
  const e = sys.$checkNull(Dic[k]);
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  Str[0] +=sys.$checkExists(Str[0], sys.$checkNull(k));
  if (sys.asBool(e < 2)) continue;
  break;
}
sys.test(Sum[0], 3);
sys.test(Str[0], "ab");

function fnf7  () {
  const Sum = sys.$checkNull([""]);
  for (let e  of sys.$forObject( {"a": 1, "b": 2, "c": 3})) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e < "b")) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf7(), "ab");

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i = 5 ;i <  4; ++i) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i = 1 ;i <  4; ++i) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 6);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let e = 1 ;e <  4; ++e) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , 2))) break;
}
sys.test(Sum[0], 3);

function fnf8  () {
  const Sum = sys.$checkNull([0]);
  for (let e = 1 ;e <  4; ++e) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e < 2)) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf8(), 3);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i = 5 ;i <  4; ++i) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
for (let i = 4 ;i <  4; ++i) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
{const _s=1;const _f=sys.$forCmp(_s);for (let i = 1 ;_f(i,  3) ;i += _s ) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));}
sys.test(Sum[0], 6);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
{const _s=1;const _f=sys.$forCmp(_s);for (let e = 1 ;_f(e,  3) ;e += _s ) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , 2))) break;
}}
sys.test(Sum[0], 3);

function fnf9  () {
  const Sum = sys.$checkNull([0]);
  for (let e  of sys.$forObject( iter.$range_step(1 , 3 , 1))) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e < 2)) continue;
    return Sum[0];
  }
  return 0;
};
sys.test(fnf9(), 3);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
{const _s=-2;const _f=sys.$forCmp(_s);for (let i = 4 ;_f(i,  20) ;i += _s  ) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));}
sys.test(Sum[0], 0);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
{const _s=-2;const _f=sys.$forCmp(_s);for (let i = 4 ;_f(i,  1) ;i += _s  ) Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(i));}
sys.test(Sum[0], 6);

Sum[0] =sys.$checkExists(Sum[0], sys.$checkNull(0));
{const _s=-2;const _f=sys.$forCmp(_s);for (let e = 4 ;_f(e,  1) ;e += _s  ) {
  Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
  if (sys.asBool(sys.$eq(e , 2))) break;
}}
sys.test(Sum[0], 6);

function fnf10  () {
  const Sum = sys.$checkNull([0]);
  {const _s=-2;const _f=sys.$forCmp(_s);for (let e = 4 ;_f(e,  1) ;e += _s  ) {
    Sum[0] +=sys.$checkExists(Sum[0], sys.$checkNull(e));
    if (sys.asBool(e > 2)) continue;
    return Sum[0];
  }}
  return 0;
};
sys.test(fnf10(), 6);

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
switch (3 + 2) {
  case 3 :{ Str[0] =sys.$checkExists(Str[0], sys.$checkNull("3"));break;}
}
sys.test(Str[0], "");

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
switch (3 + 2) {
  case 3 :{ {
    Str[0] =sys.$checkExists(Str[0], sys.$checkNull("3"));
  }break;}
  default :{ {
    Str[0] =sys.$checkExists(Str[0], sys.$checkNull("df"));
  }}
}
sys.test(Str[0], "df");

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
switch (3 + 2) {
  case 3       :{ throw new Error( ("error"));break;}
  case 30 / 6  :{ Str[0] =sys.$checkExists(Str[0], sys.$checkNull("5"));break;}
  default :{ throw new Error( ("fail"));}
}
sys.test(Str[0], "5");

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
switch (3 + 2) {
  case 3           :{ throw new Error( ("error"));break;}
  case 20: case 30 / 6  :{ Str[0] =sys.$checkExists(Str[0], sys.$checkNull("5"));break;}
  default     :{ throw new Error( ("fail"));}
}
sys.test(Str[0], "5");

Str[0] =sys.$checkExists(Str[0], sys.$checkNull(""));
switch (3 + 2) {
  case 3           :{ throw new Error( ("error"));break;}
  case 30 / 6: case 20  :{ Str[0] =sys.$checkExists(Str[0], sys.$checkNull("5"));break;}
  default     :{ throw new Error( ("fail"));}
}
sys.test(Str[0], "5");

function fn  (n)  {  return n|0;;};
sys.test(fn(4), 4);

function fn22  (n)  {

  

  
let m = n * 2;
m += 1;

return m;



  ;
};
sys.test(fn22(4), 9);

function fn33  (n, m)  {
  return 33;
};
sys.test(fn33(4, 6), 33);

console.log( "End expression tests ok at line 594");
