// Copyright 03-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Market adds-on.
///
/// Binary:
///   kutmkt
///
/// Utility:
///   Utilities for market programs.
///
/// Functions for quotes:
///
///   \[f...] -> <vec>
///   mkVec
///   sys.c("mkVec", Qs); // -> Opaque object to use with this module.
///   ------
///   Converter. The new object is a deep copy of the array.
///
///   \<vec> -> [f...]
///   vecToArr
///   sys.c("vecToArr", vQs); // -> Original array.
///   ------
///   Converter. The new array is a deep copy of the <vec> object.
///
///   \s -> [[s, f, f, f, f, i, b]|s, b] (Result)
///   quoteFromStr
///   Q = sys.c("quoteFromStr", s);
///   if (Q[1]) {
///     A = Q!;
///     return quote.new(A[0], A[1], A[2], A[3], A[4], A[5], A[6]);
///   }
///   sys.throw(Q!);
///   ------
///   Converter. if 's' is not correct quote text, an Result.error is returned.
///
///   \s -> [[[s, f, f, f, f, i, b]...]|s, b] (Result)
///   qsFromStr
///   Q0s = sys.c("qsFromStr", s);
///   if (Q0s[1]) {
///     return arr.map(
///       Q0s!,
///       \A -> return quote.new(A[0], A[1], A[2], A[3], A[4], A[5], A[6]);
///     );
///   }
///   sys.throw(Q0s!);
///   ------
///   Converter. if 's' is not correct quotes text, an Result.error is returned.
///
///   \[s, i] -> f
///   qsVol
///   sys.c("qsVol", s, samples); // -> Volume of the first 'samples' quotes 's.
///   ------
///   Converter. if 's' is not correct quotes text, 0 is returned.
///
/// Functions for strategy:
///
///   \[d, <vec>, <vec>] -> ()
///   strategyRun
///   sys.c("strategyRun", [Env, Cls, Refs]);
///   ------
///   Running the market strategy.
///   See "libmkt/strategy" in "KtWeb:CMarket".
///
///   \{Closes=[<vec>...], Params=[f...]} -> <vec>
///   calcXxx
///   sys.c("calcXxx", [Env]); // -> References vector
///   ------
///   Calculate simulation results for a market model.
///   They are defined:
///     calcAppr
///     calcAppr2
///     calcEa
///     calcEa2
///     calcMa
///     calcMm
///     calcQfix
///     calcQmov

#ifndef FACKE

/// Fake function
int exmodule__default ();

#endif
