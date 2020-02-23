let info = text =>
  Pastel.(<Pastel italic=true> text </Pastel>) |> print_endline;

let success = text =>
  Pastel.(<Pastel color=Green> text </Pastel>) |> print_endline;

let error = text =>
  Pastel.(<Pastel color=Red> text </Pastel>) |> prerr_endline;

let emph = text => Pastel.(<Pastel bold=true> text </Pastel>) |> print_endline;

let log = (label, text) =>
  Pastel.(
    <Pastel color=BlueBright>
      <Pastel bold=true> {label ++ ": "} </Pastel>
      text
    </Pastel>
  )
  |> print_endline;

let logAny = Console.log;
