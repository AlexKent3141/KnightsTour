// Knight's Tour: This program finds "Knight's Tours" on an NxN board. These are sequences of knight
// moves which land on every square exactly once.

#include "KnightBoard.h"
#include "TourFinder.h"
#include "TourRenderer.h"
#include "SDL/SDL.h"
#include <iostream>
#include <string>
#include <thread>

bool perf = false;
bool ui = false;
TourRenderer renderer;

// This method initialises the window.
void StartWindowMode(TourFinder* finder)
{
    // Initialise SDL.
    SDL_Init(SDL_INIT_VIDEO);

    // Set the title.
    SDL_WM_SetCaption("Knight's Tours", "Knight's Tours");

    // Create window.
    SDL_Surface* screen = SDL_SetVideoMode(640, 640, 0, 0);

    // Set the back colour.
    SDL_PixelFormat* fmt = screen->format;
    const int Blue = SDL_MapRGB(fmt, 0, 0, 255);
    SDL_FillRect(screen, NULL, Blue);

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
                    finder->Stop();
                    quit = true;
                    break;
            }
        }

        SDL_Flip(screen);

        // Show the latest tour.
        renderer.Render(screen);

        // Wait for a short period.
        const int TimeDelay = 100;
        SDL_Delay(TimeDelay);
    }

    SDL_FreeSurface(screen);
    SDL_Quit();
}

void OutputTour(KnightTour* tour)
{
    if (tour != nullptr && !perf)
    {
        if (ui)
        {
            // Display graphically.
            renderer.SetTour(*tour);
        }
        else
        {
            std::cout << tour->ToString() << std::endl;
        }
    }
}

// Stop the search after a pre-determined amount of time.
void PerfTest(TourFinder* finder)
{
    const int TestTimeMs = 10000;
    std::chrono::milliseconds delay(TestTimeMs);
    std::this_thread::sleep_for(delay);

    finder->Stop();
}

// Arguments:
// N - the dimension of the NxN board.
// perf - execute performance test.
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        int N = stoi(std::string(argv[1]));
        perf = argc > 2 && std::string(argv[2]) == "--perf";
        ui = argc > 2 && std::string(argv[2]) == "--ui";

        TourFinder* finder = new TourFinder(N, &OutputTour);

        if (perf || ui)
        {
            // Spawn a searching thread.
            std::thread run([&] { finder->Start(); });
            run.detach();

            if (perf)
            {
                PerfTest(finder);
            }
            else
            {
                // Show the window.
                StartWindowMode(finder);
            }

            auto numTours = finder->ToursFoundAtPositions();
            std::cout << "Num tours found: " << std::endl;
            for (int n : numTours) std::cout << n << std::endl;

            std::cout << "Total: " << finder->ToursFound() << std::endl;
        }
        else
        {
            finder->Start();
        }
    }
    else
    {
        std::cout << "No arguments specified!" << std::endl;
    }

    return 0;
}
