export function encode (s) {
  return btoa(encodeURIComponent(s).replace(
    /%([0-9A-F]{2})/g,
    (match, p1) => String.fromCharCode(parseInt("0x" + p1, 16))
  ));
}
export function decode (b64) {
  return decodeURIComponent(atob(b64).split("").map(
    c =>"%" + ("00" + c.charCodeAt(0).toString(16)).slice(-2)
  ).join(""));
}
export function encodeBytes (bs) {
  return btoa(String.fromCharCode.apply(null, bs));
}
export function decodeBytes (b64) {
  return new Uint8Array(atob(b64).split("").map(c => c.charCodeAt(0)));
}
