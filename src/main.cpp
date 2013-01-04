#include <iostream>
#include <cstdlib>

#include "game.h"
#include "solver.h"

using namespace std;

static GameState solveRandomGame(Dimensions& dim, int mineCount);

int main(int argc, char** argv)
{
   // Create the game
   Dimensions dim(30, 16);

   int winCount = 0;
   int loseCount = 0;
   int progressCount = 0;

   list<unsigned int> losses;
   unsigned int initialRandom = unsigned( time(NULL) );
   srand(initialRandom);
   cout << "Initial Random: " << initialRandom << endl;
   //for(int i = 0; i < 1; ++i)
   for(int i = 0; i < 10000; ++i)
   {
      // Seed the random number generator
      unsigned int randomSeed = rand();
      //unsigned int randomSeed = 1388616052;
      srand(randomSeed);
      cout << "Random Seed: " << randomSeed << endl;
      cout << endl;

      GameState result = solveRandomGame(dim, 99);

      switch(result)
      {
         case PROGRESS:
            progressCount++;
            break;

         case WON:
            winCount++;
            break;

         case LOST:
            loseCount++;
            losses.push_back(randomSeed);
            break;
      }

      if(i % 5 == 0)
      {
         cout << "Processed " << i << endl;
      }
   }

   cout << "WINs: " << winCount << endl;
   cout << "PROGRESSes " << progressCount << endl;
   cout << "ERRORS (losses) " << loseCount << endl;
   cout << "Seeds for losses: " << endl;
   for(list<unsigned int>::const_iterator it = losses.begin();
         it != losses.end();
         ++it
      )
   {
      cout << " " << *it << endl;
   }

   /*
   cout << "Final game state: ";
   switch(result)
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
   */

   return EXIT_SUCCESS;
}

static GameState solveRandomGame(Dimensions& dim, int mineCount)
{
   Game game(dim, mineCount);
   solver turnSolver;

   // Make the initial move
   // TODO make this a random move
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

   return game.getState();
}
