# Minesweeper and Matricies

## Introduction

Welcome to my code that demonstrates that you can solve all of the known parts of
Minesweeper by using Matricies and some special properties that you can derive from them.
Please note, in advance, that there will be certain situations where it is impossible to
tell for sure which squares are Mines and which are not. This solver will not make a move
if there are no sure moves that it can make; I do not attempt to do any probabalistic
analysis (yet).

If you want the explicit details on how it works then you should [read my blog post on the
subject][1].

## License

I have written all of this code myself and therefore it is mine to license as I wish. The
only part of the code that can be considered copied is the gaussian elimination algorithm
which I read on Wikipedia and then wrote myself.

Here is the short version: you can use the following code however you like, I will not
support it or be responsible for it but please give me attribution if you do use it. I
love attribution and all I really want is for people to go around saying:

> Hey, did you know that you can solve Minesweeper using Matricies? Yeah, this person called 
> Robert Massaioli came up with that and wrote this cool example program on the internet.

That is all I want. But for practical purposes you should consider this code to be
licensed under the MIT license.

## Compilation

I took careful effort to make sure that this code had no external dependencies so that it
would be possible to compile cross platform. All that you require to compile it is [CMake][2]. 
You can compile it as follows:

    mkdir localbuild && cd localbuild
    cmake ..
    make

And then you should be able to run the program by typing in:

    ./src/mnm

And that should run the solver. 

## Installation

What, really? You actually want to install this code on your machine, well I'm chuffed.
You can use the standard cmake tools to install it but, honestly, it will probably just
clutter up your bin directory.

### Code Caveats

There are a few things that you should know about the code:

 - It is pretty messy. I wrote it quickly and it could be separated better. I will attempt
   to make it nicer over time.
 - There are no dependencies and that has reduced nice modern C++ features: specifically
   using RAII principles to manage pointers. I should have written my own smart pointer
   class but I did not. Instead you will find delete's spread out over the code. Be happy
   that I have attempted to use valgrind to weed out the memory leaks.

* original repo: https://bitbucket.org/robertmassaioli/minesweeper-and-matricies.git
 [1]: http://robertmassaioli.wordpress.com/2013/01/12/solving-minesweeper-with-matricies/
 [2]: http://www.cmake.org/
