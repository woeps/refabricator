let log = (_unit, fabrics) => {
  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();
       name |> String.uppercase_ascii |> Print.emph;
       content |> Print.log;
     });
  Ok();
};
