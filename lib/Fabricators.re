type mdOpt = {path: string};

let md = opt =>
  opt.path
  |> Fs.readFiles
  |> Fs.filterMd
  |> List.map((filePath, ()) =>
       (
         filePath |> Filename.basename,
         `Html(filePath |> Fs.file2string |> Md.string2html),
       )
     );
