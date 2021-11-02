# C2EO <img src="https://www.yegor256.com/images/books/elegant-objects/cactus.svg" height="32px" /> [![build-project](https://github.com/polystat/c2eo/actions/workflows/build.yml/badge.svg)](https://github.com/polystat/c2eo/actions/workflows/build.yml) [![Lines of code](https://tokei.rs/b1/github/polystat/c2eo)](https://tokei.rs/b1/github/polystat/c2eo) [![Hits-of-Code](https://hitsofcode.com/github/polystat/c2eo?branch=master)](https://hitsofcode.com/github/polystat/c2eo/view?branch=master)

This is a translator of C/C++ to [EOLANG](https://www.eolang.org).
Semantic-preserving translation of C programs to EOLANG programs. *Other languages: [Russian](readme.ru.md)*

# Getting started guide

Should work. If it doesn't, [submit an issue](https://github.com/polystat/c2eo/issues),
we will fix it.

## Step 1. OS and tools
You need a [Linux](https://www.linux.org/pages/download/) operating system (we recommend to use [Ubuntu 20.+ ver.](https://ubuntu.com/download))

Tools:
[wget](https://www.tecmint.com/install-wget-in-linux/), 
[tar](https://www.tecmint.com/install-tar-in-centos-rhel-and-fedora/), 
[git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git), 
[cmake](https://cmake.org/download/), 
[gcc](http://mirror.linux-ia64.org/gnu/gcc/releases/), 
[g++](https://pkgs.org/download/g++), [ninja-build](https://ninja-build.org/), [python3.+](https://www.python.org/downloads/)

```bash
# Installation tools for Ubuntu
$ sudo apt install wget tar git cmake gcc g++ ninja-build python3
```

## Step 2. Install LLVM + CLANG

[Link to the archive](https://mega.nz/file/cZ9WQCqB#z713CuC-GNFQAXIxZwZxI05zOH4FAOpwYHEElgOZflA)

```bash
# After downloading
$ tar -xvf llvmorg-12.0.1.tar.gz
```

<details>
<summary> Old variant </summary>

<h2>Step 2. Install LLVM + CLANG</h2>
<pre><code>
$ wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-12.0.1.tar.gz
$ tar -xvf llvmorg-12.0.1.tar.gz
</code></pre>

<h2>Step 2.1 Build LLVM + CLANG</h2>
<pre><code>$ mv ./llvm-project-llvmorg-12.0.1 ./llvm-clang
$ cd llvm-clang
$ mkdir build && cd $_
</code></pre>
<pre><code>$ cmake --no-warn-unused-cli -DBUILD_SHARED_LIBS:STRING=ON -DLLVM_TARGETS_TO_BUILD:STRING=X86 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE "-DLLVM_ENABLE_PROJECTS:STRING=clang;compiler-rt" -DCMAKE_BUILD_TYPE:STRING=Debug -DLLVM_OPTIMIZED_TABLEGEN:STRING=ON -DLLVM_USE_SPLIT_DWARF:STRING=ON -DLLVM_USE_LINKER:STRING=gold ../llvm -G Ninja
</code></pre>
<pre><code>$ cmake --build . --config Debug --target all -j 10 -- -j1 -l 2
$ cd ../..
</code></pre>
<code><pre>$ tar -xvf llvmorg-12.0.1.tar.gz
</code></pre>
</details>

## Step 3. Install C2EO
```bash
$ git clone https://github.com/polystat/c2eo.git
```

## Step 4. Build C2EO

> IMPORTANT. Every time the transpiler code changes, you need to repeat this step

The c2eo transpiler configuration is related to its compilation from source. By placing llvm and clang in `~/llvm-clang`, the `CMakeLists.txt` files used in the `project` and `project/src/transpiler` directories remain unchanged.
Therefore, to perform compilation, you need to enter `project/build` and execute the command `cmake ..` in it.
After that, in the same directory, run the project build with the command `make` or` cmake --build. `

## Step 5. Run transpilation
> Use `--` at the end of command below to skip all errors:
```bash
$ cd ./project/bin
$ python3 launcher.py <file-of-c-program>
# or $ ./launcher.py <file-of-c-program>
```

<details>
    <summary>More information here...</summary>

&nbsp;
## Executable transpiler file and scripts for building the program on EO

---
## Directory Hierarchy

These files are located in the `project / bin` directory

&nbsp;
## Run scripts

### Launching the program builder on EO

`python3 launcher.py <file-of-c-program>`

The `c2eo` transpiler is launched, then the `collector.py` script is launched. As a result of processing the source file in the C language, the file `global.eo` on EO is generated, which is transferred to the directory `result / eo / c2eo / src /` of the project on EO.

&nbsp;
## Support scripts and programs
---
### Transpilation and formation of intermediate files

`c2eo <file-of-c-program> <name>`

Executable native code launched from the launcher. Carries out transpilation of a C program with the formation of intermediate files containing separately external (global) and static artifacts with the extensions `*.glob` and `*.stat`, respectively. The files are created in the `project / assembly` directory. The transmitted name is generated by the launcher and sets the names for intermediate files. It can also run autonomously.

### Generating the global.eo file

`collector`

Based on intermediate files located in the `project / assembly` directory, it also creates a program file on EO `global.eo`. Launched from the launcher.

---
</details>

## Step 6. Run eo generated files

Now the generated files in this [dir](result/eo/c2eo/src/)

[There](https://github.com/cqfn/eo) github page of EO project when you can learn about EO language

This [quick start](https://github.com/cqfn/eo#quick-start) for running your first eo project

## Additional step. Tests examples
### [Tests_1](./project/tests/scripts), [Tests_2](./project/tests/datasets/launcher-set/), [Tests_3](./project/tests/any)

&nbsp;
# Project structure

    .
    ├── collection
    │   ├── c-sources
    │   └── eo-sources
    ├── doc 
    ├── llvm-clang 
    ├── project 
    │   ├── assembly
    │   ├── bin
    │   ├── build
    │   ├── lib
    │   ├── src
    │   │   ├── transpiler
    │   │   ├── collector
    │   │   └── launcher
    │   ├── tests
    │   └── CMakeLists.txt
    ├── result
    │   ├── pom.xml
    │   ├── README.md
    │   ├── run.sh
    │   └── eo
    │       └── c2eo
    │           ├── run.sh
    │           └── eo  
    └── tmp 

* ### collection
  The `collection` directory contains source codes for programs in the C and EO programming languages, which are supposed to be used both for integration testing of the transpiler and for testing possible options for transforming into EO. C programs are located in the `c-sources` subdirectory. They form data sets that allow assessing the performance of the transpiler being developed. The subdirectory `eo-sources` contains programs on EO, which are used to analyze various variants of code generation, as well as to analyze the possibility of transforming programs from C to EO.

* ### doc
  The `doc` directory contains the documentation generated during the work on the project.


* ### llvm-clang
  The `llvm-clang` directory is for storing the build of the llvm project. It is assumed that this assembly will be formed at the level of binary codes for a specific autonomous implementation (docker, VM) and will not change subsequently. It is unlikely that during the development of the project it is worth switching to a more recent version of llvm without special reason. Being within the project will allow it to be distributed along with the result of the work. At the same time, you can look at and throw away all unnecessary things that are not needed for the project, thereby reducing 8 gigabytes to a more acceptable value.

* ### project
  The `project` directory contains everything that is the result of development. The following directories are currently being viewed in it:

  * #### assembly
    A directory intended for storing intermediate results, as well as the final result of the transpiler operation. The final file `global.eo` is formed from individual intermediate files in it, which is then copied (sent) to the corresponding subdirectory of the` result` directory.

  * #### bin
    The directory in which all executable files and scripts that provide the transpiler operation are grouped.

  * #### build
    The directory for building the project. It is supposed to build the project using `cmake`. In this regard, the hierarchical organization of files is used in each of the subprojects that provide the execution of individual functions, if necessary, a separate` CMakeLists.txt` file should be placed. Also, the root file `CMakeLists.txt` is located in the` project` directory.

  * #### lib
    A directory intended for storing static and (or) dynamic libraries generated during project creation, if any.

  * #### src
    The src directory is key for development. At this stage, two main projects are viewed, placed in the respective catalogs. In general, its content is likely to change.

    * ##### transpiler
      A transpiler that parses the AST of a single compilation unit and produces EO objects as output. These objects are located in the assembly directory and split over two files. One collects all objects corresponding to global artifacts, and the other - static. Taking into account the specifics of the AST analysis, this project is implemented in C ++.

    * ##### collector
      The directory in which the `global.eo` file collector is developed. In general, after the transpiler has processed all compilation units, it links from multiple files containing both static and global objects into a single object file on EO. The development of this program can be carried out in the scripting language.

    * ##### launcher
      A directory containing a program that runs the transpiler several times according to the number of compilation units, and then transfers control to the collector of the resulting individual files into the monolith. After completing the build, this program transfers the generated file `global.eo` to the directory` result / eo / c2eo / src`.

  * #### [tests](./project/tests/scripts)
    A directory with various test programs and data that check the functionality of the code being developed.

* ### result
  Directory for storing data used by the EO compiler. It contains information about the project on EO, updated every time the project of this compiler is updated.

  * #### README.md
    Description of the compiled project, which is formed by the c2eo developers and practically does not change (it can only be corrected);

  * #### run.sh
    A script that launches the compiled application;

  * #### eo
    A directory containing the libraries used, written in EO to support C artifacts, as well as EO sources generated by the transpiler or generated by hand. Within the `eo` directory, the structure is formed by subdirectories. Directly under `eo` is the` c2eo` directory, which defines the common name of the package. It contains:

    * ##### app.eo
      Responsible for launching the application (it is written manually and does not change);

    * ##### ctypes
      A directory that essentially defines some library of objects written in EO and designed to simulate various artifacts of the C language;

    * ##### src
      The directory where the file `global.eo` is written with objects generated by the transpiler during the analysis of a C program (it contains a `global` object, in which all artifacts are collected in the form of corresponding objects).

The formation of the `global.eo` file is essentially the main task of the transpiler and provides, by assembling many units, the compilation of the source C program.

&nbsp;
## Placing a program on EO obtained during transpilation
The presented structure became possible due to the use of initial initialization of objects that mimic C variables.

Transpilation is carried out separately for each compilation unit, which generates two files at the output:

* a file with a description of all global objects, which include abstract objects obtained by transforming abstract data types, global variables, global function descriptions;

* a file with a description of all static objects that are transformed from the descriptions of static variables and functions located in the global space, static variables located inside functions.

These two files are a basic stub for further build after transpilation of all compilation units of the project. The assembly itself at the moment consists in the formation of a common file in the EO programming language. It forms a global object `global`, which contains all objects obtained as a result of compilation of abstract data types, external variables, external functions, as well as objects that are obtained from files describing static objects.

The number of static objects is determined by the number of files with static artifacts. Placing all data in a single object `global` allows you to easily provide access both from global objects to their static data and from external static objects to global data. The compiler of this file can, in principle, be a separate program implemented in any convenient programming language.

If there is a `main` function in one of the compilation units, it is converted to the corresponding object of the global space. And immediately after its description, a description of its launch follows. The function can be located anywhere in the global object.

In general, the order in which a file with global objects and static objects is assembled is irrelevant.

The presented scheme provides complete autonomy for the formation of the program on EO. The object that launches the application contains only the dateization of the global object. It does not change, remaining constant regardless of the transpiled project.
