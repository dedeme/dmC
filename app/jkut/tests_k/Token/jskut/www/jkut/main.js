import * as iter from './_js/iter.js';import * as str from './_js/str.js';import * as bytes from './_js/bytes.js';import * as cryp from './_js/cryp.js';import * as dic from './_js/dic.js';import * as timer from './_js/timer.js';import * as js from './_js/js.js';import * as sys from './_js/sys.js';import * as math from './_js/math.js';import * as domo from './_js/domo.js';import * as ui from './_js/ui.js';import * as arr from './_js/arr.js';import * as time from './_js/time.js';import * as b64 from './_js/b64.js';

sys .
test  ( true
 , true) ;



sys.test(1,
1);
sys.test(1.34, 1.34);

sys.test("ab\"ñ\"c\u2acd", "ab\"ñ\"c\u2acd");
sys.test("", "");

sys.test([1, 2, 3], [1 ,  2,   3  ]); 
sys.test([], [   ]);

sys.test( {a: "ax", b: "bc··"}, {b: "bc··", a: "ax"} );
sys.test({"a": 3, "b": 4}, {a: 3, b: 4});
const a = sys.$checkNull(3);
const b = sys.$checkNull(4);
sys.test({a:a, b:b}, {"a": 3, b : 4});




sys.test({}, {  }
  );
sys.test(
String.raw
`En algún
    punto ${"$"}{x, y}
se dice ${"`"}x, y${"`"}.
`,
  "En algún\n    punto ${x, y}\nse dice `x, y`.\n"

);

sys.test(1.e4, 10000);

sys.test((() => { return 16;})(), 16);
sys.test(((a) => { return (a);})(16), 16);
sys.test(((a, b) => { return a;})(16, 12), 16);

sys.test((() => {
  sys.test("", "");
  sys.assert("r");
  sys.assert(["a"]);
  return 16;
})(), 16);
sys.test(((a) => {
  sys.test(sys.asBool("") ? true : false, false);
  sys.test(sys.asBool([]) ? true : false, false);
  return (a);
})(16), 16);
sys.test(((a, b) => {
  return a;
})(16, 12), 16);

(() => { return 16;})();
((a) => { return (a);})(16);
((a, b) => { return a;})(16, 12);

(() => {
  sys.assert("r");
  sys.assert(["a"]);
  return 16;
})();
((a) => {
  sys.assert("r");
  sys.assert(["a"]);
  return (a);
})(16);
((a, b) => {
  sys.assert("r");
  sys.assert(["a"]);
  return a;
})(16, 12);

console.log( "End token tests ok at line 81");
