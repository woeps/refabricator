module P = Lwt;
module S = Lwt_stream;

type a = [ | `X];
type f('a, 'b) = 'a => 'b constraint 'a = [> a];

let f: f([> a], 'b) = _a => `Y;
let x = f(`Z);

module Main: {
  //---------- GENERAL TYPES ----------
  type name = string;
  type processor =
    | Fabricator(string)
    | Refabricator(string)
    | Factory(string);
  type processors = list(processor);
  type t('content, 'error) =
    pri {
      name,
      content: result('content, 'error),
      processedBy: processors,
    };

  //---------- FUNCTION TYPES ----------
  // TODO: verify if a refabricator can output different errors than it's input fabric
  type fabric('content, 'error) = Lwt_stream.t(t('content, 'error));

  type refabricator('input, 'output, 'error) =
    fabric('input, 'error) => fabric('output, 'error);

  type factory('content, 'error) =
    fabric('content, 'error) => Lwt.t(list((name, option('error))));

  //type markdown = string;

  //---------- FUNCTIONS ----------
  let fabricate:
    (~name: name, ~fabricator: string, result('content, 'error)) =>
    t('content, 'error);

  let refabricate:
    (
      ~refabricator: string,
      (~name: string, ~content: 'input) => result('output, 'error),
      fabric('input, 'error)
    ) =>
    fabric('output, 'error);
  let refabricate_async:
    (
      ~refabricator: string,
      (~name: string, ~content: 'input) => Lwt_result.t('output, 'error),
      fabric('input, 'error)
    ) =>
    fabric('output, 'error);

  let factorize:
    (
      t('input, 'error) => (name, option('error)),
      fabric('input, 'error)
    ) =>
    Lwt.t(list((name, option('error))));
  let factorize_async:
    (
      t('input, 'error) => Lwt.t((name, option('error))),
      fabric('input, 'error)
    ) =>
    Lwt.t(list((name, option('error))));
} = {
  //----------TYPES----------
  type name = string;
  type processor =
    | Fabricator(string)
    | Refabricator(string)
    | Factory(string);
  type processors = list(processor);
  type t('content, 'error) = {
    name,
    content: result('content, 'error),
    processedBy: processors,
  };
  //---------- FUNCTION TYPES ----------
  type fabric('content, 'error) = Lwt_stream.t(t('content, 'error));

  type refabricator('input, 'output, 'error) =
    fabric('input, 'error) => fabric('output, 'error);
  type factory('content, 'error) =
    fabric('content, 'error) => Lwt.t(list((name, option('error))));

  // type markdown = string;

  //----------- FUNCTIONS -----------
  let fabricate = (~name, ~fabricator, content) => {
    name,
    content,
    processedBy: [Fabricator(fabricator)],
  };

  let modify = (processor, f, t) => {
    ...t,
    content: Result.bind(t.content, content => f(~name=t.name, ~content)),
    processedBy: [processor, ...t.processedBy],
  };

  let modify_async = (processor, f, t) =>
    Lwt_result.bind(Lwt_result.lift(t.content), content =>
      f(~name=t.name, ~content)
    )
    |> Lwt.map(content =>
         {...t, content, processedBy: [processor, ...t.processedBy]}
       );

  let refabricate = (~refabricator, f, stream) =>
    stream |> Lwt_stream.map(modify(Refabricator(refabricator), f));
  let refabricate_async = (~refabricator, f, stream) =>
    stream |> Lwt_stream.map_s(modify_async(Refabricator(refabricator), f));

  let factorize = (f, stream) =>
    stream |> Lwt_stream.map(f) |> Lwt_stream.to_list;
  let factorize_async = (f, stream) =>
    stream |> Lwt_stream.map_s(f) |> Lwt_stream.to_list;
};

module Fabricators = {
  module LocalPath = {
    let name = "LocalPath";
    type error =
      | PathNotFound(string);
    let run:
      (~filterExtension: string=?, string) =>
      Main.fabric(string, [> | `LocalPath(error)]) =
      (~filterExtension=?, path) =>
        Lwt_unix.files_of_directory(path)
        |> S.filter(fileName =>
             !(fileName |> Filename.concat(path) |> Sys.is_directory)
           )
        // TODO: Instead of filtering out directories, recursively read them in
        //       and pass them down the stream
        |> S.filter_map_s(fileName => {
             let filePath = fileName |> Filename.concat(path);
             switch (filterExtension, Filename.extension(fileName)) {
             | (None, _) => P.return_some(filePath)

             | (Some(wanted), found) when wanted == found =>
               P.return_some(filePath)
             | (_, _) => P.return_none
             };
           })
        |> S.map_s(fileName => {
             Lwt_io.open_file(~mode=Input, fileName)
             |> P.bind(_, Lwt_io.read)
             |> P.map(content =>
                  Ok(content)
                  |> Main.fabricate(~name=fileName, ~fabricator="localPath")
                )
           });
  };
};

// TODO: all refabricators call Main.refabricate at the end, could this be shortened by using a functor???
module Refabricators = {
  module Markdown = {
    let name = "Markdown";
    type error =
      | Error;
    let run: Main.refabricator(string, string, [> | `MarkDown(error)]) =
      stream => {
        let string2html = (~name as _, ~content) => {
          let override: Omd_representation.element => option(string) =
            element =>
              switch (element) {
              | Url(href, children, title) =>
                let href =
                  href ++ ".html" |> Omd_utils.htmlentities(~md=true);
                let title =
                  title != ""
                    ? " title=\""
                      ++ (title |> Omd_utils.htmlentities(~md=true))
                      ++ "\""
                    : "";
                Some(
                  "<a href=\""
                  ++ href
                  ++ "\""
                  ++ title
                  ++ ">"
                  ++ Omd_backend.html_of_md(children)
                  ++ "</a>",
                );
              | _ => None
              };
          Ok(content |> Omd.of_string |> Omd.to_html(~override));
        };
        stream |> Main.refabricate(~refabricator=name, string2html);
      };
  };

  module Between = {
    let name = "Between";
    type error = unit;
    let run:
      (~before: string, ~after: string) =>
      Main.refabricator(string, string, [> | `Between(error)]) =
      (~before, ~after, stream) =>
        stream
        |> Main.refabricate(~refabricator=name, (~name as _, ~content) =>
             Ok(before ++ content ++ after)
           );
  };

  module All = {
    let name = "All";
    type error = unit;

    let rec run:
      list(Main.fabric('content, 'error)) => Main.fabric('content, 'error) =
      streams =>
        switch (streams) {
        | [s1, s2, ...tail] => Lwt_stream.append(s1, run([s2, ...tail]))
        | [s] => s
        | [] => assert(false)
        };
  };
};

// TODO: change let name to let factory etc to use punning in factorize_async
module Factories = {
  module Log = {
    let name = "Log";
    type error = unit;
    let run: Main.factory(string, [> | `Log(error)]) =
      stream => {
        stream
        |> Main.factorize_async(t =>
             switch (t.content) {
             | Ok(content) =>
               Lwt_io.printl("\n\nLOG: " ++ t.name ++ "\n" ++ content)
               |> Lwt.map(_ => (t.name, None))
             | Error(err) => (t.name, Some(err)) |> Lwt.return
             }
           );
      };
  };
};

let main =
  [
    Fabricators.LocalPath.run("./pages") /* Fabricators.LocalPath.run(
      "../../../pages2",
    ),*/
  ]
  |> Refabricators.All.run
  |> Refabricators.Markdown.run
  |> Refabricators.Between.run(
       ~before=
         "<html><head><title>Site generated with Refabricator</title></head><body><main>\n",
       ~after=
         "\n"
         ++ {|</main><footer>This site was generated by <a href="https://github.com/woeps/Refabricator">Refabricator</a>!</footer></body></html>|},
     )
  |> Factories.Log.run
  |> Lwt.bind(
       _,
       Lwt_list.map_p(((name, maybeError)) => {
         Pastel.(
           (
             switch (maybeError) {
             | None => <Pastel color=Green> {name ++ ":\n    OK"} </Pastel>
             | Some(_err) =>
               // TODO: Pretty Print errors
               <Pastel color=Red> {name ++ ":\n    EROR"} </Pastel>
             }
           )
           |> Lwt_io.printl
         )
       }),
     );

Lwt_main.run(main);

/*
 let (stream, push) = S.create();

 let main =
   stream
   |> S.map_s(x => Lwt_io.printl(string_of_float(x)) |> P.map(_ => x))
   |> S.map_s(x => Lwt_unix.sleep(10. -. x) |> P.map(_ => x))
   |> S.map_s(x => x |> string_of_float |> Lwt_io.printl |> P.map(_ => x))
   |> S.to_list
   |> P.map(l => Console.log(l));

 P.async(() => {
   [4., 2., 1., 0.]
   |> List.map(x => Lwt_unix.sleep(x) |> P.map(_ => push(Some(x))))
   |> P.all
   |> P.map(_ => push(None))
 });

 Lwt_main.run(main);
 */
