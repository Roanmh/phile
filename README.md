# Systems Programming File Systems Project

A (very) simple file system implemented in native c.


# Requirements

 - File Allocation Table
 - Read, Write, Modify Operations
 - Write to disk
 - All files are pre-declared 'poems'
   (defined in poetry_map.cpp)
 - 'Map' function to show disk usage.


# Design

The project is written primarily in c, except for functions to make the
instructor-provided c++ available in c. The third-party library argtable3 is
used to create the CLI. A wrapping function is used to create the file handle
that the disk.c functions use. Finally, each command is carried out by its own
function, which calls disk.c functions for the heavy lifting.

Argparse allows a program to compare its arguments against templates for each of
its various functions. The program uses Argparse to compare the input to each
command template. By counting the number of errors produced from each parse, the
correct command is chosen (0 errors). If no command runs, then all errors are
explained to the user.

The project uses Make, g++, and gcc for compilation. It's meant to compile in a
unix environment. Both the c and c++ source files are compiled respectively then
linked. This relies on the `extern` modifiers on the linking functions. See both
poetry_map.cpp and main.c for these functions. The final output is a single
executable that relies on the standard libm.so library only.

The program maintains a relatively small memory footprint through its operations.
The largest memory consumption is during the poetry map initialization. This is
done by using native c file operations wherever possible. Instead of storing the
entire disk in memory, the program uses `lseek()`, `read()`, and `write()` to
transfer minimal amount of data at a time. In general, only a block (4096 bytes)
of the disk is held at time. The FAT and directory entries read and write small
in smaller increments. This is most visible in the disk.c functions; especially
in `append_poem()` and `first_avail_block()`. No profiling has been done on
this, and the program could likely be optimized to reduce file operations without
increase the memory use by much.

# Usage

## Build

Build with make (no arguments). The build requires g++, gcc, make, and libm.so.

```
make
```

Also, clean with `make clean`.

## Run

1. First, run `./phile format` to create and setup the disk file.

2. Then, `./phile map` can be used at any time to see the usage of the disk.
   The map displays an array of characters with the following key:

```
F - FAT table
D - Directory
* - Occupied block
_ - Free block
```

3. `./phile write <filename> <poemname>` will create a file on the disk with the
   contents of the given poem. Filenames can be any valid c-string with a max
   length of 26 characters. The available poem names are: (Poem names are
   case-sensitive, and strictly matched.)

```
"Jaberwocky"
"TheLama"
"Eldorado"
"SongOfTheOpenRoad"
```


4. `./phile read <filename>` will print the contents of the file to stdout.
5. `./phile append <filename> <poemname>` will append the contents of the given
   poem to an existing file. It will also create a new file if the file does
   not exist. Available poems are as above.


# Source Files

 - main.c: CLI parsing, disk-file handling, and command functions.
 - disk.c: Functions for reading and modifying the disk
 - argtable3.c: Third-party tool for CLI parsing
 - poetry_map.cpp: Mapping of poem names to content. Includes linking functions.
