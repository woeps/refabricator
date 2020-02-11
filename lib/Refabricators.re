let between = ((before, after), fabrics) => {
  fabrics
  |> List.map((fabric, ()) =>
       fabric() |> Util.modify(content => before ++ content ++ after)
     );
};
