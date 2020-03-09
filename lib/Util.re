let concat = fabrics => List.concat(fabrics);

let rename = (rename, (name, content)) => (name |> rename, content);

let modify = (modify, (name, content)) => (name, modify(content));

/* TODO: move to bin */
let handleFactoryResult =
  fun
  | Ok () => "Success!" |> Print.success
  | Error(msg) => "Error: " ++ msg |> Print.error;
