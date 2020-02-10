/** Uility Functions for fabrics */

let concat: list(Types.fabrics) => Types.fabrics;
/** Combine a list of several `fabrics` into a single `fabrics`. */

let rename: (string => string, Types.t) => Types.t;
/** Rename the result of a fabric */

let modify: (string => string, Types.t) => Types.t;
/** Modify the content of a fabric */
