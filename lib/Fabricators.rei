/** Built-in Fabricators
 *  See Types module for explanation on the namings and types.
 */

type mdOpt = {path: string};

let md: Types.fabricator(mdOpt, [ | `Html(string)]);
/** Load Markdown files from a specified (relative) path */;
