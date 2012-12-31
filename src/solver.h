#ifndef __ROBERTM_MINESWEEPER_SOLVER
#define __ROBERTM_MINESWEEPER_SOLVER

#include <list>

class solver
{
   public:
      std::list<Move>* getMoves(Board* board);
};

#endif
