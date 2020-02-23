let localPath = (pathDescriptor: Types.pathDescriptor) => {
  pathDescriptor.path
  |> Fs.readFiles
  |> (
    files => {
      switch (pathDescriptor.extension) {
      | Some(ext) =>
        files
        |> List.filter(filePath =>
             filePath |> Filename.extension |> String.equal("." ++ ext)
           )
      | None => files
      };
    }
  )
  |> List.map((filePath, ()) => {
       Print.log("Fabricators.localPath", filePath);
       (filePath |> Filename.basename, filePath |> Fs.file2string);
     });
};
