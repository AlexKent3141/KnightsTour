// Knight's Tour: This program finds "Knight's Tours" on an NxN board. These are sequences of knight
// moves which land on every square exactly once.

#include "KnightBoard.h"
#include <iostream>
#include <string>
#include <thread>

bool stopSearching = false;

void FindSubTours(KnightBoard* board, int& numToursFound)
{
    const auto& moves = board->GetMoves();
    if (moves.size() == 0)
    {
        if (board->SquaresVisited() == board->TotalSquares())
        {
            ++numToursFound;
            board->PrintMoves();
        }
    }
    else
    {
        for (size_t i = 0; i < moves.size() && !stopSearching; i++)
        {
            const auto& move = moves[i];
            board->MakeMove(move);
            FindSubTours(board, numToursFound);
            board->UndoMove();
        }
    }
}

int FindTours(int N)
{
    KnightBoard board(N);
    int numToursFound = 0;
    const auto& moves = board.GetMoves();
    for (size_t i = 0; i < moves.size() && !stopSearching; i++)
    {
        const auto& move = moves[i];
        board.MakeMove(move);
        FindSubTours(&board, numToursFound);
        board.UndoMove();
    }

    std::cout << "Num tours found: " << numToursFound << std::endl;

    return numToursFound;
}

void PerfTest(int N)
{
    std::thread run(FindTours, N);
    run.detach();

    const int TestTimeMs = 10000;
    std::chrono::milliseconds delay(TestTimeMs);
    std::this_thread::sleep_for(delay);

    stopSearching = true;
}

// Arguments:
// N - the dimension of the NxN board.
// perf - execute performance test.
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        int N = stoi(std::string(argv[1]));
        bool perf = argc > 2 && std::string(argv[2]) == "perf";
        if (perf)
        {
            PerfTest(N);
        }
        else
        {
            FindTours(N);
        }
    }
    else
    {
        std::cout << "No arguments specified!" << std::endl;
    }

    return 0;
}
