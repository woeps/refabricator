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
  buf |> Bytes.to_string;
};

let writeFile = (path, content) => {
  let oc = path |> open_out;
  content |> output_string(oc);
  close_out(oc);
};
