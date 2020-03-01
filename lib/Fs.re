module L = (
  val Logs.(
        src_log(
          Logs.Src.create(
            "Refabricator.Fs",
            ~doc="logs events in filesystem operations",
          ),
        )
      )
);

let readFiles = path =>
  path
  |> Sys.readdir
  |> Array.map(fileName => Filename.concat(path, fileName))
  |> Array.to_list;

let filterMd = files =>
  files
  |> List.filter(fileName => Filename.check_suffix(fileName, ".md"))
  |> List.sort(String.compare);

let file2string = filename => {
  let file = open_in(filename);
  let size = in_channel_length(file);
  let buf = Bytes.create(size);
  really_input(file, buf, 0, size);
  let fileString = buf |> Bytes.to_string;
  close_in(file);
  fileString;
};

let writeFile = (path, content) => {
  L.debug(m =>
    m(
      "Write file %s. Currently existing files in same dir: %s",
      path,
      path |> Filename.dirname |> readFiles |> String.concat(", "),
    )
  );
  let oc = path |> open_out;
  content |> output_string(oc);
  close_out(oc);
};
