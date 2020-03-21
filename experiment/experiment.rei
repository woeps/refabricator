/* TYPES */
type meta = {name: string};
type payload('t) = {
  meta,
  data: 't,
};
type fabric('t) = React.event(payload('t));

type trigger = unit => unit;

type fabricator('t) = (fabric('t), trigger);

type refabricator('a, 'b) = fabricator('a) => fabricator('b); // this will be usually done by using React's (partially applied) map or lift functions

type factory('a, 'outOk, 'outErr) =
  fabricator('a) => fabricator(Result.t('outOk, 'outErr));

type err = [ | `writeError];

/* COMMON FUNCTIONS */
let fabricator: (list('a), 'a => payload('b)) => fabricator('b);

let refabricator: (payload('a) => payload('b)) => refabricator('a, 'b);

let map: (payload('a) => payload('b), fabric('a)) => fabric('b);

let filter: (payload('a) => bool, fabric('a)) => fabric('a);

let all: list(fabricator('a)) => fabricator('a);

let run: fabricator(Result.t('ok, 'err)) => unit;

/* FACTORIES */
let log: factory(string, string, err);

/* FABRICATORS */
let fab1: (string, int) => fabricator(string);

/* REFABRICATORS */
let parsedFiles: string => refabricator(string, string);

let between:
  (~before: string, ~after: string) => refabricator(string, string);

/* EXAMPLE */
let main: unit => unit;
