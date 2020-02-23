# Changelog - Refabricator
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Types of changes: `Added`, `Changed`, `Deprecated`, `Removed`, `Fixed`, `Security`

## [0.0.2 - UNRELEASED]
### Added
- CONTRIBUTING.md - moved some parts from README into this file
- Factory.file & Main.toFiles - support writing generated content to files - woeps/refabricator#1
- Types.pathDescriptor as a reusable type for in- & output - woeps/refabricator#20
- Fabricators.localPath: create fabrics based on files on the local filesystem - woeps/refabricator#6

### Changed
- README.md - moved some parts to CONTRIBUTING.md - commit woeps/refabricator@fb5aefa527f13d297c87bccae179193eb847fae2
- rename lib package to refabricator - woeps/refabricator#5
- remove public name from binary and update test script - woeps/refabricator#3
- rename module Refabricator to Main - woeps/refabricator#4
- refactored Fabricators.md to Refabricators.md and use Types.pathDescriptor - woeps/refabricator#6
- chaned Print.log to logAny and introduced new log(label, message) - woeps/refabricator#20

### Fixed
- remove public name for example binary Ssg - woeps/refabricator#3
- print errors to stderr - woeps/refabricator#2


## [0.0.1] - 2020-02-10
Initial rough version containing concept, types and the following parts:

### Fabricators
- md

### Refabricators
- between

### Factories
- log
