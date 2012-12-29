#include <iostream>
#include <cstdlib>

#include "game.h"

int main(int argc, char** argv)
{
   Dimensions dim(10, 10);
   Game game(dim, 5);

   return EXIT_SUCCESS;
}
