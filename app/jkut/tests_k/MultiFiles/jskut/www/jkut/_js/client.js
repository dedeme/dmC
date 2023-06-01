// Copyright 05-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as js from './js.js';
import * as str from './str.js';
import * as dic from './dic.js';
import * as b64 from './b64.js';
import * as cryp from './cryp.js';
import * as timer from './timer.js';
import * as storage from './storage.js';

const klen = 300;
let isDmCgi = true;
let appName = "";
let user = "";
let sessionKey = "0";
let connectionKey = "";
let levelV = "";
let fExpired = () => {};

// \s -> <promise>s
function sendServer (jsRq) {
  return new Promise(function (resolve, reject) {
    const request = new XMLHttpRequest();
    request.open(
      "POST",
      "http://" + location.host + (isDmCgi ? "/cgi-bin/ccgi.sh" : ""),
      true
    );
    request.onload = () => {
      if (request.status >= 200 && request.status < 300)
        resolve(request.responseText.trim());
      else reject(new Error(request.statusText));
    };
    request.onerror = () => reject(new Error("Network error: " + request.statusText));
    request.onabort = () => reject(new Error("Network error: " + request.statusText));
    request.setRequestHeader(
      "Content-Type",
      "text/plain"
    );
    request.send(appName + ":" + jsRq);
  });
}

// \b, {*...} -> <promise>{*...}
async function sendCommon (isSecure, rq) {
  let rp = "<without response>";
  try {
    rp = await sendServer(isSecure
      ? sessionId() + ":" + connectionKey + ":" + cryp.encode(sessionKey, js.w(rq))
      : sessionId() + ":" + cryp.encode(sessionKey, js.w(rq))
    );
    return js.r(cryp.decode(sessionKey, rp));
  } catch (e) {
    try {
      const data = js.r(cryp.decode("nosession", rp));
      if (dic.hasKey(data, "expired")) {
        fExpired();
        return {};
      }
      throw(e);
    } catch (e2) {
      throw new Error(str.fmt(
        "RAW SERVER RESPONSE:\n%v\nCLIENT ERROR:\n%v", [rp, e2.message]
      ));
    }
  }
}

// \s, s, b -> <promise>b
export async function authentication (userName, pass, withExpiration) {
  sys.$params(arguments.length, 3);

  sessionKey = cryp.key(appName, klen);
  const p = crypPass(pass);
  const exp = withExpiration ? "1" : "0";
  let rp = "<without response>";
  try {
    rp = await sendServer(
      ":" + cryp.encode(sessionKey, str.fmt("%v:%v:%v", [userName, p, exp]))
    );
    const data = js.r(cryp.decode(sessionKey, rp));
    const sessionId = data["sessionId"];
    if (sessionId === "") return false;

    storage.put("Client_sessionId_" + appName, sessionId);
    user = userName;
    sessionKey = data["key"];
    levelV = data["level"];
    connectionKey = ["conKey"];
    return true;
  } catch (e) {
    throw new Error(str.fmt(
      "RAW SERVER RESPONSE:\n%v\nCLIENT ERROR:\n%v", [rp, e.message]
    ));
  }
}

// \-> <promise>b
export async function connect () {
  sys.$params(arguments.length, 0);

  let rp = "<without response>";
  try {
    rp = await sendServer(sessionId());
    const data = js.r(cryp.decode(sessionId(), rp));
    sessionKey = data["key"];
    if (sessionKey === "") return false;

    user = data["user"];
    levelV = data["level"];
    connectionKey = data["conKey"];
    return true;
  } catch (e) {
    throw new Error(str.fmt(
      "RAW SERVER RESPONSE:\n%v\nCLIENT ERROR:\n%v", [rp, e.message]
    ));
  }
}

// \s -> s
export function crypPass (pass) {
  sys.$params(arguments.length, 1);
  return cryp.key(pass, klen);
}

// \b, s, (\->()) -> ()
export function init (isDmCgiP, appNameP, fExpiredP) {
  sys.$params(arguments.length, 3);
  isDmCgi = isDmCgiP;
  appName = appNameP;
  fExpired = fExpiredP;
}

// \-> i
export function level () {
  sys.$params(arguments.length, 0);
  return levelV;
}

// \{*...} -> <promise>{*...}
export async function longRun (rq) {
  sys.$params(arguments.length, 1);

  const tm = timer.mk(1000);
  let count = 0;
  rq["longRunFile"] = "";
  let rp = await send(rq);

  if (dic.hasKey(rp, "longRunEnd") && rp["longRunEnd"]) return rp;

  timer.run(tm, async () => {
    rp = await send(rq);
    ++count;

    if (dic.hasKey(rp, "longRunEnd") && rp["longRunEnd"]) timer.stop(tm);
    else if (count >= 60) throw new Error("Long run time over");
    return rp;
  });
}

// \-> s
export function sessionId () {
  sys.$params(arguments.length, 0);
  const r = storage.get("Client_sessionId_" + appName);
  return sys.asBool(r) ? r[0] : b64.encode("0");
}

// \{*...} -> <promise>{*...}
export async function send (rq) {
  sys.$params(arguments.length, 1);
  return await sendCommon(false, rq);
}

// \{*...} -> <promise>{*...}
export async function ssend (rq) {
  sys.$params(arguments.length, 1);
  return await sendCommon(true, rq);
}

// \-> s
export function userName () {
  sys.$params(arguments.length, 0);
  return user;
}


