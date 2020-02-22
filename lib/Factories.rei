/** Built-in Factories
 *  See Types module for explanation on the namings and types.
 */

let log: Types.factory(unit);

type writeOpt = {path: string, extension: string};

let file: Types.factory(writeOpt);
