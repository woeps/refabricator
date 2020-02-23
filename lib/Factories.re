let log = (_unit, fabrics) => {
  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();
       name |> String.uppercase_ascii |> Print.emph;
       content |> Print.logAny;
     });
  Ok();
};

let file = (opts: Types.pathDescriptor, fabrics) => {
  let errors = ref([]);
  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();
       try(
         content
         |> Fs.writeFile(
              Filename.concat(
                Sys.getcwd(),
                Filename.concat(
                  opts.path,
                  name
                  ++ (
                    opts.extension
                    |> Option.map(ext => "." ++ ext)
                    |> Option.value(~default="")
                  ),
                ),
              ),
            )
       ) {
       | Sys_error(err) => errors := [err, ...errors^]
       };
     });
  switch (errors^) {
  | [] => Ok()
  | errs => Error(errs |> String.concat("\n"))
  };
};
