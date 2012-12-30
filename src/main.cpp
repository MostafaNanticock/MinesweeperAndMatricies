#include <iostream>
#include <cstdlib>

#include "game.h"

using namespace std;

int main(int argc, char** argv)
{
   // Seed the random number generator
   unsigned int randomSeed = unsigned( time(NULL) );
   srand(randomSeed);
   cout << "Random Seed: " << randomSeed << endl;
   cout << endl;

   // Create the game
   Dimensions dim(30, 30);
   Game game(dim, 100);

   {
      Move move(Position(2, 2), NORMAL); 
      game.acceptMove(move);
   }
   {
      Move move(Position(20, 20), NORMAL); 
      game.acceptMove(move);
   }
   {
      Move move(Position(10, 2), NORMAL); 
      game.acceptMove(move);
   }

   game.print();

   return EXIT_SUCCESS;
}
