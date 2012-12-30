#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <vector>

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
   if(state == PROGRESS)
   {
      state = board.clickSquare(m);
   }
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
   generated = false;
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
         int position = locPos(col, row);
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
   if(!generated)
   {
      generateGrid(move);
   }

   GameState resultingState = PROGRESS;

   Position clickPosition = move.getPosition();
   if(isValidPos(clickPosition)) 
   {
      int position = locPos(move);

      switch(move.getClickType())
      {
         case NORMAL:
            squaresLeft -= openEmptySquares(clickPosition);

            if(grid[position].state == NOT_CLICKED)
            {
               grid[position].state = CLICKED;
               squaresLeft--;
            }

            // Check to see if you won or lost on this click
            if(grid[position].value == MINE)
            {
               // if you clicked a mine then you lost
               resultingState = LOST;
            }
            else if(squaresLeft == mines) 
            {
               // if you did not click a mine and only mines are left on the board then
               // you won!
               resultingState = WON;
            }
            break;

         case FLAG:
            break;

         case QUESTION:
            break;

         case EXPAND:
            break;

         case CLEAR:
            break;
      }
   } else {
      // TODO we should report some sort of error message or mistake here
   }

   return resultingState;
}

int Board::openEmptySquares(Position& position)
{
   int pos = locPos(position);
   int clicked = 0;

   if(isValidPos(position))
   {
      if(grid[pos].state != CLICKED)
      {
         grid[pos].state = CLICKED;
         clicked++;

         if(grid[pos].value == EMPTY)
         {
            for(int mapPos = 0; mapPos < 8; ++mapPos)
            {
               Position nextPos(position.getX() + map[mapPos][0], position.getY() + map[mapPos][1]);
               clicked += openEmptySquares(nextPos);
            }
         }
      }
   }

   return clicked;
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

int Board::locPos(Move& move)
{
   Position pos = move.getPosition();
   return locPos(pos);
}

int Board::locPos(Position& pos)
{
   return locPos(pos.getX(), pos.getY());
}

int Board::locPos(int col, int row)
{
   return row * dim.getWidth() + col;
}

bool Board::isValidPos(Position& pos)
{
   return isValidPos(pos.getX(), pos.getY());
}

bool Board::isValidPos(int col, int row)
{
   if((row < 0) || (row >= dim.getHeight())) return false;
   if((col < 0) || (col >= dim.getWidth())) return false;

   return true;
}

class IncGenerator
{
   public:
      IncGenerator(Dimensions dim) 
         : dimensions(dim), current(Position(0,0))
      {
      }

      IncGenerator(Position initial, Dimensions dim)
         : dimensions(dim), current(initial)
      {
      }

      Position operator()()
      {
         Position last = current;
         if(current.getX() + 1 == dimensions.getWidth()) 
         {
            current = Position(0, current.getY() + 1);
         }
         else
         {
            current = Position(current.getX() + 1, current.getY());
         }
         return last;
      }

   private:
      Position current;
      Dimensions dimensions;
};

// We can assume that the move is valid by this stage.
void Board::generateGrid(Move& move)
{
   int width = dim.getWidth();
   int height = dim.getHeight();
   int totalSquares = width * height;
   squaresLeft = totalSquares;
   assert(mines >= 0);
   assert(mines < totalSquares); // Cannot place more mines than there are squares.

   // Shuffle the positions so that we can pick the ones that we want to be the mines.
   vector<Position> squarePositions(totalSquares);
   IncGenerator gen(dim);
   generate(squarePositions.begin(), squarePositions.end(), gen);
   random_shuffle(squarePositions.begin(), squarePositions.end());

   // Generate the board
   grid = new Square[totalSquares];
   for(int i = 0; i < totalSquares; ++i)
   {
      grid[i].state = NOT_CLICKED;
      grid[i].value = EMPTY;
   }

   // Pick the Mines
   {
      int mineCount = 0;
      for(
            vector<Position>::iterator positionIter = squarePositions.begin();
            mineCount < mines && positionIter != squarePositions.end();
            ++positionIter)
      {
         if(!positionIter->isAdjacent(move.getPosition())) {
            grid[locPos(*positionIter)].value = MINE;
            ++mineCount;
         }
      }
   }

   // Calculate the numbers of the squares
   for(int row = 0; row < dim.getHeight(); ++row)
   {
      for(int col = 0; col < dim.getWidth(); ++col)
      {
         int currentSquare = locPos(col, row);

         if(grid[currentSquare].value != MINE)
         {
            int count = EMPTY;

            for(int mapCounter = 0; mapCounter < 8; ++mapCounter)
            {
               Position testPos(col + map[mapCounter][0], row + map[mapCounter][1]);
               if(isValidPos(testPos))
               {
                  count += (grid[locPos(testPos)].value == MINE);
               }
            }

            grid[currentSquare].value = count;
         }
      }
   }

   // Now finally state that the board has been generated
   generated = true;
}
