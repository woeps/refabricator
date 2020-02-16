/** Uility Functions for fabrics */

let concat: list(Types.fabrics('content)) => Types.fabrics('content);
/** Combine a list of several `fabrics` into a single `fabrics`. */

let rename: (string => string, Types.t('content)) => Types.t('content);
/** Rename the result of a fabric */

let modify: ('content => 'modifiedContent, Types.t('content)) => Types.t('modifiedContent);
/** Modify the content of a fabric */

let handleFactoryResult: result(unit, string) => unit;
/** Print either error or success message */
