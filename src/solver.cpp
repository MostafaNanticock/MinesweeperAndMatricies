#include "game.h"
#include "solver.h"
#include "matrix.h"

#include <iostream>
#include <map>

using namespace std;

/**
 * TODO change this to be a function that is provided from Position.
 * This map contains all of the potential relative positions.
 */
static int adjMap[8][2] = {
   {-1,-1},
   {0,-1},
   {1,-1},
   {1, 0},
   {1, 1},
   {0, 1},
   {-1,1},
   {-1,0}
};

template<class A>
class optional
{
   private:
      A value;
      bool present;

   public:
      optional(A value)
         : value(value)
      {
         present = true;
      }

      optional()
         :
      {
         present = false;
      }

      bool isPresent()
      {
         return isPresent;
      }

      A get()
      {
         return value;
      }
};

list<Move>* solver::getMoves(Board* board)
{
   Square* grid = board->getGrid();

   // 1 List of non flagged numbers
   Dimensions gridDim = board->getDimensions();
   list<Position> nonFlaggedPositions;
   for(int row = 0; row < gridDim.getHeight(); ++row)
   {
      for(int col = 0; col < gridDim.getWidth(); ++col)
      {
         Square* currentSquare = grid + board->locPos(col, row);
         if(currentSquare->state == CLICKED)
         {
            if((currentSquare->value > EMPTY) && (currentSquare->value < MINE))
            {
               int flagCount = 0;
               for(int i = 0; i < 8; ++i)
               {
                  Position tempPos(col + adjMap[i][0], row + adjMap[i][1]);
                  if(board->isValidPos(tempPos))
                  {
                     int position  = board->locPos(tempPos);
                     if(grid[position].state == FLAG_CLICKED)
                     {
                        flagCount++;
                     }

                  }
               }

               // If the square is not completely flagged
               if(flagCount < currentSquare->value)
               {
                  nonFlaggedPositions.push_back(Position(col, row));
               }
            }
         }
      }
   }

   // 2 Get all of the adjacent squares that have not been clicked and identify them.
   int currentSquareId = 0;
   map<int, int> idToPosition;
   map<int, int> positionToId;
   for(
         list<Position>::const_iterator it = nonFlaggedPositions.begin();
         it != nonFlaggedPositions.end();
         ++it)
   {
      for(int i = 0; i < 8; ++i)
      {
         Position tempPos(it->getX() + adjMap[i][0], it->getY() + adjMap[i][1]);
         if(board->isValidPos(tempPos)) 
         {
            int position = board->locPos(tempPos);
            if(grid[position].state == NOT_CLICKED)
            {
               map<int, int>::iterator found = positionToId.find(position);
               if(found == positionToId.end())
               {
                  positionToId[position] = currentSquareId;
                  idToPosition[currentSquareId] = position;
                  currentSquareId++;
               }
            }
         }
      }
   }

   cout << "Non flagged positions: " << nonFlaggedPositions.size() << endl;
   cout << "Total Squares: " << currentSquareId << endl;

   // 3 Create a matrix based on the numbers that we have discovered. Base it off the
   // nonFlagged squares.
   int totalSquares = currentSquareId;
   Matrix solMat = createMatrix();
   for(
         list<Position>::const_iterator it = nonFlaggedPositions.begin();
         it != nonFlaggedPositions.end();
         ++it)
   {
      Vector currentRow = createVector();
      setDimVector(currentRow, totalSquares + 1);
      int position = board->locPos(it->getX(), it->getY());

      setValueVector(currentRow, totalSquares, grid[position].value);
      for(int i = 0; i < 8; ++i)
      {
         Position adjacent(it->getX() + adjMap[i][0], it->getY() + adjMap[i][1]);
         int adjacentPosition = board->locPos(adjacent);
         int matrixColumn = positionToId[adjacentPosition];
         setValueVector(currentRow, matrixColumn, 1);
      }

      addRowMatrix(solMat, currentRow);
   }

   // 4 Gaussian Eliminate the Matrix
   gaussianEliminate(solMat);

   // TEMP Print the matrix out
   int matrixWidth = getDimVector(getRowMatrix(solMat, 0));
   int matrixHeight = getHeightMatrix(solMat);
   for(int row = 0; row < matrixHeight; ++row)
   {
      Vector currentRow = getRowMatrix(solMat, row);
      for(int col = 0; col < matrixWidth; ++col)
      {
         cout << getValueVector(currentRow, col) << " ";
      }
      cout << endl;
   }
   
   // 5 Use the eliminated matrix and reduce to discover which squares must be mines and
   // which are unknown. Use those squares to generate a list of moves that you can
   // return.
   // Step 1: Find the first non zero row.
   int firstNonZeroRow = 0;
   for(int row = matrixHeight - 1; row >= 0; --row)
   {
      Vector currentRow = getRowMatrix(solMat, row);
      bool foundNonZero = false;
      for(int col = 0; col < matrixWidth && !foundNonZero; ++col)
      {
         foundNonZero |= getValueVector(currentRow, col) != 0;
      }
      if(foundNonZero)
      {
         firstNonZeroRow = row;
         break;
      }
   }

   Optional<bool>[] results = new Optional<bool>[matrixWidth - 1];

   int maxVariableColumn = matrixWidth - 1;
   for(int row = firstNonZeroRow; row >= 0; --row)
   {
      // If there is not a 1 in the current square then look right until you find one.
      // There cannot be values in a col that is < row because of the gaussian elimination
      for(int col = row + 1; col < maxVariableColumn; ++col)
      {
      }
   }

   delete[] results;

   return NULL;
}
