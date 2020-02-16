let log = (_unit, fabrics) => {
  fabrics
  |> List.iter(fabric => {
       let (name, `Html(content)) = fabric();
       name |> String.uppercase_ascii |> Print.emph;
       content ++ "\n" |> Print.log;
     });
  Ok();
};
