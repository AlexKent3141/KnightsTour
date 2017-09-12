#include "TourRenderer.h"
#include <cmath>
#include <iostream>

TourRenderer::~TourRenderer()
{
    CleanupTour();
}

void TourRenderer::Render(SDL_Surface* target)
{
    std::lock_guard<std::mutex> lk(_m);

    if (_latestTour != nullptr)
    {
        SDL_Rect* squares = GetSquares(target->w, target->h);
        DrawSquares(target, squares);
        DrawMoves(target, squares);

        CleanupTour();
        delete[] squares;
    }
}

void TourRenderer::SetTour(const  KnightTour& tour)
{
    std::lock_guard<std::mutex> lk(_m);

    CleanupTour();
    _latestTour = new KnightTour(tour);
}

void TourRenderer::CleanupTour()
{
    if (_latestTour != nullptr)
    {
        delete _latestTour;
        _latestTour = nullptr;
    }
}

// Get the pixel locations of the squares given the width and height.
// The returned vector of locations goes from top-left to bottom-right across each row.
SDL_Rect* TourRenderer::GetSquares(int w, int h) const
{
    const double BorderSize = 0.1; // The width of the border as a proportion of the rect size.

    int minDim = std::min(w, h);
    int borderWidth = minDim * BorderSize;
    int n = _latestTour->BoardSize();
    uint16_t squareSize = (minDim - 2*borderWidth) / n;

    short rowLeft = borderWidth;
    short rowTop = borderWidth;
    SDL_Rect* squares = new SDL_Rect[n*n];
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            squares[n*r+c] = {rowLeft, rowTop, squareSize, squareSize};
            rowLeft += squareSize;
        }

        rowLeft = borderWidth;
        rowTop += squareSize;
    }

    return squares;
}

void TourRenderer::DrawSquares(SDL_Surface* target, SDL_Rect* squares) const
{
    // Define colours.
    SDL_PixelFormat* fmt = target->format;
    const int White = SDL_MapRGB(fmt, 255, 255, 255);
    const int Black = SDL_MapRGB(fmt, 0, 0, 0);

    int n = _latestTour->BoardSize();
    bool rowStartCol = n % 2;
    for (int r = 0; r < n; r++)
    {
        bool rowCol = rowStartCol;
        for (int c = 0; c < n; c++)
        {
            // Render this square.
            SDL_Rect sq = squares[n*r+c];
            int col = rowCol ? Black : White;
            SDL_FillRect(target, &sq, col);

            rowCol = !rowCol;
        }

        rowStartCol = !rowStartCol;
    }
}

void TourRenderer::DrawMoves(SDL_Surface* target, SDL_Rect* squares) const
{
    // Define colours.
    SDL_PixelFormat* fmt = target->format;
    const int Red = SDL_MapRGB(fmt, 255, 0, 0);

    int n = _latestTour->BoardSize();
    const int Half = squares[0].h/2;
    const auto& coords = _latestTour->Coords();
    size_t i = 0;
    do
    {
        auto& start = coords[i];
        auto& end = coords[i+1];

        // Draw lines between the centres of each rect.
        const SDL_Rect& r1 = squares[n*start.first + start.second];
        const SDL_Rect& r2 = squares[n*end.first + end.second];

        DrawLine(target, Red, r1.x+Half, r1.y+Half, r2.x+Half, r2.y+Half);

    } while (++i < coords.size()-1);
}

// There is no built in method for drawing lines so we do it manually.
void TourRenderer::DrawLine(SDL_Surface* target, int col, int x1, int y1, int x2, int y2) const
{
    int* pixels = (int*)target->pixels;

    double x = x2-x1;
    double y = y2-y1;
    double length = sqrt(x*x + y*y);

    double dx = x/length;
    double dy = y/length;

    x = x1;
    y = y1;

    int w = target->w;
    for (double i = 0; i < length; i++)
    {
        int ix = (int)x;
        int iy = (int)y;
        pixels[w*iy+ix] = col;
        x += dx;
        y += dy;
    }
}
