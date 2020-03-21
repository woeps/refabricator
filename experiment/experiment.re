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
let fabricator = (l, f) => {
  let (e, send) = React.E.create();
  (e, () => l |> List.iter(x => x |> f |> send));
};

let refabricator = (f, (fabric, trigger)) => (
  React.E.map(f, fabric),
  trigger,
);

let map = (f, fabric) => React.E.map(f, fabric);

let filter = (f, fabric) => React.E.filter(f, fabric);

let all = fabricsAndTriggers => {
  let fabric = fabricsAndTriggers |> List.map(fst) |> React.E.select;
  let trigger = () =>
    fabricsAndTriggers |> List.map(snd) |> List.iter(t => t());
  (fabric, trigger);
};

let run = ((fabric, trigger)) => {
  fabric
  |> filter(p => p.data |> Result.is_error)
  |> map(p => {
       Console.error("Error in: " ++ p.meta.name);
       p;
     })
  |> ignore;
  trigger();
};

/* FACTORIES */
let log: factory(string, string, err) =
  refabricator(x => {
    Console.log(x.data);
    {...x, data: Ok(x.data)};
  });

/* FABRICATORS */
let fab1 = (path, count) =>
  fabricator(List.init(count, c => (path, c)), ((p, c)) =>
    {
      meta: {
        name: c |> string_of_int,
      },
      data: p ++ (c |> string_of_int),
    }
  );

/* REFABRICATORS */
let parsedFiles = text =>
  refabricator(fabric => {...fabric, data: fabric.data ++ text});

let between = (~before, ~after) =>
  refabricator(fabric => {...fabric, data: before ++ fabric.data ++ after});

/* EXAMPLE */
let main = () => {
  Console.log("--] EXPERIMENT [--");
  [
    fab1("./pages", 42)
    |> parsedFiles("..")
    |> between(~before=">>>", ~after=""),
    fab1("./someOtherPath", 42)
    |> parsedFiles("...")
    |> between(~before="YIPEEAH!: ", ~after="!"),
  ]
  |> all
  |> between(~before="|+| ", ~after="|-|")
  |> log
  |> run;
};

main();
