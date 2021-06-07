#include <iostream>
#include <cstdlib>

#include "game.h"
#include "solver.h"
#include "logging.h"

#include <fstream>
#include <ctime>

using namespace std;

class results
{
public:
    results()
    {
        winCount = loseCount = progressCount = total = 0;
    }

    void count(GameState state, unsigned int randomSeed)
    {
        total++;
        switch(state)
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
    }

    int winCount, loseCount, progressCount, total;
    list<unsigned int> losses;
};

static void printResults(logger* log, results& res);
static GameState solveRandomGame(Dimensions& dim, int mineCount, logger* log);

int main(int argc, char** argv)
{
    // Create the logger
    logger* log = nullptr;
    if(argc == 2)
    {
        fstream out_file(argv[1], fstream::out);
        log = new ostream_logger(out_file);
    }
    else
    {
        log = new nop_logger;
    }

    /*
        The windows version of minesweeper has the following difficulties.
        Beginner: 8 × 8 or 9 × 9 field with 10 mines
        Intermediate: 16 × 16 field with 40 mines
        Expert: 30 × 16 field with 99 mines
    */

    // Create the game
    Dimensions dim(9, 9);
    const int mines = 10;
    const int testRuns = 1;

    // Init the random numbers
    unsigned int initialRandom = unsigned( time(nullptr) );
    srand(initialRandom);
    (*log) << "Initial Random: " << initialRandom << logger::endl;

    results res;
    for(int i = 0; i < testRuns; ++i)
    {
        // Seed the random number generator
        unsigned int randomSeed = rand();
        srand(randomSeed);
        (*log) << "Random Seed: " << randomSeed << "\n\n" << logger::endl;

        // Solve a game and count the result
        GameState result = solveRandomGame(dim, mines, log);
        res.count(result, randomSeed);

        if(i % 500 == 0)
        {
            cout << "Processed " << i << std::endl;
        }
    }
    cout << "Processed " << testRuns << std::endl;

    {
        ostream_logger tempLogger(std::cout);

        printResults(&tempLogger, res);
    }
    (*log) << "\n" << logger::endl;
    printResults(log, res);

    delete log;
    return EXIT_SUCCESS;
}

static GameState solveRandomGame(Dimensions& dim, int mineCount, logger* log)
{
    Game game(dim, mineCount, log);
    solver turnSolver;

    // Make the initial move
    // TODO make this a random move
    {
        Move move(Position(rand() % dim.getWidth(), rand() % dim.getHeight()), NORMAL);
        game.acceptMove(move);
    }

    // Print out the game after the first move
    game.print();

    // Now get the AI to work out the rest
    list<Move>* movesToPerform = nullptr;
    do
    {
        if(movesToPerform != nullptr)
        {
            delete movesToPerform;
            movesToPerform = nullptr;
        }

        movesToPerform = turnSolver.getMoves(game.getBoard(), log);

        if(movesToPerform != nullptr)
        {
            for(list<Move>::const_iterator it = movesToPerform->begin(); it != movesToPerform->end(); ++it)
            {
                Move currentMove = *it;
                game.acceptMove(currentMove);
            }
        }
        game.print();
    } while (game.getState() == PROGRESS && movesToPerform != nullptr && !movesToPerform->empty());

    // Clear the final moves
    if(movesToPerform != nullptr)
    {
        delete movesToPerform;
        movesToPerform = nullptr;
    }

    (*log) << "\n" << logger::endl;
    game.print();
    (*log) << "\n" << logger::endl;

    return game.getState();
}

static void printResults(logger* log, results& res)
{
    double winPercentage = ((double) (res.winCount * 100)) / ((double) res.total);
    double progressPercentage = ((double) (res.progressCount * 100)) / ((double) res.total);
    (*log) << "WINs: " << res.winCount << " (" << winPercentage << "%)" << logger::endl;
    (*log) << "PROGRESSes " << res.progressCount << " (" << progressPercentage << "%)" << logger::endl;
    (*log) << "ERRORS (losses) " << res.loseCount << logger::endl;

    if(!res.losses.empty())
    {
        // Print out losses
        (*log) << "Seeds for losses: " << logger::endl;
        for(
            list<unsigned int>::const_iterator it = res.losses.begin();
            it != res.losses.end();
            ++it
            )
        {
            (*log) << " " << *it << logger::endl;
        }
    }
}
