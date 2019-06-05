// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/**
'ctpl' documentation.<p>

<!-        _____ -><hr>
<u><big><i>Index</i></big></u>:
<!-        TTTTT ->

<ul>
<li><a href="#template">Template</a></li>
<li><a href="#definition">Definition</a></li>
<li><a href="#name">StructName</a></li>
<li><a href="#argument">ConstructorArguments</a></li>
<li><a href="#vars">Variables</a></li>
<li><a href="#funs">Functions</a></li>
<li><a href="#examples">Examples</a></li>
</ul>

<!-        ________ -><hr><span id = "template">
<u><big><i>Template</i></big></u>:
<!-        TTTTTTTT ->

<table><tr><td>
  (1) &sol;*.
      <a href="#definition">Definition</a>*
      *&sol;
      ...
  (2) &sol;*--*&sol;
      &sol;*--*&sol;
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
<i>NOTE: Only can be one definition per file.</i><p>

<!-        __________ -><hr><span id = "definition">
<u><big><i>Definition</i></big></u>:
<!-        TTTTTTTTTT ->

<table><tr><td>
  <a href="#name">StuctureName</a>
  <a href="#argument">ConstructorArguments</a>*
  [ ---
    <a href="#vars">Variables</a>*
    [ ---
      <a href="#funs">Functions</a>*
    ]
  ]
  ===
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

<!-        __________ -><hr><span id = "name">
<u><big><i>StructName</big></i></u>:
<!-        TTTTTTTTTT ->

<table><tr><td>
  [ - | = ] <i>name</i> [: [<tt>serial</tt> | <tt>SERIAL</tt> | <tt>to</tt> | <tt>from</tt>] | <tt>FROM</tt>]]
</td></tr></table>
Examples:
<table><tr><td>
  Person : to
</td></tr></table>
<table><tr><td>
  = Person
</td></tr></table>
The optional symbol '-' means that the <i>struct</i> constructor will be
private.<p>
The optional symbol '=' means that the <i>struct</i> constructor and its
definition are private. In this case variables and functions will be
private, althoug they are not marked as such.<p>
After the name it is posible to indicate if the <i>struct</i> will be
serialized:<p>
<dl>
<dt><i>serial</i></dt><dd>Functions '*_to_js' and '*_from_js' will be
created.</dd>
<dt><i>SERIAL</i></dt><dd>Functions '*_to_js' and '*_from_js' will be
created. Structure constructor will be skipped.</dd>
<dt><i>to</i></dt><dd>Only the function '*_to_js' will be created.</dd>
<dt><i>from</i></dt><dd>Only the function '*_from_js' will be created.</dd>
<dt><i>FROM</i></dt><dd>Only the function '*_from_js' will be created.
Structure constructor will be skipped.</dd>
</dl>
<b>Example 1</b><br>
<table><tr><td>
  Person : serial
</td></tr></table>

Creates a <i>struct</i> with a public constructor and serialializable.<p>
<b>Example 2</b><br>
<table><tr><td>
  - Person
</td></tr></table>
Creates a <i>struct</i> with a private constructor without serializations.

<!-        ____________________ -><hr><span id = "argument">
<u><big><i>ConstructorArguments</big></i></u>:
<!-        TTTTTTTTTTTTTTTTTTTT ->

<table><tr><td>
  [ - | @ ] <i>name</i> : <i>type</i>
</td></tr></table>
Examples:
<table><tr><td>
  dni : char *
</td></tr></table>
<table><tr><td>
  dni    : char *
  @ sel  : bool
  - port : Port
  pets   : Arr - pet
  ages   : [int]
  assets : double
</td></tr></table>
An argument will be exposed as a 'getter' property.
The optional symbols '-' and '@' mean:
<dl>
<dt>-</dt><dd>Argument is private (neither getter nor setter).</dd>
<dt>@</dt><dd>Argument is exposed as a 'getter/setter'</dd>
</dl>
Types allowed are:
<table><tr><td>
  <tt>bool | char | int</tt>
    Serialized as int
  <tt>long | size_t | time_t</tt>
    Serialized as long
  <tt>float | double</tt>
    Serialized as double
  <tt>char *</tt>
    Serialized as String
  <tt>Opt - T | Arr - T | List - T | Map - T | Hash - T</tt>
    Serialization use 'serialization of T'.
    <tt>T</tt> is 'char' for 'char *', 'Xxx' for 'Xxx *' and 'xxx' for 'xxx *'
  <tt>Xxx | xxx</tt>
    Serialized as xxx_to_js and xxx_from_js in both cases
</td></tr></table>

<!-        _________ -><hr><span id = "vars">
<u><i><big>Variables</big></i></u>:
<!-        TTTTTTTTT ->

<table><tr><td>
  [ - | @ ] <i>name</i> : type : <a href="#val">Value</a>
</td></tr></table>
Examples:
<table><tr><td>
  age : int : 4
  pers : Person : person_new("Peter")
  - children : Arr - Person : arr_new()
</td></tr></table>
A variable will be exposed as a 'getter' property.
The optional symbols '-' and '@' mean:
<dl>
<dt>-</dt><dd>Variable is private (neither getter nor setter).</dd>
<dt>@</dt><dd>Variable is exposed as a 'getter/setter'</dd>
</dl>
<span id="val">
<b>Value</b><br>
Initialization of variable.

<!-        _________ -><hr><span id = "funs">
<u><i><big>Functions</big></i></u>:
<!-        TTTTTTTTT ->

<table><tr><td>
  [ - | @ ] <i>definition</i> : <a href="#val2">Value</a>
</td></tr></table>
Examples:
<table><tr><td>
  - void (*proc)(char *) : proc
</td></tr></table>
A function will be exposed as a 'getter' property.
The optional symbols '-' and '@' mean:
<dl>
<dt>-</dt><dd>Function is private (neither getter nor setter).</dd>
<dt>@</dt><dd>Function is exposed as a 'getter/setter'</dd>
</dl>
<span id="val2">
<b>Value</b><br>
Initialization of function.

<!-        ________ -><hr><span id = "examples">
<u><i><big>Examples</big></i></u>:
<!-        TTTTTTTT ->

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
    -children : Arr - Person
    ---
    sel : int : 1
    - boss : Person : person_new("Peter")
    assets : Asset : asset_new()
    ---
    char *(mk_msg)(char *) : mk_msg
  ===
  - Parent
    @ child : Person
  *&sol;
</td></tr></table>

<pre></pre>
*/

#ifndef _DOC__H
  #define _DOC__H
#endif
