# warg
a C command line argument parser

## What Is
A super-basic C command line argument parser that can be "parachuted" directly into a project that needs arg parsing without pulling in any build- or run-time dependencies.

## How Do

`make` will build a statically-linked library for warg and run a suite of unit tests.

`make help` will spit out a list of other possibly helpful build targets.

Documentation is on my [TODO](TODO.md) list, but for now you might take a peek at [warg-test.c](warg-test.c) for some usage examples.

## Motivation
I love [popt](https://github.com/rpm-software-management/popt). It gets a lot of things right w/rt CLI argument parsing, is decidedly long-lived and battle-tested, has decent doc (and real-world examples abound), and is broadly available in that it is installed by default on a lot of platforms.

That said, I *hate* having to add a dependency to dev projects for something as simple/basic as parsing CLI option processing. While libpopt is generally installed (or is easily installable), its dev headers and other build-time dependencies generally are not.

popt is released under the MIT license so in theory I could just re-package it so that it could be dropped in alongside a project rather than rolling my own (rsync [did this](https://github.com/WayneD/rsync/tree/master/popt)). As it turns out, the effort to do that is non-trivial - in fact, so expensive to do repeatably that I decided to just "roll my own".
