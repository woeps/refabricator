# Refabricator
A Static Site Generator written in and for ReasonML.

This project uses [Esy](https://github.com/esy-ocaml/esy) to compile ReasonML to native.

This project is **Work In Progress** and published to get early feedback from the community.

## Example
### md > beween > log
Get all markdown files of a directory, put each content into a template and write everything out as log messages.

```
open Lib.Refabricator;

let () =
  fromMd("pages")
  |> between((
       "<html><head><title>Site generated with Refabricator</title></head><body><main>\n",
       "\n"
       ++ {|</main><footer>This site was generated by <a href="https://github.com/woeps/Refabricator">Refabricator</a>!</footer></body></html>|},
     ))
  |> toLog;
```

### 2x md > between > log
Get all markdown files of one directory and put each content into a template + get all markdown files of another directory (without template) and write everything out as log messages.

```
open Lib.Refabricator;

let () =
  [
    fromMd("pages")
    |> between((
         "<html><head><title>Site generated with Refabricator</title></head><body><main>\n",
         "\n"
         ++ {|</main><footer>This site was generated by <a href="https://github.com/woeps/Refabricator">Refabricator</a>!</footer></body></html>|},
       )),
    fromMd("pages2"),
  ]
  |> from
  |> toLog;
```


## Usage

You need Esy, you can install the beta using [npm](https://npmjs.com):

    % npm install -g esy@latest

> NOTE: Make sure `esy --version` returns at least `0.5.4` for this project to build.

Then run the `esy` command from this project root to install and build dependencies.

    % esy

Now you can run your editor within the environment (which also includes merlin):

    % esy $EDITOR
    % esy vim

Alternatively you can try [vim-reasonml](https://github.com/jordwalke/vim-reasonml)
which loads esy project environments automatically.

After you make some changes to source code, you can re-run project's build
again with the same simple `esy` command.

    % esy

And test compiled executable (runs `scripts.tests` specified in
`package.json`):

    % esy test
