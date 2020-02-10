let concat = fabrics => List.concat(fabrics);

let rename = (name, (_oldName, content)) => (name, content);

let modify =
  (modify, (name, content)) => (name, modify(content));
