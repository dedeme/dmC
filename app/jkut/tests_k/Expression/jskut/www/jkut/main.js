import * as iter from './_js/iter.js';import * as str from './_js/str.js';import * as bytes from './_js/bytes.js';import * as cryp from './_js/cryp.js';import * as dic from './_js/dic.js';import * as timer from './_js/timer.js';import * as js from './_js/js.js';import * as sys from './_js/sys.js';import * as math from './_js/math.js';import * as domo from './_js/domo.js';import * as ui from './_js/ui.js';import * as arr from './_js/arr.js';import * as time from './_js/time.js';import * as b64 from './_js/b64.js';
const a =
    sys.$checkNull(12)
;
sys.test(a, 12);
sys.test(12, a);

const b1 = sys.$checkNull(sys.asBool(true) && sys.asBool(true));
sys.assert(b1);
const b2 = sys.$checkNull(sys.asBool(true)
  && sys.asBool(false));
sys.assert(!sys.asBool(b2));
const b3 = sys.$checkNull(sys.asBool(false) &&
  sys.asBool(true));
sys.assert(!sys.asBool(b3));
const b4 = sys.$checkNull(sys.asBool(false)
  &&
  sys.asBool(false));
sys.assert(!sys.asBool(b4));
const b5 = sys.$checkNull(sys.asBool(true) || sys.asBool(true));
sys.assert(b5);
const b6 = sys.$checkNull(sys.asBool(true) || sys.asBool(false));
sys.assert(b6);
const b7 = sys.$checkNull(sys.asBool(false) || sys.asBool(true));
sys.assert(b7);
const b8 = sys.$checkNull(sys.asBool(false) || sys.asBool(false));
sys.assert(!sys.asBool(b8));

const sum1 = sys.$checkNull(2 + 1);
sys.test(sum1, 3);
const sum2 = sys.$checkNull(2.5
  +
    1.4)
    ;
sys.test(sum2, 3.9);
const sum3 = sys.$checkNull("a" + "b");
sys.test(sum3, "ab");
const sum4 = sys.$checkNull("a" + "");
sys.test(sum4, "a");

const sub1 = sys.$checkNull(2 - 1);
sys.test(sub1, 1);
const sub2 = sys.$checkNull(2.5
  -
    1.4)
    ;
sys.test(sub2, 1.1);

const mul1 = sys.$checkNull(12 * 5);
sys.test(mul1, 60);
const mul2 = sys.$checkNull(22.52
  *
    10)
    ;
sys.test(mul2, 225.2);

const div1 = sys.$checkNull(12 / 5);
sys.test(div1, 2.4);
const div2 = sys.$checkNull(22.52
  /
    10)
    ;
sys.test(div2, 2.252);

const mod1 = sys.$checkNull(14 % 5);
sys.test(mod1, 4);
const mod2 = sys.$checkNull(22.52
  %
    10)
    ;
sys.test(mod2, 2.52);

sys.test(sys.toStr(true), "true");
sys.test(sys.toStr(false), "false");
sys.test(sys.toStr(!sys.asBool(false)), "true");
sys.test(sys.toStr(
  !sys.asBool(
  true)
), "false");

sys.test(sys.toStr(0), "0");
sys.test(sys.toStr(1234), "1234");
sys.test(sys.toStr(564), "564");

sys.test(sys.toStr(1.2300), "1.23");
sys.test(sys.toStr(1586.235e2), "158623.5");
sys.test(sys.toStr( - 1.2300), "-1.23");
sys.test(sys.toStr( -1586.235e2), "-158623.5");
sys.test(sys.toStr(0.0), "0");
sys.test(sys.toStr(0.012), "0.012");

sys.test(sys.toStr(""), "");
sys.test(sys.toStr("abc"), "abc");

sys.test(sys.toStr([]), "[]");
sys.test(sys.toStr(["a" ]), "[\"a\"]");
sys.test(sys.toStr([ 1 ,
  2, 3 ]), "[1,2,3]");

sys.test(sys.toStr({}), "{}");
sys.test(sys.toStr({"a": 1}), '{"a":1}');
sys.test(sys.toStr({"a": 1, "b": 2, "c": 3}), '{"a":1,"b":2,"c":3}');
sys.test({"a": 1, "b": 2, "c": 3}, {"a": 1, "b": 2, "c": 3});

sys.test(sys.toStr((()=> {})), "()=> {}");
sys.test(sys.toStr((()=> { sys.println("");})), '()=> { sys.println("");}');
sys.test(sys.toStr(((a, b) => {
    const c = sys.$checkNull(a + b);
    return c;
  })), '(a, b) => {\n    const c = sys.$checkNull(a + b);\n    return c;\n  }');

sys.test({"a": 1, "b": 2}.a, 1);


sys.test("ab"[1], "b");
sys.test("aros"[3],"s");
sys.test("años"[2], "o");

sys.test({"a": 1, "b": 2}["a"], 1);


sys.test(sys.$slice("abc",null,null), "abc");
sys.test(sys.$slice("abc",null,2), "ab");
sys.test(sys.$slice("abc",2,null), "c");
sys.test(sys.$slice("abc",1,2), "b");
sys.test(sys.$slice("abc",null, -1), "ab");
sys.test(sys.$slice("abc", -1,null), "c");

sys.test(sys.$slice([1, 2, 3],null,null), [1, 2, 3]);
sys.test(sys.$slice([1, 2, 3],null,2), [1, 2]);
sys.test(sys.$slice([1, 2, 3],2,null), [3]);
sys.test(sys.$slice([1, 2, 3],1,2), [2]);
sys.test(sys.$slice([1, 2, 3],null, -1), [1, 2]);
sys.test(sys.$slice([1, 2, 3], -1,null), [3]);

sys.test(
   
    sys.$eq([1, 2, 3][1],1) ? "one":
    sys.$eq([1, 2, 3][1],2) ? "two":
    sys.$eq([1, 2, 3][1],3) ? "three":
     "none"
  , "two"
);
sys.test(
   
    sys.$eq([1, 2, 3][1],1) ? "one":
    sys.$eq([1, 2, 3][1],3) ? "three":
     "none"
  , "none"
);
sys.test(
   
    sys.$eq([1, 2, 3][1],1) ? "one":
    sys.$eq([1, 2, 3][1],2)|| sys.$eq([1, 2, 3][1],15) ? "two":
    sys.$eq([1, 2, 3][1],3) ? "three":
     "None"
  , "two"
);
sys.test(
   
    sys.$eq([1, 2, 3][1],1) ? "one":
    sys.$eq([1, 2, 3][1],15)|| sys.$eq([1, 2, 3][1],2)|| sys.$eq([1, 2, 3][1],33) ? "two":
    sys.$eq([1, 2, 3][1],3) ? "three":
     "None"
  , "two"
);

sys.test(!sys.asBool(true), false);
sys.test(!sys.asBool(false), true);
sys.test(!sys.asBool("abc"), false);
sys.test(!sys.asBool(""), true);
sys.test(!sys.asBool([1, 2]), false);
sys.test(!sys.asBool([]), true);

sys.test( -3,  -3);
sys.test( - -3, 3);
sys.test( -0.0, 0.0);
sys.test( - -3.45, 3.45);

sys.test(2 + 3, 5);
sys.test(2.1 + 3.6, 5.7);
sys.test("a" + "bc", "abc");
sys.test("" + "bc", "bc");
sys.test("bc" + "", "bc");

sys.test(2 -  -2, 4);
sys.test(2 *  -2,  -4);
sys.test(3 /  -2,  -1.5);
sys.test(2.0 -  -2.0, 4.0);
sys.test(2.0 *  -2.0,  -4.0);
sys.test(3.0 /  -2.0,  -1.5);

sys.test(15 % 7, 1);

sys.test(sys.asBool(true) && sys.asBool(false), false);
sys.test(sys.asBool(true) || sys.asBool(4/0), true);
sys.test(sys.asBool("abc") && sys.asBool(""), false);
sys.test(sys.asBool("abc") || sys.asBool(""), true);
sys.test(sys.asBool([]) && sys.asBool(4/0), false);
sys.test(sys.asBool([]) || sys.asBool([1, 2]), true);
sys.test(sys.asBool(sys.$eq(3 , 0)) && sys.asBool(sys.$eq(3 , "a")), false);
sys.test(sys.asBool(sys.$neq(3 , 0)) || sys.asBool(sys.$eq(3 , "a")), true);

sys.test(sys.asBool(sys.$eq("ab"[0] , "a")) ? 1 : 2, 1);
sys.test(sys.asBool("") ? 1 : 2, 2);
sys.test(sys.asBool([0]) ? 1 : 2, 1);

sys.test(4 - 2 - 2, 0);
sys.test(4 - (2 - 2), 4);
sys.test(4 + 3 * 2, 10);
sys.test((4 + 3) * 2, 14);
sys.test((4 +  -3) * 2, 2);
sys.test(3 + 2 > 3 + 1, true);
sys.test(sys.asBool(3 + 2 < 3 + 1) ? sys.$eq(2 , 4 + 12) : 16, 16);
sys.test(sys.asBool(3 + 2 < 3 + 1) ? 4/0 : 16, 16);
sys.test(sys.asBool(3 + 2 > 3 + 1) ? sys.$eq(2 , 4 + 12) : 16,  false);
sys.test(sys.asBool(3 + 2 > 3 + 1) ? sys.$eq(2 , 4 + 12) : 4/0, false);

sys.assert(sys.$eq(true , true));
sys.assert(sys.$neq(true , false));

sys.assert(sys.$eq(34 , 34));
sys.assert(sys.$neq(34 , 35));
sys.assert(sys.$neq(34 ,  -34));
sys.assert(sys.$eq(0 ,  -0));

sys.assert(sys.$eq(12.5 , 12.5));
sys.assert(sys.$neq(12.6 , 12.5));
sys.assert(sys.$neq(12.5 ,  -12.5));
sys.assert(sys.$eq(0.0 ,  -0.0));

sys.assert(5 > 4);
sys.assert(5 >= 4);
sys.assert(4 >= 2 + 2);
sys.assert( -4 < 4);
sys.assert( -4 <= 4);
sys.assert( -4 <=  -4);


sys.assert(sys.$eq("" , ""));
sys.assert(sys.$neq("" , "abc"));
sys.assert(sys.$eq("abc" , "abc"));
sys.assert(sys.$neq("abc" , "a"));
sys.assert(sys.$neq("abc" , "bbc"));

sys.assert("a" > "");
sys.assert("a" >= "");
sys.assert("a" >= "a");
sys.assert("ab" < "b");
sys.assert("ab" <= "b");
sys.assert("ab" <= "ab");

sys.assert(sys.$eq([] , []));
sys.assert(sys.$neq([] , [1, 2]));
sys.assert(sys.$eq([1,2] , [1, 2]));
sys.assert(sys.$neq([1, 2] , [1]));
sys.assert(sys.$neq([1, 2] , [1, 1]));

sys.assert(sys.$eq({} , {}));
sys.assert(sys.$neq({} , {"a": 1, "b": 2}));
sys.assert(sys.$eq({"a": 1, "b": 2} , {"a": 1, "b": 1 + 1}));
sys.assert(sys.$neq({"a": 1, "b": 2} , {"a": 1}));
sys.assert(sys.$neq({"a": 1, "b": 2} , {"a": 1, "b": 1}));


export const n1 = sys.$checkNull(23);
sys.test(n1 + 2, 25);
const A1 = sys.$checkNull([12]);
const A2 = sys.$checkNull([12.0]);
const A3 = sys.$checkNull([true]);

const M = sys.$checkNull({"a": 12, "b": 12.0, "c": true});
function fn1  ()  { return M.a;};
function fn2  (n)   {
  return M.a - 4 + n1;
};
function fn3  (m, n)  {
  const n2 = sys.$checkNull(n1 - 13 + m + n);
  return M.a - n2;
};
const n2 = sys.$checkNull(n1);
{
  { sys.test(n2, 23); }
  const n1 =  sys.$checkNull(-2);
  sys.test(n1,  -2);
  {
    sys.test(n1, fn1());
    const n2 = sys.$checkNull(11);
    sys.test(n2, 11);
  }

  function fn1  () { return  -2;}; 

  A1[0] +=sys.$checkExists(A1[0], sys.$checkNull(2));
  sys.test(A1[0], 14);
  A1[0] -=sys.$checkExists(A1[0],  sys.$checkNull(-2));
  sys.test(A1[0], 16);
  A1[0] *=sys.$checkExists(A1[0], sys.$checkNull(3));
  sys.test(A1[0], 48);
  A1[0] /=sys.$checkExists(A1[0], sys.$checkNull(2));
  sys.test(A1[0], 24);
  A1[0] %=sys.$checkExists(A1[0], sys.$checkNull(5));
  sys.test(A1[0], 4);

  A2[0] +=sys.$checkExists(A2[0], sys.$checkNull(2.0));
  sys.test(A2[0], 14.0);
  A2[0] -=sys.$checkExists(A2[0],  sys.$checkNull(-2.0));
  sys.test(A2[0], 16.0);
  A2[0] *=sys.$checkExists(A2[0], sys.$checkNull(3.0));
  sys.test(A2[0], 48.0);
  A2[0] /=sys.$checkExists(A2[0], sys.$checkNull(2.0));
  sys.test(A2[0], 24.0);

  A3[0] &&=sys.$checkExists(A3[0], sys.$checkNull(false));
  sys.assert(!sys.asBool(A3[0]));
  A3[0] ||=sys.$checkExists(A3[0], sys.$checkNull(true));
  sys.assert(A3[0]);

  M["a"] +=sys.$checkExists(M["a"], sys.$checkNull(2));
  sys.test(M.a, 14);
  M["a"] -=sys.$checkExists(M["a"],  sys.$checkNull(-2));
  sys.test(M.a, 16);
  M["a"] *=sys.$checkExists(M["a"], sys.$checkNull(3));
  sys.test(M.a, 48);
  M["a"] /=sys.$checkExists(M["a"], sys.$checkNull(2));
  sys.test(M.a, 24);
  M["a"] %=sys.$checkExists(M["a"], sys.$checkNull(5));
  sys.test(M.a, 4);

  M.b +=sys.$checkExists(M.b, sys.$checkNull(2.0));
  sys.test(M["b"], 14.0);
  M.b -=sys.$checkExists(M.b,  sys.$checkNull(-2.0));
  sys.test(M["b"], 16.0);
  M.b *=sys.$checkExists(M.b, sys.$checkNull(3.0));
  sys.test(M["b"], 48.0);
  M.b /=sys.$checkExists(M.b, sys.$checkNull(2.0));
  sys.test(M["b"], 24.0);

  M.c &&=sys.$checkExists(M.c, sys.$checkNull(false));
  sys.assert(!sys.asBool(M.c));
  M.c ||=sys.$checkExists(M.c, sys.$checkNull(true));
  sys.assert(M.c);

  {
    sys.test(fn1(),  -2);
  }
  sys.test(fn2(10), 23);
  sys.test(fn3(15,  -5),  -16);
}
sys.test(n1, 23);
sys.test(A1[0], 4);
sys.test(A2[0], 24.0);
sys.test(A3[0], true);
sys.test(M.a, 4);
sys.test(M.b, 24.0);
sys.test(M.c, true);

sys.test(fn1(), 4);
sys.test(fn2(10), 23);
sys.test(fn3(15,  -5),  -16);

fn1();

function fn4  () { sys.test(fn5(), "ok");};
fn4();

function fn5  () { return "ok";};

sys.test( Number('123'), 123);

const d = sys.$checkNull(new  Date('December 17, 1995 03:24:00'));
sys.test( Number(d), 819167040000);
sys.test(sys.$null( d.getTime()), [819167040000]);
sys.test(sys.$null( d["timex"]), []);

sys.test( "a\"b\"c\n", "a\"b\"c\n");
sys.test(

"This is the thing"








, "This is the thing");



console.log( "End expression tests ok at line 392");
