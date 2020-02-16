let modify = (modFn, fabrics) =>
  fabrics |> List.map((fabric, ()) => fabric() |> Util.modify(modFn));

let between = ((before, after), fabrics) => {
  fabrics
  |> modify(
       fun
       | `Html(content) => `Html(before ++ content ++ after),
     );
};
