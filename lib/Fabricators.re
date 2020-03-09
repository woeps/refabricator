module L = (
  val Logs.(
        src_log(
          Logs.Src.create(
            "Refabricator.Fabricators",
            ~doc="logs events in fabricators",
          ),
        )
      )
);

let localPath = (pathDescriptor: Types.pathDescriptor) => {
  let header = "Fabricators.localPath";
  let tags = Log.name("localPath");
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
