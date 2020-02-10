type name = string;
type content = string;
type t = (name, content);
type fabric = unit => t;
type fabrics = list(fabric);
type fabricator('a) = 'a => fabrics;
type refabricator('a) = ('a, fabrics) => fabrics;
type factory('a) = ('a, fabrics) => result(unit, string);
type appliedFactory = fabrics => result(unit, string);
