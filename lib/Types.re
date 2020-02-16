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

// type content('a) = 'a;
/** Content of the result of a fabric */

type t('content) = (name, 'content);
/** Result of a fabric */

type fabric('content) = unit => t('content);
/** A fabric is a generator function for a source.
 *  The function can be called later on to actually fetch sources
 *  aka "lazyly loaded source"
 */

type fabrics('content) = list(fabric('content));
/** Multiple fabrics
 *  aka "multiple lazyly loaded sources"
 */

type fabricator('options, 'content) = 'options => fabrics('content);
/** Constructor of multple fabrics.
 *  Each concrete type of fabricator has it's own set of arguments.
 *  aka "loader of fabrics"
 */

type refabricator('options, 'content, 'refabricatedContent) =
  ('options, fabrics('content)) => fabrics('refabricatedContent);
/** Manipulates fabrics
 *  Each concrete type of refabricator has it's own set of arguments
 *  aka "rewriter of source's content"
 */

type factory('options, 'content) =
  ('options, fabrics('content)) => result(unit, string);
/** Construct an `appliedFactory`, which will process fabrics (e.g. write them to file)
 *  aka "Processor of fetched & manipulated content"
 */

type appliedFactory('content) = fabrics('content) => result(unit, string);
/** Construct an `appliedFactory`, which will process fabrics (e.g. write them to file)
 *  aka "Processor of fetched & manipulated content"
 */;
