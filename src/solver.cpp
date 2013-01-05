#include "game.h"
#include "solver.h"
#include "matrix.h"

#include "logging.h"
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
      {
         present = false;
      }

      bool isPresent()
      {
         return present;
      }

      A get()
      {
         return value;
      }
};

list<Move>* solver::getMoves(Board* board, logger* log)
{
   Square* grid = board->getGrid();

   // 1 List number squares that touch non-cliked squares
   Dimensions gridDim = board->getDimensions();
   list<Position> nonFlaggedPositions;
   for(int row = 0; row < gridDim.getHeight(); ++row)
   {
      for(int col = 0; col < gridDim.getWidth(); ++col)
      {
         Square* currentSquare = grid + board->locPos(col, row);
         if(currentSquare->state == CLICKED)
         {
            if((currentSquare->value > EMPTY) && (currentSquare->value < MINE)) // convert to helper function
            {
               bool unclickedFound = false;
               for(int i = 0; i < 8 && !unclickedFound; ++i)
               {
                  Position tempPos(col + adjMap[i][0], row + adjMap[i][1]);
                  if(board->isValidPos(tempPos))
                  {
                     int position  = board->locPos(tempPos);
                     if(grid[position].state == NOT_CLICKED)
                     {
                        unclickedFound = true;
                     }
                  }
               }

               // If the square is not completely flagged
               if(unclickedFound)
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

   (*log) << "Non flagged positions: " << nonFlaggedPositions.size() << logger::endl;
   (*log) << "Total Squares: " << currentSquareId << logger::endl;

   if(nonFlaggedPositions.size() == 0 || currentSquareId == 0)
   {
      // There cannot be any solution.
      return NULL;
   }

   // print out every element int the maps
   (*log) << "Id: Position" << logger::endl;
   for(
         map<int, int>::const_iterator iter = idToPosition.begin();
         iter != idToPosition.end();
         ++iter
      )
   {
      (*log) << iter->first << ": " << iter->second << logger::endl;
   }
   (*log) << logger::endl;

   // 3 Create a matrix based on the numbers that we have discovered. Base it off the
   // nonFlagged squares.
   int totalSquares = currentSquareId;
   matrix<double> solMat;
   Vector<double> tempRow;
   tempRow.setDimension(totalSquares + 1);
   for(
         list<Position>::const_iterator it = nonFlaggedPositions.begin();
         it != nonFlaggedPositions.end();
         ++it)
   {
      int position = board->locPos(it->getX(), it->getY());

      tempRow.reset(0.0);
      tempRow.setValue(totalSquares, grid[position].value);
      for(int i = 0; i < 8; ++i)
      {
         Position adjacent(it->getX() + adjMap[i][0], it->getY() + adjMap[i][1]);
         if(board->isValidPos(adjacent))
         {
            int adjacentPosition = board->locPos(adjacent);
            if(grid[adjacentPosition].state == NOT_CLICKED) 
            {
               int matrixColumn = positionToId[adjacentPosition];
               tempRow.setValue(matrixColumn, 1.0);
            } 
            else if (grid[adjacentPosition].state == FLAG_CLICKED) 
            {
               tempRow.setValue(totalSquares, tempRow.getValue(totalSquares) - 1);
            }
         }
      }

      solMat.addRow(&tempRow);
   }

   // 4 Gaussian Eliminate the Matrix
   solMat.render(log);
   solMat.gaussianEliminate();
   solMat.render(log);

   // 5 Use the eliminated matrix and reduce to discover which squares must be mines and
   // which are unknown. Use those squares to generate a list of moves that you can
   // return.
   // Step 1: Find the first non zero row.
   matrix<double>::width_size_type matrixWidth = solMat.getWidth();
   matrix<double>::height_size_type matrixHeight = solMat.getHeight();
   matrix<double>::height_size_type firstNonZeroRow = 0;
   // if the condition looks incorrect then look again. row is often unsigned on multiple platforms
   // so you cannot only say row >= 0 because that is always true you need to spot the integer 
   // overflow as well otherwise you have an infinite loop
   for(matrix<double>::height_size_type row = matrixHeight - 1; row >= 0 && row < matrixHeight; --row)
   {
      Vector<double>* currentRow = solMat.getRow(row);
      bool foundNonZero = false;
      for(Vector<double>::size_type col = 0; col < matrixWidth && !foundNonZero; ++col)
      {
         foundNonZero |= currentRow->getValue(col) != 0;
      }
      if(foundNonZero)
      {
         firstNonZeroRow = row;
         break;
      }
   }

   vector<optional<bool> > results;
   results.resize(matrixWidth - 1);

   matrix<double>::width_size_type maxVariableColumn = matrixWidth - 1;
   for(matrix<double>::height_size_type row = firstNonZeroRow; row >= 0 && row <= firstNonZeroRow; --row)
   {
      // If there is not a 1 in the current square then look right until you find one.
      // There cannot be values in a col that is < row because of the gaussian elimination

      // Place values on the other side.
      bool failedToFindValue = false;
      matrix<double>::height_size_type pivot = row;
      double pivotVal = solMat.getValue(row, pivot);
      double val = solMat.getValue(row, maxVariableColumn);
      for(matrix<double>::width_size_type col = row + 1; col < maxVariableColumn; ++col)
      {
         double currentValue = solMat.getValue(row, col);

         // Update the pivot if need be.
         if(pivotVal == 0.0 && currentValue != 0.0) 
         {
            pivot = col;
            pivotVal = currentValue;
            (*log) << "Pivot updated to: " << pivot << " => " << currentValue << logger::endl;
         }

         // Swap variables over to the other side.
         if(currentValue != 0.0)
         {
            if(results[col].isPresent())
            {
               val -= currentValue * (results[col].get() ? 1.0 : 0.0);
               solMat.setValue(row, col, 0.0);
            } 
            else
            {
               failedToFindValue = true;
            }
         }
      }
      (*log) << "value: " << val << " (" << (failedToFindValue ? "true" : "false") << ")" << logger::endl;
      solMat.setValue(row, maxVariableColumn, val);

      if(pivotVal != 0.0)
      {
         if(failedToFindValue) 
         {
            (*log) << "==" << logger::endl;
            // Otherwise Calculate min and max values for lemmas
            double minValue = 0;
            double maxValue = 0;

            for(matrix<double>::width_size_type col = row; col < maxVariableColumn; ++col)
            {
               double currentValue = solMat.getValue(row, col);
               if(currentValue > 0.0) maxValue += currentValue;
               if(currentValue < 0.0) minValue += currentValue; // plus a negative
            }

            if(val == minValue)
            {
               // every non zero item is actually zero
               for(matrix<double>::width_size_type col = row; col < maxVariableColumn; ++col)
               {
                  double currentValue = solMat.getValue(row, col);
                  if(currentValue > 0.0)
                  {
                     (*log) << "Col " << col << " is not a mine." << logger::endl;
                     results[col] = optional<bool>(false);
                  }
                  if(currentValue < 0.0)
                  {
                     results[col] = optional<bool>(true);
                  }
               }
            } 
            else if (val == maxValue)
            {
               // every non zero item is actually zero
               for(matrix<double>::width_size_type col = row; col < maxVariableColumn; ++col)
               {
                  double currentValue = solMat.getValue(row, col);
                  if(currentValue > 0.0)
                  {
                     (*log) << "Col " << col << " is a mine." << logger::endl;
                     results[col] = optional<bool>(true);
                  }
                  if(currentValue < 0.0)
                  {
                     results[col] = optional<bool>(false);
                  }
               }
            }
            // Apply elmmas to see if you can work it out using min and max properties.
         }
         else
         {
            // If there is only the pivot left the the row can be solved with normal methods
            if(results[pivot].isPresent())
            {
               (*log) << "Already found pivot for: " << pivot << logger::endl;
            } 
            else 
            {
               (*log) << "Found standard result: " << pivot << " => " << val << logger::endl;
               if(val == 0.0 || val == 1.0)
               {
                  results[pivot] = optional<bool>(val == 1.0);
               }
               else
               {
                  (*log) << "Found pivot value is not 0 or 1 ... what do we do?" << logger::endl;
               }
            }
         }
      }
      else
      {
         (*log) << "There in now pivot in row: " << row << logger::endl;
      }
   }

   // print out results
   list<Move>* moves = new list<Move>;
   for(matrix<double>::width_size_type i = 0; i < matrixWidth - 1; ++i)
   {
      (*log) << i << ": ";
      if(results[i].isPresent())
      {
         if(results[i].get())
         {
            (*log) << "mine";
            moves->push_back(Move(board->posLoc(idToPosition[(int) i]), FLAG));
         }
         else
         {
            (*log) << "not mine";
            moves->push_back(Move(board->posLoc(idToPosition[(int) i]), NORMAL));
         }
      }
      else
      {
         (*log) << "NA";
      }
      (*log) << logger::endl;
   }

   return moves;
}
