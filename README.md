# barec
Turns C source to easy to process tokens.

Reads correct C/C++ source from standard input and writes space delimited C tokens to the standard output.
Turns:
#include <stdio.h>
int
main()
{
  printf("Hello world\n");
}

to:
int main ( ) { printf ( "Hello world\n" ) ; }

The program has one command line argument: keephash
to let the compiler process the output which is my simple way of testing that the output is still a valid source code.

Wanted to process my own C source files to extract and document/visualise certain usual patterns like state-machines.
Basically anything which is not available is existing tools like doxygen.

Also my "Hello world" github project to share this project amongs my computers I use: at work, living room, bedroom.
