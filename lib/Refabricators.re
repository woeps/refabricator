let between=
  ((before, after), fabrics) => {
    fabrics
    |> List.map((fabric, unit_) =>
         unit_ |> fabric |> Util.modify(content => before ++ content ++ after)
       );
  };
