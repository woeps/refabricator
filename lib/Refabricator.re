/* FABRICATORS */
let from = Util.concat;
let fromMd = Fabricators.md;

/* REFABRICATORs */
let between = Refabricators.between;

let handleFactoryResult =
  fun
  | Ok () => "Success!" |> Print.success
  | Error(msg) => "Error: " ++ msg |> Print.error;

let to_ = (factory, fabrics) => factory(fabrics) |> handleFactoryResult;

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
  |> handleFactoryResult;

/* FACTORIES */
let toLog = to_(Factories.log());
