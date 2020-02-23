/** Common Types of the Refabricator Library
 *
 *  - Fabrics consist of a name and the actual content
 *  - a fabric is a function taking a unit and returning a name and the actual content
 *  - a fabricator is "generator" of fabrics
 *  - a refabricator manipulates the result of a fabric
 *  - a factory produces the SSG's output based on fabrics and their manipulations of refabricators
 *  - an appliedFactory is an already configure factory which takes fabrics and processes them
 */

type name = string;
/** Name of the result of a fabric */

type content = string;
/** Content of the result of a fabric */

type t = (name, content);
/** Result of a fabric */

type fabric = unit => t;
/** A fabric is a generator function for a source.
 *  The function can be called later on to actually fetch sources
 *  aka "lazyly loaded source"
 */

type fabrics = list(fabric);
/** Multiple fabrics
 *  aka "multiple lazyly loaded sources"
 */

type fabricator('a) = 'a => fabrics;
/** Constructor of multple fabrics.
 *  Each concrete type of fabricator has it's own set of arguments.
 *  aka "loader of fabrics"
 */

type refabricator('a) = ('a, fabrics) => fabrics;
/** Manipulates fabrics
 *  Each concrete type of refabricator has it's own set of arguments
 *  aka "rewriter of source's content"
 */

type factory('a) = ('a, fabrics) => result(unit, string);
/** Construct an `appliedFactory`, which will process fabrics (e.g. write them to file)
 *  aka "Processor of fetched & manipulated content"
 */

type appliedFactory = fabrics => result(unit, string);
/** Construct an `appliedFactory`, which will process fabrics (e.g. write them to file)
 *  aka "Processor of fetched & manipulated content"
 */

type pathDescriptor = {
  path: string,
  extension: option(string),
};
/** Specifies a path and an optional extension
  * to be used by fabricators & factories which interact with fs
  */;
