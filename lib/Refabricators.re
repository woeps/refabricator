let md = (_, fabrics) =>
  fabrics
  |> List.map((fabric, ()) => {
       let fabric = fabric();
       if (!(fabric |> fst |> Filename.extension |> String.equal(".md"))) {
         Print.error(
           "It seems you are trying to parse a file with md() which is not a markdown file: "
           ++ (fabric |> fst),
         );
       };
       Print.log("Refabricators.md", fabric |> fst);
       fabric |> Util.modify(content => content |> Md.string2html);
     });

let between = ((before, after), fabrics) => {
  fabrics
  |> List.map((fabric, ()) =>
       fabric() |> Util.modify(content => before ++ content ++ after)
     );
};
