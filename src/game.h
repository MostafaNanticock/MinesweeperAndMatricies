#ifndef _MINESWEEPER_GAME
#define _MINESWEEPER_GAME

// In every board game square there should be a mine and they can be EMPTY, 1-8 or a MINE.
#define NA -1
#define EMPTY 0
#define MINE 9

enum GameState 
{
   PROGRESS, WON, LOST;
};

enum ClickType 
{
   NORMAL, FLAG, QUESTION, EXPAND, CLEAR;
};

enum SquareState
{
   NOT_CLICKED,
   FLAG,
   QUESTION,
   CLICKED;
};

/**
 * \brief This class represents dimensions.
 *
 * Please note that it is immutable.
 */
class Dimensions 
{
   public:
      Dimensions(int width, int height) 
      {
         this->width = width;
         this->height = height;
      }

      int getWidth() { return width; }
      int getHeight() { return height; }

   private:
      int width, height;
};

class Square
{
   public:
      int value;
      SquareState state;
};

class Board
{
   public:
      void print();

      GameState clickSquare(int row, int column, ClickType clickType);

      int getWidth();
      int getHeight();
      Square* getGrid();
      
   private:
      int width, height;
      int mines, squaresLeft;
};

// TODO it is possible that only Game and Move need to be in the final interface. Think more on
// this.

/**
 * \brief This class represents a move that the user can make in a game of minesweeper. 
 *
 * When the user wants to make a move then they can just use this to make it happen. This way we can
 * pass moves into the game wether they come from an AI or a computer. Please note that this class
 * should be immutable.
 */
class Move
{
   public: 
      Move(int row, int column, ClickType clickType) {
         this->row = row;
         this->column = column;
         this->clickType = clickType;
      }

      int getRow() { return row; }
      int getColumn() { return column; }
      ClickType getClickType() { return clickType; }
   private:
      int row, column;
      ClickType clickType;
};

class Game
{
   public:
      Game(Dimensions dim, int mineCount);
      ~Game();

      void acceptMove(Move& m);

      void print();
      void restart();

   private:
      void generateBoard(int rows, int cols);

      Board board; 
}

#endif
