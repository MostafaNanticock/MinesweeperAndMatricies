#include <iostream>
#include <cstdlib>
#include <cassert>

#include "game.h"

using namespace std;

/**
 * This map contains all of the potential relative positions.
 */
static int map[8][2] = {
   {-1,-1},
   {0,-1},
   {1,-1},
   {1, 0},
   {1, 1},
   {0, 1},
   {-1,1},
   {-1,0}
};

// Implementing the Game
Game::Game(Dimensions dim, int mineCount) : board(dim, mineCount)
{
   state = PROGRESS;  
}

Game::~Game()
{
}

void Game::acceptMove(Move& m)
{
}

void Game::print()
{
   // TODO maybe show more game information
   board.print();
}

//
// Implementing the Board
//
Board::Board(Dimensions dim, int mineCount)
   : dim(dim)
{
   grid = NULL;
   isGenerated = false;
   mines = mineCount;
}

Board::~Board()
{
   if(grid != NULL)
   {
      delete grid;
   }
}

void Board::print()
{
   for(int row = 0; row < dim.getHeight(); ++row) 
   {
      for(int col = 0; col < dim.getWidth(); ++col) 
      {
         int position = locPos(row, col);
         int gridValue;
         switch(grid[position].state)
         {
            case CLICKED:
               gridValue = grid[position].value;
               switch(gridValue)
               {
                  case MINE:
                     cout << "M";
                     break;

                  case EMPTY:
                     cout << " ";
                     break;

                  default:
                     cout << gridValue;
                     break;
               }
               break;

            case FLAG:
               cout << "F";
               break;
               
            case QUESTION:
               cout << "?";
               break;

            case NOT_CLICKED:
               cout << "#";
               break;

            default:
               cout << "E";
               break;
         }
      }
      
      cout << endl;
   }
}

GameState Board::clickSquare(Move& move)
{
}

Dimensions Board::getDimensions()
{
   return dim;
}

Square* Board::getGrid()
{
   // TODO is this function even required
   return grid;
}

// We can assume that the move is valid by this stage.
void Board::generateGrid(Move& move)
{
   int width = dim.getWidth();
   int height = dim.getHeight();
   int totalSquares = width * height;
   squaresLeft = totalSquares;
   assert(mines >= 0);
   assert(mines < totalSquares); // Cannot place more mines than there are squares.

   srand(time(NULL));
   int remainingMines = mines;
   int badMinePos = locPos(move);
   while(remainingMines > 0)
   {
      // TODO implement this as a list operation.
      int position = rand() % totalSquares; 

      if(grid[position].value) {}
   }
}
