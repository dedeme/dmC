// Copyright 03-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as iter from './iter.js';
import * as arr from './arr.js';
import * as timer from './timer.js';
import * as domo from './domo.js';

// \s -> ()
export function alert (s) {
  sys.$params(arguments.length, 1);
  window.alert(s);
}

// \-> ()
export function beep () {
  sys.$params(arguments.length, 0);
  const au = new AudioContext();
  const o = au.createOscillator();
  o.frequency.value = 990;
  o.connect(au.destination);
  o.start(0);
  timer.delay(120, () => o.stop(0));
}

// \<domo> -> <domo>
export function changePoint (inp) {
  sys.$params(arguments.length, 1);
  const el = inp.e;
  const ac = el.onkeydown;
  el.onkeydown = function (e) {
    if (e.keyCode === 110) {
      const start = el.selectionStart;
      const end = el.selectionEnd;
      const text = el.value;

      el.value = text.substring(0, start) + "," + text.substring(end);
      el.selectionStart = start + 1;
      el.selectionEnd = start + 1;

      return false;
    }

    if (ac !== null) ac(e);
    return true;
  }
  return inp;
}

// \s -> b
export function confirm (s) {
  sys.$params(arguments.length, 1);
  return window.confirm(s);
}

// \s -> <domo>
export function field (targetId) {
  sys.$params(arguments.length, 1);
  const r = q("input").att("type", "text");
  r.e.onkeydown = function (e) {
    if (e.keyCode === 13) {
      e.preventDefault();
      q("#" + targetId).e.focus();
    }
  }
  return r;
}

// \s, i -> <domo>
export function hrule (label, margin) {
  sys.$params(arguments.length, 2);
  return q("table")
    .style(
      "border-collapse: collapse;" +
      "border : 0px;" +
      "width : 100%")
    .add(q("tr")
      .add(q("td")
        .style("width: " + margin + "px")
        .add(q("hr")))
      .add(q("td")
        .style("white-space:nowrap; width: 5px")
        .text(" " + label + " "))
      .add(q("td")
        .add(q("hr"))))
  ;
}

// \<domo>, (\<fileList>->()) -> ()
export function ifiles (o, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  const style = o.getAtt("style");
  function handleDragOver (evt) {
    o.att("style", style + ";background-color: rgb(240, 245, 250);");
    evt.stopPropagation();
    evt.preventDefault();
    evt.dataTransfer.dropEffect = 'copy';
  }
  o.e.addEventListener("dragover", handleDragOver, false);

  o.e.addEventListener("dragleave", function (evt) {
    o.att("style", style);
  }, false);

  function handleDrop (evt) {
    o.att("style", style);
    evt.stopPropagation();
    evt.preventDefault();

    fn(evt.dataTransfer.files);
  }
  o.e.addEventListener("drop", handleDrop, false);

  return o;
}

// \s -> <domo>
export function img (name) {
  sys.$params(arguments.length, 1);
  if (name.indexOf(".") === -1) name = name + ".png";
  return q("img").att("src", "img/" + name);
}

// \s, i -> <domo>
export function led (color, side) {
  sys.$params(arguments.length, 2);
  return q("div")
    .style(
      "padding:" + side + "px;" +
       "border: 1px solid #002040;border-radius: " + side + "px;" +
       "background: " + color + ";"
    );
}

// \s -> <domo>
export function lightImg (name) {
  sys.$params(arguments.length, 1);
  return img(name).att("style", "opacity:0.4");
}

// \(\->() | \<event>->()) -> ()
export function link (fn) {
  sys.$params(arguments.length, 1);
  return q("span").att("style", "cursor:pointer").on("click", fn);
}

// \<event> -> n
export function mouseX (ev) {
  sys.$params(arguments.length, 1);
  return document.documentElement.scrollLeft +
    document.body.scrollLeft +
    ev.clientX
  ;
}

// \<event> -> n
export function mouseY (ev) {
  sys.$params(arguments.length, 1);
  return document.documentElement.scrollTop +
    document.body.scrollTop +
    ev.clientY
  ;
}

// \s -> <domo>
export function pass (targetId) {
  sys.$params(arguments.length, 1);
  const r = q("input").att("type", "password");
  r.e.onkeydown = function (e) {
    if (e.keyCode === 13) {
      e.preventDefault();
      q("#" + targetId).e.focus();
    }
  }
  return r;
}

// \\s, s -> s
export function prompt (msg, def) {
  sys.$params(arguments.length, 2);
  if (def === "") return prompt(msg);
  else return prompt(msg, def);
}

// \* -> <domo>
export function q (el) {
  sys.$params(arguments.length, 1);
  if (typeof(el) !== "string") return domo.mk(el);
  switch (el.charAt(0)) {
    case "#":
      return domo.mk(sys.$checkNull(document.getElementById(el.substring(1))));
    case "@":
      return domo.mk(sys.$checkNull(document.querySelector(el.substring(1))));
    default:
      return domo.mk(sys.$checkNull(document.createElement(el)));
  }
}

// \* -> [<domo>] | []
export function qOp (el) {
  sys.$params(arguments.length, 1);
  if (typeof(el) !== "string") return domo.mk(el);
  let e = null;
  switch (el.charAt(0)) {
    case "#":
      e = document.getElementById(el.substring(1));
      return e == null ? [] : [domo.mk(e)];
    case "@":
      e = document.querySelector(el.substring(1));
      return e == null ? [] : [domo.mk(e)];
    default:
      e = document.createElement(el);
      return e == null ? [] : [domo.mk(e)];
  }
}

/// \s -> <iter>[domo...]
export function qq (s) {
  sys.$params(arguments.length, 1);
  function toIt (lst) {
    let c = 0;
    const len = lst.length;
    return iter.mk(
      () => c < len,
      () => lst.item(c++)
    );
  }
  if (s === "") return toIt(document.getElementsByTagName("*"));
  switch (s.charAt(0)) {
    case "%": return toIt(document.getElementsByName(str.substring(1)));
    case ".": return toIt(document.getElementsByClassName(s.substring(1)));
    default: return toIt(document.getElementsByTagName(s));
  }
}

/// \s, [s...] -> <domo>
export function select (idPrefix, list) {
  sys.$params(arguments.length, 2);
  const r = q("select").att("id", idPrefix);
  arr.each(list, function (tx) {
    const op = q("option");
    if (tx.length > 0 && tx.charAt(0) === "+") {
      tx = tx.substring(1);
      op.att("selected", "true");
    }
    op.text(tx).att("name", idPrefix).att("id", idPrefix + "_" + tx);
    r.e.add(op.e);
  });
  return r;
}

/// \s -> <promise>s
export function upload (fpath) {
  sys.$params(arguments.length, 1);
  const url = fpath.charAt(0) === "/"
    ? "http://" + location.host + fpath
    : fpath
  ;
  return new Promise(function (resolve, reject) {
    const request = new XMLHttpRequest();
    request.open("GET", url, true);
    request.onload = () => {
      if (request.status >= 200 && request.status < 300)
        resolve(request.responseText);
      else reject(new Error(request.statusText));
    };
    request.onerror = () => reject(new Error(request.statusText));
    request.onabort = () => reject(new Error(request.statusText));

    request.send();
  });
}

// \s -> <domo>
export function upTop (image) {
  sys.$params(arguments.length, 1);
  return q("div")
    .style("position: fixed;bottom: 0px;right: 20px")
    .add(link(() => scroll(0, 0))
      .add(img(image)))
  ;
}

// \-> <dic>[s...]
export function url () {
  sys.$params(arguments.length, 0);
  const search = location.search;
  if (search === "") return {};

  let i = 0;
  return arr.reduce(search.substring(1).split("&"), {}, (r, e) => {
    const ix = e.indexOf("=");
    if (ix === -1) r["" + i] = decodeURI(e);
    else r[decodeURI(e.substring(0, ix))] = decodeURI(e.substring(ix + 1));
    ++i;
    return r;
  });
}


