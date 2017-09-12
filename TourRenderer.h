#ifndef __TOUR_RENDERER_H__
#define __TOUR_RENDERER_H__

#include "KnightTour.h"
#include "SDL/SDL.h"
#include <mutex>
#include <utility>
#include <vector>

class TourRenderer
{
public:
    ~TourRenderer();
    void Render(SDL_Surface*);
    void SetTour(const KnightTour&);
    
private:
    KnightTour* _latestTour = nullptr;
    std::mutex _m;

    void CleanupTour();
    SDL_Rect* GetSquares(int, int) const;
    void DrawSquares(SDL_Surface*, SDL_Rect*) const;
    void DrawMoves(SDL_Surface*, SDL_Rect*) const;
    void DrawLine(SDL_Surface*, int, int, int, int, int) const;
};

#endif // __TOUR_RENDERER_H__
