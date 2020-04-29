module P = Lwt;
module S = Lwt_stream;

module Main: {
  type name = string;
  type processor =
    | Fabricator(string)
    | Refabricator(string)
    | Factory(string);
  type processors = list(processor);
  type t('content) =
    pri {
      name,
      content: 'content,
      processedBy: processors,
    };
  type fabric('content) = Lwt_stream.t(t('content));

  type refabricator('input, 'output) = fabric('input) => fabric('output);

  type factory('content) =
    fabric('content) => Lwt.t(list(t(result('content, list(string)))));

  type markdown = string;

  let fabricate: (~name: name, ~fabricator: string, 'content) => t('content);

  let refabricate:
    (
      ~refabricator: string,
      (~name: string, ~content: 'input) => 'output,
      fabric('input)
    ) =>
    fabric('output);
  let refabricate_async:
    (
      ~refabricator: string,
      (~name: string, ~content: 'input) => Lwt.t('output),
      fabric('input)
    ) =>
    fabric('output);

  let factorize:
    (
      ~factory: string,
      (~name: string, ~content: 'input) => 'output,
      fabric('input)
    ) =>
    fabric('output);
  let factorize_async:
    (
      ~factory: string,
      (~name: string, ~content: 'input) => Lwt.t('output),
      fabric('input)
    ) =>
    fabric('output);

  let all: list(fabric('content)) => fabric('content);
} = {
  //----------TYPES----------
  type name = string;
  type processor =
    | Fabricator(string)
    | Refabricator(string)
    | Factory(string);
  type processors = list(processor);
  type t('content) = {
    name,
    content: 'content,
    processedBy: processors,
  };
  type fabric('content) = Lwt_stream.t(t('content));

  type refabricator('input, 'output) = fabric('input) => fabric('output);
  type factory('content) =
    fabric('content) => Lwt.t(list(t(result('content, list(string)))));
  //-------------------------

  type markdown = string;

  let fabricate = (~name, ~fabricator, content) => {
    name,
    content,
    processedBy: [Fabricator(fabricator)],
  };

  let modify = (processor, f, t) => {
    name: t.name,
    content: f(~name=t.name, ~content=t.content),
    processedBy: [processor, ...t.processedBy],
  };

  let modify_async = (processor, f, t) =>
    f(~name=t.name, ~content=t.content)
    |> Lwt.map(content =>
         {name: t.name, content, processedBy: [processor, ...t.processedBy]}
       );

  let refabricate = (~refabricator, f, stream) =>
    stream |> Lwt_stream.map(modify(Refabricator(refabricator), f));
  let refabricate_async = (~refabricator, f, stream) =>
    stream |> Lwt_stream.map_s(modify_async(Refabricator(refabricator), f));

  let factorize = (~factory, f, stream) =>
    stream |> Lwt_stream.map(modify(Factory(factory), f));
  let factorize_async = (~factory, f, stream) =>
    stream |> Lwt_stream.map_s(modify_async(Factory(factory), f));

  let rec all = streams =>
    switch (streams) {
    | [s1, s2, ...tail] => Lwt_stream.append(s1, all([s2, ...tail]))
    | [s] => s
    | [] => assert(false)
    };
};

module Fabricators = {
  let localPath: (~filterExtension: string=?, string) => Main.fabric(string) =
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
           |> P.map(Main.fabricate(~name=fileName, ~fabricator="localPath"))
         });
};

module Refabricators = {
  let markdown: Main.refabricator(string, Main.markdown) =
    stream => {
      let string2html = (~name as _, ~content) => {
        let override: Omd_representation.element => option(string) =
          element =>
            switch (element) {
            | Url(href, children, title) =>
              let href = href ++ ".html" |> Omd_utils.htmlentities(~md=true);
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
        content |> Omd.of_string |> Omd.to_html(~override);
      };
      stream |> Main.refabricate(~refabricator="markdown", string2html);
    };

  let between:
    (~before: string, ~after: string) => Main.refabricator(string, string) =
    (~before, ~after, stream) =>
      stream
      |> Main.refabricate(~refabricator="between", (~name as _, ~content) =>
           before ++ content ++ after
         );
};

module Factories = {
  // FIXME: this needs to put the result type inside of {... content ...} => use Main.factorize
  let log: Main.factory(string) =
    stream => {
      stream
      |> Main.factorize_async(~factory="log", (~name, ~content) =>
           Lwt_io.printl("\n\nLOG: " ++ name ++ "\n" ++ content)
           |> Lwt.map(_ => Ok(content))
         )
      |> S.to_list;
    };
};

let main =
  [Fabricators.localPath("./pages"), Fabricators.localPath("./pages2")]
  |> Main.all
  |> Refabricators.markdown
  |> Refabricators.between(
       ~before=
         "<html><head><title>Site generated with Refabricator</title></head><body><main>\n",
       ~after=
         "\n"
         ++ {|</main><footer>This site was generated by <a href="https://github.com/woeps/Refabricator">Refabricator</a>!</footer></body></html>|},
     )
  |> Factories.log
  |> Lwt.bind(
       _,
       Lwt_list.map_p((x: Main.t(result('content, list(string)))) => {
         Pastel.(
           (
             switch (x.content) {
             | Ok(_content) =>
               <Pastel color=Green> {x.name ++ ":\n    OK"} </Pastel>
             | Error(err) =>
               <Pastel color=Red>
                 {x.name
                  ++ ":\n    EROR:"
                  ++ List.fold_right(
                       (e, acc) => acc ++ "\n        " ++ e,
                       err,
                       "",
                     )}
               </Pastel>
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
