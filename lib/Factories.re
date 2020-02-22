let log = (_unit, fabrics) => {
  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();
       name |> String.uppercase_ascii |> Print.emph;
       content |> Print.log;
     });
  Ok();
};

type writeOpt = {
  path: string,
  extension: string,
};

let file = (opts, fabrics) => {
  let errors = ref([]);
  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();
       try(
         content
         |> Fs.writeFile(
              Filename.(
                concat(
                  Sys.getcwd(),
                  concat(opts.path, name ++ "." ++ opts.extension),
                )
              ),
            )
       ) {
       | Sys_error(err) =>
         errors := [err, ...errors^];
       };
     });
  switch (errors^) {
  | [] => Ok()
  | errs => Error(errs |> String.concat("\n"))
  };
};
