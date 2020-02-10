/** Utility Functions for the filesystem */

let readFiles: string => list(string);
/** Get a list of all files (absolute path) in a given relative path */

let filterMd: list(string) => list(string);
/** Get only Markdown files from a list of files */

let file2string: string => string;
/** Read a file (absolute path) in as a string */

let writeFile: (/*path*/ string, /*content*/ string) => unit;
/** Write a string into a file at the specified path */
