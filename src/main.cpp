#include <iostream>
#include <cstdlib>

#include "game.h"
#include "solver.h"

using namespace std;

int main(int argc, char** argv)
{
   // Seed the random number generator
   unsigned int randomSeed = unsigned( time(NULL) );
   srand(randomSeed);
   cout << "Random Seed: " << randomSeed << endl;
   cout << endl;

   // Create the game
   Dimensions dim(10, 10);
   Game game(dim, 10);
   solver turnSolver;

   // Make the initial move
   {
      Move move(Position(dim.getWidth() / 2, dim.getHeight() / 2), NORMAL); 
      game.acceptMove(move);
   }

   // Print out the game after the first move
   game.print();

   // Now get the AI to work out the rest
   list<Move>* movesToPerform = NULL;
   do
   {
      if(movesToPerform != NULL)
      {
         delete movesToPerform;
         movesToPerform = NULL;
      }

      movesToPerform = turnSolver.getMoves(game.getBoard());

      if(movesToPerform != NULL)
      {
         for(
               list<Move>::const_iterator it = movesToPerform->begin();
               it != movesToPerform->end();
               ++it
            )
         {
            Move currentMove = *it;
            game.acceptMove(currentMove);
         }
      }
   } while (movesToPerform != NULL && !movesToPerform->empty());

   // Clear the final moves
   if(movesToPerform != NULL)
   {
      delete movesToPerform;
      movesToPerform = NULL;
   }

   return EXIT_SUCCESS;
}
