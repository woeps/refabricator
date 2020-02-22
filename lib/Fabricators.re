let md = path =>
  path
  |> Fs.readFiles
  |> Fs.filterMd
  |> List.map((filePath, ()) =>
       (
         filePath |> Filename.basename |> Filename.remove_extension,
         filePath |> Fs.file2string |> Md.string2html,
       )
     );
