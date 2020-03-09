/* FABRICATORS */
let from = Util.concat;

let fromMd = path =>
  Fabricators.localPath({path, extension: Some("md")}) |> Refabricators.md();

/* REFABRICATORs */
let between = Refabricators.between;

/* HELEPR */
let to_ = (factory, fabrics) => factory(fabrics) |> Util.handleFactoryResult;

let toMany = (factories, fabrics) =>
  factories
  |> List.map(factory => fabrics |> factory)
  |> List.fold_left(
       (acc, r) =>
         switch (acc, r) {
         | (Ok (), Error(msg) as err) => err
         | (Error(err1), Error(err2)) => Error(err1 ++ "\n" ++ err2)

         | (acc, _) => acc
         },
       Ok(),
     )
  |> Util.handleFactoryResult;

/* FACTORIES */
let toLog = to_(Factories.log());

let toFiles = (writeOpts, fabrics) =>
  fabrics |> to_(Factories.file(writeOpts));
