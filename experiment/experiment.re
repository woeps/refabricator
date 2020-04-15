open Lambda_streams;
let foo = Finite.Sync.from_list([1, 2, 3]);

let bar = foo |> Finite.Sync.map(x => x * 2 - 1) |> Finite.Sync.to_list;
Console.log(bar);

let triple = Finite.Async.map(x => x ++ x ++ x);
let between = (~before="", ~after="") =>
  Finite.Async.map(x => before ++ x ++ after);
let delay = delay =>
  Finite.Async.map(x => {
    Unix.sleep(delay);
    x;
  });

let all: list(Finite.Async.t('a)) => Finite.Async.t('a) =
  streams => {
    let openStreams = ref(List.length(streams));
    Async.make(send => {
      streams
      |> List.iter(
           Async.listen(
             fun
             | Signal.Data(_) as d => send(d)
             | EndOfSignal as d => {
                 openStreams := openStreams^ - 1;
                 if (openStreams^ == 0) {
                   send(d);
                 };
               },
           ),
         )
    });
  };

let allP: list(Finite.Async.t('a)) => Finite.Async.t('a) =
  streams => {
    let openStreams = ref(streams |> List.length);
    Async.make(send => {
      let handle =
        fun
        | Signal.Data(_) as d => send(d)
        | EndOfSignal as d => {
            openStreams := openStreams^ - 1;
            if (openStreams^ == 0) {
              send(d);
            };
          };
      streams
      |> List.fold_left(
           (cum, s) =>
             s
             |> Async.listen(evt => {
                  cum;
                  evt |> handle;
                }),
           (),
         );
    });
  };

let combine = (streams: list(Finite.Async.t('a))): Finite.Async.t('a) =>
  Async.make(send => {
    let open_streams = ref(List.length(streams));
    streams
    |> List.iter(
         Async.listen(
           fun
           | Signal.Data(_) as d => send(d)
           | EndOfSignal as eos => {
               open_streams := open_streams^ - 1;
               if (open_streams^ == 0) {
                 send(eos);
               };
             },
         ),
       );
  });

Console.log("NEXT:");
let x = Finite.Async.from_list(["a", "b", "c"]) |> triple;

let _ =
  all([
    Finite.Async.from_list(["1", "2", "3"]) |> delay(2),
    Finite.Async.from_list(["X", "Y", "Z"]) |> delay(1),
  ])
  |> Async.listen(Console.log);
//|> triple
/*|> Finite.Async.map(x => {
    print_endline("test");
    Console.log(x);
    x;
  })
  */
//|> Lambda_streams_async.Async.to_async_list
//|> Console.log;

/*let y =
  x |> Finite.Async.map(x => x * 2) |> Finite.Async.map(x => Console.log(x));
  */
