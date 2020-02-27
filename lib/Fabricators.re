module L = {
  let src =
    Logs.Src.create(
      "Refabricator.Refabricators",
      ~doc="logs events in fabricators",
    );
  let nameTag: Logs.Tag.def(string) =
    Logs.Tag.def("name", ~doc="name of the element", Format.pp_print_string);
  let name: string => Logs.Tag.set =
    fabricatorName => Logs.Tag.(empty |> add(nameTag, fabricatorName));

  let debug = m => Logs.debug(~src, m);
  let info = m => Logs.info(~src, m);
  let warn = m => Logs.warn(~src, m);
  let err = m => Logs.err(~src, m);
};

let localPath = (pathDescriptor: Types.pathDescriptor) => {
  let header = "Fabricators.localPath";
  let tags = L.name("localPath");
  L.debug(m =>
    m(
      "start {path=%s, extension=%s}",
      pathDescriptor.path,
      pathDescriptor.extension |> Option.value(~default=""),
      ~tags,
      ~header,
    )
  );

  let files = pathDescriptor.path |> Fs.readFiles;

  L.debug(m =>
    m(
      "files in path(%d): %s",
      files |> List.length,
      files |> String.concat(", "),
      ~tags,
      ~header,
    )
  );

  let fabrics =
    (
      switch (pathDescriptor.extension) {
      | Some(ext) =>
        let filteredFiles =
          files
          |> List.filter(filePath =>
               filePath |> Filename.extension |> String.equal("." ++ ext)
             );
        L.debug(m =>
          m(
            "filtered Files(extension=%s): %s",
            pathDescriptor.extension |> Option.value(~default=""),
            filteredFiles |> String.concat(", "),
            ~tags,
            ~header,
          )
        );
        filteredFiles;
      | None =>
        L.debug(m => m("no files filtered", ~tags, ~header));
        files;
      }
    )
    |> List.map((filePath, ()) => {
         let fileName = filePath |> Filename.basename;
         L.debug(m => m("generated fabric: %s", fileName, ~tags, ~header));
         (fileName, filePath |> Fs.file2string);
       });
  L.info(m =>
    m("generated %d fabric(s)", fabrics |> List.length, ~tags, ~header)
  );
  fabrics;
};
