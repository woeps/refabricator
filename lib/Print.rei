/** Utility Functions to write to the stdout */

let info: string => unit;
let success: string => unit;
let error: string => unit;
let emph: string => unit;
let log: (string, string) => unit;

let logAny: 'a => unit;
