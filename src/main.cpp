#include <iostream>
#include <cstdlib>

#include "game.h"
#include "solver.h"

using namespace std;

int main(int argc, char** argv)
{
   // Seed the random number generator
   unsigned int randomSeed = unsigned( time(NULL) );
   //unsigned int randomSeed = 1357209138;
   srand(randomSeed);
   cout << "Random Seed: " << randomSeed << endl;
   cout << endl;

   // Create the game
   Dimensions dim(30, 16);
   Game game(dim, 99);
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
      game.print();
   } while (game.getState() == PROGRESS && movesToPerform != NULL && !movesToPerform->empty());

   // Clear the final moves
   if(movesToPerform != NULL)
   {
      delete movesToPerform;
      movesToPerform = NULL;
   }

   cout << endl;
   game.print();
   cout << "Final game state: ";
   switch(game.getState())
   {
      case PROGRESS:
         cout << "Progress...unwinnable without probabilities";
         break;

      case WON:
         cout << "We won the game!";
         break;

      case LOST:
         cout << "ERROR: We hit a mine, this should never happen in a non probabalistic game.";
         break;
   }
   cout << endl;

   return EXIT_SUCCESS;
}
