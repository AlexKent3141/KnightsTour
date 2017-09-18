#ifndef __TOUR_FINDER_H__
#define __TOUR_FINDER_H__

#include "KnightBoard.h"
#include "KnightTour.h"
#include <vector>

typedef void (*OnTourCallback)(KnightTour*);

// This class contains the tour finding algorithm.
// It finds all tours on a board of specified dimensions.
class TourFinder
{
public:
    TourFinder(int, OnTourCallback);
    ~TourFinder();

    int ToursFound() const;
    std::vector<int> ToursFoundAtPositions() const;

    void Start();
    void Stop();

private:
    int _boardSize;
    bool _stopSearching;
    int* _toursFound = nullptr;
    int** _moveStorage = nullptr;
    OnTourCallback _callback;

    void StoreSymmetries(int, int, int);

    // Find tours (top level call).
    // Takes a start row/col as arguments.
    int FindTours(int, int);

    // Recursively iterate through deeper levels.
    int FindSubTours(KnightBoard*, int);
};

#endif // __TOUR_FINDER_H__
