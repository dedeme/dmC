// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/**
'ctpl' documentation.<p>

<!-   ________ -><hr>
<u><i>Index</i></u>:
<!-   TTTTTTTT ->

<ul>
<li><a href="#template">Template</a></li>
<li><a href="#definition">Definition</a></li>
<li><a href="#name">StructName</a></li>
<li><a href="#argument">ConstructorArgument</a></li>
<li><a href="#vars">Variables</a></li>
<li><a href="#funs">Functions</a></li>
<li><a href="#examples">Examples</a></li>
</ul>

<!-   ________ -><hr><span id = "template">
<u><i>Template</i></u>:
<!-   TTTTTTTT ->

<table><tr><td>
  (1) \n &sol;*. \n <a href="#definition">Definition</a>* \n *&sol; \n;
      ...
  (2) \n &sol;*--*&sol; \n &sol;*--*&sol; \n
</td></tr></table>
Example:
<table><tr><td>
  &sol;*.
  Definition 1
  Definition 2
  *&sol;
  ... Code ...
  &sol;*--*&sol;
  &sol;*--*&sol;
</td></tr></table>

Every template has two parts.<p>
The first part is the definition (1). It indicates how deploying code.<p>
The second part is the area where deploing code (2).<p>
<i>NOTE: Only can be one definition per file.</i>.<p>


<!-   __________ -><hr><span id = "definition">
<u><i>Definition</i></u>:
<!-   TTTTTTTTTT ->

<table><tr><td>
  <a href="#name">StuctureName</a> \n <a href="#argument">ConstructorArgument</a>* \n
  [--- \n <a href="#vars">Variables</a>* \n [--- \n <a href="#funs">Functions</a>*]? ]? \n === \n
</td></tr></table>
Example:
<table><tr><td>
  Name
    a1
    a2
    ---
    v1
    v2
    ---
    f1
    f2
  ===
</td></tr></table>
<ul>
<li> Documentation commentaries are allowed. Every commentary uses a complete
line and starts with '#'.</li>
<li> Blank lines are allowed.</li>
<li> The last "===" can be ommited.</li>
</ul>
Example:
<table><tr><td>
  Name
    a1
    #A commentary
    a2
    ---
    # A commentary of

    # two lines

    v1

    v2
</td></tr></table>




<!-   __________ -><hr><span id = "name">
<u><i>StructName</i></u>:
<!-   TTTTTTTTTT ->

<table><tr><td>
  [-] <i>name</i> [: [<tt>serial</tt> | <tt>to</tt> | <tt>from</tt>]]
</td></tr></table>
Examples:
<table><tr><td>
  (1) Person : serial
</td></tr></table>
<table><tr><td>
  (2) - Person
</td></tr></table>
The optional symbol '-' means that the <i>struct</i> constructor will be
private.<p>
After the name it is posible to indicate if the <i>struct</i> will be
serialized:<p>
<dl>
<dt><i>serial</i></dt><dd>Functions *_to_js_new and *_from_js_new will be
created.</dd>
<dt><i>to</i></dt><dd>Only the function *_to_js_new will be created.</dd>
<dt><i>from</i></dt><dd>Only the function from_js_nes will be created.</dd>
</dl>
<b>Example 1</b><br>
Creates a <i>struct</i> with a public constructor and serialializable.<p>
<b>Example 2</b><br>
Creates a <i>struct</i> with a private constructor without serializations.

<!-   ____________________ -><hr><span id = "argument">
<u><i>ConstructorArguments</i></u>:
<!-   TTTTTTTTTTTTTTTTTTTT ->

<table><tr><td>
  [- | @] <i>name</i> : <i>type</i> [: <a href="#free">Free</a>? [: <a href="#serial">Serial</a>]? ]?
</td></tr></table>
Examples:
<table><tr><td>
  dni : char * : _s : _s
</td></tr></table>
<table><tr><td>
  dni : char *
  @ sel : int :: _b
  - assets : Port *
  pets : Arr * : pet : pet
</td></tr></table>
An (argument / variable/ function) will be exposed as a 'getter' property.
The optional symbols '-' and '@' mean:
<dl>
<dt>-</dt><dd>Argument is private (neither getter nor setter).</dd>
<dt>@</dt><dd>Argument is exposed as a 'getter/setter'</dd>
</dl>
Part 'Serial' only is set when <a href="name">StructName</a> was defined with
'serial', 'to' or 'from'.<p>
Parts 'Free' and 'Serial' are optional when they are standard types.<p>
<span id="free">
<b>Free</b><br>
Standards:
<table><tr><td>
  int, double, time_t, size_t : _i
  enum xxx : _i
  char * : _s
  Arr * ; _a
  Map * : _m
  Xxx * : xxx (_free will be added. e.g. person_free)
</td></tr></table>
Others:
<table><tr><td>
  If argument is not freeable : _i
  Otherwise: !<i>function</i> (e.g. !kill_reg)
</td></tr></table>
Final result:
<table><tr><td>
  _i | _s | _a | _m | !xxx | xxx
</td></tr></table>
<span id="serial">
<b>Serial</b><br>
Standards:
<table><tr><td>
  int : _i
  enum xxx : _i
  char * : _s
  Xxx * : xxx (_to_js_new and _from_js_new will be added)
          e.g. person_to_js_new, person_from_js_new
</td></tr></table>
Semi-standards:
<table><tr><td>
  int (boolean) : _b
  double : _dN // N from 0 to 9 (e.g. _d2 -> scale 2, d4 -> scale 4, ...)
  Arr * : serialization code of element type (e.g. _s)
          Allowed types are: _i, _b, _s and xxx.
  Map * : serialization code of element type (e.g. person)
          Allowed types are: _i, _b, _s and xxx.
</td></tr></table>
Others:
<table><tr><td>
  If it is not possible match any of previous options, it is need to define
    <tt>static xxx_to_js_new</tt> and <tt>static xxx_from_js_new</tt> and use
    the standard 'Xxx' option.
</td></tr></table>
Final result:
<table><tr><td>
  "" | _i | _s | _b | _dN | _a _s | _a xxx | _m _s | _m xxx | xxx
</td></tr></table>

<!-   _________ -><hr><span id = "vars">
<u><i>Variables</i></u>:
<!-   TTTTTTTTT ->

<table><tr><td>
  [-] <i>name</i> : type : <a href="#val">Value</a> [: <a href="#free">Free</a>?]
</td></tr></table>
Examples:
<table><tr><td>
  age : int : 4
  pers_null : Person * : NULL
  - children : Arr * : arr_new(free)
  assets : Port * : NULL : kill_port
</td></tr></table>

A variable will be exposed as a 'getter' property, but [-] will make it
private.

<span id="val">
<b>Value</b><br>
Mandatory initialization of (variable / function).

<!-   _________ -><hr><span id = "funs">
<u><i>Functions</i></u>:
<!-   TTTTTTTTT ->

<table><tr><td>
  [-] <i>definition</i> : <a href="#val">Value</a>
</td></tr></table>
Examples:
<table><tr><td>
  int (*cmp_null)(void *, void *) : NULL
  - void (*proc)(char *) : proc
</td></tr></table>

A function will be exposed as a 'getter' property, but [-] will make it
private.

<!-   ________ -><hr><span id = "examples">
<u><i>Examples</i></u>:
<!-   TTTTTTTT ->

<table><tr><td>
  &sol;*.
  Fret
    error : char *
    value : double
  *&sol;
</td></tr></table>
<table><tr><td>
  &sol;*.
  Person : serial
    name : char *
    @age : int
    -children : Arr * :: person
    ---
    sel : int : 1
    - boss : Person * : NULL
    assets : Asset : asset_new() : kill_asset
    ---
    char *(mk_msg_new)(char *) : p_msg_new
  ===
  - Parent
    @ child : Person *
  *&sol;
</td></tr></table>

*/

#ifndef _DOC__H
  #define _DOC__H
#endif
