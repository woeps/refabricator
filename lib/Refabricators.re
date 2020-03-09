module L = (
  val Logs.(
        src_log(
          Src.create(
            "Refabricator.Refabricators",
            ~doc="logs events in refabricators",
          ),
        )
      )
);

let md = (_, fabrics) => {
  let header = "Refabricators.md";
  let tags = Log.name("md");

  let fabrics =
    fabrics
    |> List.map((fabric, ()) => {
         let fabric = fabric();
         if (!(fabric |> fst |> Filename.extension |> String.equal(".md"))) {
           /* TODO: testing for markdown shouldn't be done by file extension,
            *       since markdown could come from any source (given more fabricators)
            */
           L.warn(m =>
             m(
               "It seems you are trying to parse a file with md() which is not a markdown file: %s",
               fabric |> fst,
               ~tags,
               ~header,
             )
           );
         };
         let fabric =
           fabric |> Util.modify(content => content |> Md.string2html);
         L.debug(m => m("processed %s", fabric |> fst, ~tags, ~header));
         fabric;
       });
  L.info(m =>
    m("processed %d fabric(s)", fabrics |> List.length, ~tags, ~header)
  );
  fabrics;
};

let between = ((before, after), fabrics) => {
  let header = "Refabricators.between";
  let tags = Log.name("between");

  let fabrics =
    fabrics
    |> List.map((fabric, ()) => {
         let fabric =
           fabric() |> Util.modify(content => before ++ content ++ after);
         L.debug(m => m("processed %s", fabric |> fst, ~tags, ~header));
         fabric;
       });
  L.info(m =>
    m("processed %d fabric(s)", fabrics |> List.length, ~tags, ~header)
  );
  fabrics;
};
