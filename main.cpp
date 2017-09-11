// Knight's Tour: This program finds "Knight's Tours" on an NxN board. These are sequences of knight
// moves which land on every square exactly once.

#include "KnightBoard.h"
#include "SDL/SDL.h"
#include <iostream>
#include <string>
#include <thread>

bool stopSearching = false;
int** storage = nullptr;
SDL_Surface* screen = nullptr;

// This method initialises the window.
void StartWindowMode()
{
    // Initialise SDL.
    SDL_Init(SDL_INIT_VIDEO);

    // Set the title.
    SDL_WM_SetCaption("Knight's Tours", "Knight's Tours");

    // Create window.
    screen = SDL_SetVideoMode(640, 480, 0, 0);

    // Handle input from the window.
    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    stopSearching = true;
                    quit = true;
                    break;
            }
        }

        SDL_Flip(screen);

        // Show the latest tour.

        // Wait for a short period.
        const int TimeDelay = 100;
        SDL_Delay(TimeDelay);
    }
}

void CleanupWindow()
{
    if (screen != nullptr)
    {
        SDL_FreeSurface(screen);
        SDL_Quit();
    }
}

void FindSubTours(KnightBoard* board, int& numToursFound, int level = 1)
{
    int* buf = storage[level];
    int n = board->GetMoves(buf);
    if (n == 0)
    {
        if (board->SquaresVisited() == board->TotalSquares())
        {
            ++numToursFound;
            auto tour = board->LatestTour();
            if (tour != nullptr)
            {
                std::cout << tour->ToString() << std::endl;
            }
        }
    }
    else
    {
        for (int i = 0; i < n && !stopSearching; i++)
        {
            const auto& move = buf[i];
            board->MakeMove(move);
            FindSubTours(board, numToursFound, level+1);
            board->UndoMove();
        }
    }
}

int FindTours(int N)
{
    KnightBoard board(N);
    int numToursFound = 0;
    int* buf = storage[0];
    int n = board.GetMoves(buf);
    for (int i = 0; i < n && !stopSearching; i++)
    {
        const auto& move = buf[i];
        board.MakeMove(move);
        FindSubTours(&board, numToursFound);
        board.UndoMove();
    }

    std::cout << "Num tours found: " << numToursFound << std::endl;

    return numToursFound;
}

// Stop the search after a pre-determined amount of time.
void PerfTest(int N)
{
    const int TestTimeMs = 10000;
    std::chrono::milliseconds delay(TestTimeMs);
    std::this_thread::sleep_for(delay);

    stopSearching = true;
}

void AllocateStorage(int N)
{
    const int MaxMoves = 8;
    const int TourLength = N*N;

    storage = new int*[TourLength];
    for (int i = 0; i < TourLength; i++)
    {
        storage[i] = new int[MaxMoves];
    }
}

void DeallocateStorage(int N)
{
    const int TourLength = N*N;
    for (int i = 0; i < TourLength; i++)
    {
        delete[] storage[i];
    }

    delete[] storage;
}

// Arguments:
// N - the dimension of the NxN board.
// perf - execute performance test.
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        int N = stoi(std::string(argv[1]));
        bool perf = argc > 2 && std::string(argv[2]) == "--perf";
        bool ui = argc > 2 && std::string(argv[2]) == "--ui";

        // Allocate sufficient memory.
        AllocateStorage(N);

        if (perf || ui)
        {
            // Spawn a searching thread.
            std::thread run(FindTours, N);
            run.detach();

            if (perf)
            {
                PerfTest(N);
            }
            else
            {
                // Show the window.
                StartWindowMode();
            }
        }
        else
        {
            FindTours(N);
        }

        DeallocateStorage(N);
    }
    else
    {
        std::cout << "No arguments specified!" << std::endl;
    }

    return 0;
}
