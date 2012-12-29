#ifndef _MINESWEEPER_GAME
#define _MINESWEEPER_GAME

// In every board game square there should be a mine and they can be EMPTY, 1-8 or a MINE.
#define NA -1
#define EMPTY 0
#define MINE 9

enum GameState 
{
   PROGRESS, WON, LOST
};

enum ClickType 
{
   NORMAL, FLAG, QUESTION, EXPAND, CLEAR
};

enum SquareState
{
   NOT_CLICKED,
   FLAG_CLICKED,
   QUESTION_CLICKED,
   CLICKED
};

// The prototypes of the classes so that people know they exist and are waiting to happen.
class Position;
class Dimensions;
class Move;
class Square;
class Board;
class Game;

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

class Position
{
   public:
      Position(int x, int y)
      {
         this->x = x;
         this->y = y;
      }

      int getX() { return x; }
      int getY() { return y; }

   private:
      int x, y;
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
      Board(Dimensions dim, int mineCount);
      ~Board();

      void print();

      GameState clickSquare(Move& move);

      Dimensions getDimensions();
      Square* getGrid();
      
   private:
      void generateGrid(Move& move);
      int expandSquares(Position position);
      int locPos(Move& move);
      int locPos(int row, int col);

      Square* grid; 

      Dimensions dim;
      int mines, squaresLeft;
      bool isGenerated;
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
      Move(Position pos, ClickType clickType)
         : position(pos), clickType(clickType)
      {}

      Position getPosition() { return position; }
      ClickType getClickType() { return clickType; }

   private:
      Position position;
      ClickType clickType;
};

class Game
{
   public:
      Game(Dimensions dim, int mineCount);
      ~Game();

      void acceptMove(Move& m);
      void print();
   private:
      void generateBoard(int rows, int cols);

      Board board; 
      GameState state;
};

#endif
