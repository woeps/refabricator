module S = Lwt_stream;

type markdown = string;

module Fabricators = {
  /** TODO: recursively read files from dir */
  let localPath: string => S.t(string) =
    path =>
      Lwt_unix.files_of_directory(path)
      |> S.map(f => f |> Filename.concat(path));
};

module Refabricators = {
  let string2html = md => md |> Omd.of_string |> Omd.to_html;

  let md: S.t(string) => S.t(markdown) =
    stream => {
      stream
      |> Lwt_stream.filter_map_s(fileName => {
           switch (Filename.extension(fileName)) {
           | ".md" =>
             let fileDescriptor = Unix.openfile(fileName, [], 640);
             fileDescriptor
             |> Lwt_io.of_unix_fd(
                  ~close=() => Unix.close(fileDescriptor) |> Lwt.return,
                  ~mode=Input,
                )
             |> Lwt_io.read
             |> Lwt.map(x => Some(string2html(x)));

           | _ => Lwt.return_none
           }
         });
    };
};

module Factories = {
  let log: S.t(string) => Lwt.t(unit) =
    stream => {
      stream
      |> S.map_s(x => Lwt_io.printl("\n\nLOG:\n"++x))
      |> S.to_list
      |> Lwt.map(_ => ());
    };
};

let main =
  "./pages" |> Fabricators.localPath |> Refabricators.md |> Factories.log;

Lwt_main.run(main);

/*
 let (stream, push) = S.create();

 let main =
   stream
   |> S.map_s(x => Lwt_io.printl(string_of_float(x)) |> Lwt.map(_ => x))
   |> S.map_s(x => Lwt_unix.sleep(10. -. x) |> Lwt.map(_ => x))
   |> S.map_s(x => x |> string_of_float |> Lwt_io.printl |> Lwt.map(_ => x))
   |> S.to_list
   |> Lwt.map(l => Console.log(l));

 Lwt.async(() => {
   [4., 2., 1., 0.]
   |> List.map(x => Lwt_unix.sleep(x) |> Lwt.map(_ => push(Some(x))))
   |> Lwt.all
   |> Lwt.map(_ => push(None))
 });

 Lwt_main.run(main);
 */
