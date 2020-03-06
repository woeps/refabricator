module L = (
  val Logs.(
        src_log(
          Src.create(
            "Refabricator.Factories",
            ~doc="logs events in factories",
          ),
        )
      )
);

let log = (_unit, fabrics) => {
  let header = "Factories.log";
  let tags = Log.name("log");

  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();
       name |> String.uppercase_ascii |> Print.emph;
       content |> Print.logAny;
       L.debug(m => m("processed %s", name, ~tags, ~header));
     });
  L.info(m =>
    m("processed %d fabric(s)", fabrics |> List.length, ~tags, ~header)
  );
  Ok();
};

let file = (opts: Types.pathDescriptor, fabrics) => {
  let header = "Factories.file";
  let tags = Log.name("file");

  L.debug(m => m("Factories.file used", ~tags, ~header));

  let errors = ref([]);
  fabrics
  |> List.iter(fabric => {
       let (name, content) = fabric();

       L.debug(m => m("Factories.file iter: %s", name, ~tags, ~header));

       let path = Filename.concat(Sys.getcwd(), opts.path);
       let ext =
         opts.extension
         |> Option.map(ext => "." ++ ext)
         |> Option.value(~default="");
       let fileName = Filename.concat(path, name ++ ext);

       L.debug(m => m("write %s to file", fileName, ~tags, ~header));

       try(content |> Fs.writeFile(fileName)) {
       | Sys_error(err) =>
         errors := [err, ...errors^];
         L.err(m => m("Error: %s", err, ~tags, ~header));
       };
     });
  L.info(m =>
    m(
      "processed %d fabric(s) with %d errors",
      fabrics |> List.length,
      errors^ |> List.length,
      ~tags,
      ~header,
    )
  );
  switch (errors^) {
  | [] => Ok()
  | errs => Error(errs |> String.concat("\n"))
  };
};
