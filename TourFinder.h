#ifndef __TOUR_FINDER_H__
#define __TOUR_FINDER_H__

#include "KnightBoard.h"
#include "KnightTour.h"

typedef void (*OnTourCallback)(KnightTour*);

// This class contains the tour finding algorithm.
// It finds all tours on a board of specified dimensions.
class TourFinder
{
public:
    TourFinder(int, OnTourCallback);
    ~TourFinder();

    inline int NumToursFound() const { return _numToursFound; }

    void Start();
    void Stop();

private:
    int _boardSize;
    int _numToursFound;
    bool _stopSearching;
    int** _moveStorage = nullptr;
    OnTourCallback _callback;

    // Find tours (top level call).
    void FindTours();

    // Recursively iterate through deeper levels.
    void FindSubTours(KnightBoard*, int);
};

#endif // __TOUR_FINDER_H__
