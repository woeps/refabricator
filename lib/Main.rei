/** Main Module of Refabricator
 *  A Static Site Generator for/in ReasonML.
 *
 *  Example:
 *  fromMd("pages")
 *  |> between((
 *       "<html><head><title>Site generated with Refabricator</title></head><body><main>\n",
 *       "\n"
 *       ++ {|</main><footer>This site was generated by <a href="https://github.com/woeps/Refabricator">Refabricator</a>!</footer></body></html>|},
 *     ))
 *  |> toLog;
 */

let from: list(Types.fabrics) => Types.fabrics;
/** Combines a list of several fabrics
 *
 *  Example:
 *  from([Fabricators.md("path1"), Fabricators.md("path2")]
 */

let fromMd: Types.fabricator(string);
/** Generate fabrics for all markdown files in the given path;
 *
 *  Example:
 *  fromMd("path1")
 */

let between: Types.refabricator((string, string));
/** Add text before and after the fabricated content
 * Example:
 * fromMd("path1") |> between(("text-before-content", "text-after-content"))
 */

let to_: (Types.appliedFactory, Types.fabrics) => unit;
/** Handle success/error of a factory
 *
 *  Example:
 *  to_(Factories.log())
 */

let toMany: (list(Types.appliedFactory), Types.fabrics) => unit;
/** Handle success/error of multiple factories
 *
 *  Example:
 *  toMany([Factories.log(), Factories.file("path")])
 */

let toLog: Types.fabrics => unit;
/** Write all fabrics to log and print out result
 *
 *  Example:
 *  fromMd("path") |> toLog
 */

let toFiles: (Types.pathDescriptor, Types.fabrics) => unit;
/** Write all fabrics to the filesystem
 */;
