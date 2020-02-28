let nameTag: Logs.Tag.def(string) =
  Logs.Tag.def("name", ~doc="name of the element", Format.pp_print_string);
let name: string => Logs.Tag.set =
  fabricatorName => Logs.Tag.(empty |> add(nameTag, fabricatorName));
