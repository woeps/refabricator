module S = Lwt_stream;

let p = Lwt_io.printl;

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
