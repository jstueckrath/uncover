# Uncover

This is a mirror of Jan Stückrath’s [Uncover](http://www.ti.inf.uni-due.de/research/tools/uncover/) tool.

## Overview

_Uncover_ implements a backward search for graph transformation systems to solve the coverability problem.

This tool can be used to verify the correctness of a system (e.g. protocols, distributed systems, concurrent systems, etc.) modelled by graphs and graph transformation rules. Given a well-quasi-order, any error preserved by the order (i.e. if a graph contains the error, then any larger graph contains the error as well) can be represented by a finite set of minimal error graphs. This tool can check if an error graph is coverable by the graph transformation system and thus check if the error is reachable. The theoretical framework on which this tool is based, is explained in [this paper](http://www.ti.inf.uni-due.de/fileadmin/public/tools/uncover/backward-subgraph.pdf). Currently, the subgraph and the minor ordering are implemented.

The development of this tool is part of the [GaReV](http://www.ti.inf.uni-due.de/research/garev/) project (related [Publications](http://www.ti.inf.uni-due.de/publications/project-garev.shtml)).

## System requirements

For 64-bit Linux systems a binary including the necessary libraries (see below) is available in the download section. On other systems _Uncover_ may be compiled if the necessary libraries exist.

### Compiling Uncover

_Uncover_ is written in C++ using the C++11 standard. It currently compiles on Linux (tested on Ubuntu 14.04, Fedora 21) and Mac OS X with MacPorts (tested on Yosemite), but does not compile on Windows. _Uncover_ uses the following external libraries:

* [Boost](http://www.boost.org/) (version 1.54)
  - more precisely: boost_system, boost_filesystem, boost_program_options, boost_regex and boost_unit_test_framework
* [Xerces-C++](http://xerces.apache.org/xerces-c) (version 3.1)

Note that for compilation the standard and development packages of the above libraries are necessary. The source code archive contains a CMake script (requiring CMake 2.8+) for generating the makefiles, which is capable of compiling with gcc and clang. The source code should be compilable with newer versions of the above libraries and may be compilable with older versions, but this was not tested.

### Additional run requirements

The analysis procedures do not use other tools. However, _Uncover_ provides scenarios to draw graphs and graph transformation systems. These scenarios only work if [LaTeX](http://www.latex-project.org/) and [Graphviz](http://www.graphviz.org/) (version 2.36) are installed and in the operating systems search path for binaries.

## Usage

Currently, only the source code documentation exists. A more general description or manual will be published at a later time.

### Running Uncover

_Uncover_ is a command line tool and does not provide a graphical user interface. A detailed usage description can be printed via:

    uncover -h

A list of all scenarios is available via `uncover -l` and the usage of a specific scenario s can be printed via `uncover -u s`.

See the case studies archive for examples of using _Uncover_.

### Input and output

For loading and storing graphs and graph transformation systems, _Uncover_ uses the XML based standards GXL (for graphs) and GTXL (for transformation systems). DTDs for both standards are given in the resource folder in the source code archive. However, we modified the GTXL defintion to enable universally quantified rules. An adapted DTD (gtxl_mod.dtd) is also given in the resource folder.
