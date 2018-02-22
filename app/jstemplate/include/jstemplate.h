// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Templates to easy making js code.
/// <style>h3{font-weight:bold;color:#004080;font-family:monospace}</style>
/// <h2>jstemplate Use</h2>
/// From command line use:
///   jstemplate file
/// For example:
///   jstemplate "src/main.js"
/// <h2>Working with jstemplate</h2>
/// Modify a js file changing templates to javascript code.<br>
/// If a fail happens the js file will not be modified.
/// <h2>Templates</h2>
/// Valid templates are:
/// <h3>class: Generates a seriazable class.</h3>
///   &lt;&lt;&lt;class:className
///   field1:type
///   field2:type
///   @field3:type
///   ...
///   >>>
///   field is a get field
///   @field is a get/set field
/// <h3>func: Generates a function skeleton</h3>
///   &lt;&lt;&lt;func:functionName
///   field1:type
///   field2:type
///   ...
///   returnType
///   >>>
/// fields and return are optional.<br>
/// If returnType is missing its value is set to 'void'
/// <h3>meth: Generates a method skeleton</h3>
///   &lt;&lt;&lt;meth:methodName
///   field1:type
///   field2:type
///   ...
///   returnType
///   >>>
/// fields and return are optional.<br>
/// If returnType is missing is value is set to 'void'
/// <h3>static: Generates a static method skeleton</h3>
///   &lt;&lt;&lt;static:staticMethodName
///   field1:type
///   field2:type
///   ...
///   returnType
///   >>>
/// fields and return are optional.<br>
/// returnType must be the last line.<br>
/// If returnType is missing, is value is set to 'void'.
/// <h3>vars: Declares 'let' variables out of class.
/// They will be linked with &lt;&lt;&lt;link:</h3>
///   &lt;&lt;&lt;vars:
///   field1:type
///   field2:type
///   &#64;field2:type
///   ...
///   >>>
/// field is a get field<br>
/// @field is a get/set field
/// <h3>consts: Declares constants out of class.
/// They will be linked with &lt;&lt;&lt;link:</h3>
///   &lt;&lt;&lt;consts:
///   field1:type
///   field2:type
///   ...
///   >>>
/// <h3>pars: Declares 'this_' variables.
/// They will be linked with &lt;&lt;&lt;link:</h3>
///   &lt;&lt;&lt;pars:
///   field1:type
///   field2:type
///   @field2:type
///   ...
///   >>>
/// field is a get field.<br>
/// @field is a get/set field
/// <h3>&lt;&lt;&lt;links: Generates links to all the vars, consts and pars
/// previously declared</h3>
///   &lt;&lt;&lt;links:
///   >>>

#ifndef JSTEMPLATE_H
  #define JSTEMPLATE_H

///
int main (int argc, char **argv);

#endif
