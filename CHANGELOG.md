# Changelog - Refabricator
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Types of changes: `Added`, `Changed`, `Deprecated`, `Removed`, `Fixed`, `Security`

## [0.0.2 - UNRELEASED]
### Added
- CONTRIBUTING.md - moved some parts from README into this file
- Factory.file & Main.toFiles - support writing generated content to files - #1
- Types.pathDescriptor as a reusable type for in- & output - #20
- Fabricators.localPath: create fabrics based on files on the local filesystem - #6
- use library for logging: [Logs](https://erratique.ch/software/logs) - #22

### Changed
- README.md - moved some parts to CONTRIBUTING.md - commit @fb5aefa527f13d297c87bccae179193eb847fae2
- rename lib package to refabricator - #5
- remove public name from binary and update test script - #3
- rename module Refabricator to Main - #4
- refactored Fabricators.md to Refabricators.md and use Types.pathDescriptor - #6
- changed Print.log to logAny and introduced new log(label, message) - #20
- rename binary to refabricate - #12

### Fixed
- remove public name for example binary Ssg - #3
- print errors to stderr - #2


## [0.0.1] - 2020-02-10
Initial rough version containing concept, types and the following parts:

### Fabricators
- md

### Refabricators
- between

### Factories
- log
