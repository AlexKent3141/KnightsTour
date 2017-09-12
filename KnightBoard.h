#ifndef __KNIGHT_BOARD_H__
#define __KNIGHT_BOARD_H__

#include "KnightTour.h"
#include <cstdlib>
#include <vector>
#include <string>

// This class stores the current board state and allows for backtracking.
class KnightBoard
{
public:
    KnightBoard() = delete;
    KnightBoard(int);
    ~KnightBoard();

    inline size_t TotalSquares() const { return _n*_n; }
    inline size_t SquaresVisited() const { return _numberOfMoves + 1; }
    inline KnightTour* LatestTour() { return _latestTour; }

    // Get the available moves in the position.
    int GetMoves(int*) const;

    // Make the specified knight move.
    void MakeMove(int);

    // Undo the last move.
    void UndoMove();

private:
    const int OffBoard = -1;

    int _n;
    int _paddedN; // Pad with off-board squares for easier edge detection.
    size_t _numberOfMoves = 0;
    size_t _movesInTour;
    size_t _currentLoc;

    int* _board = nullptr;
    int* _neighbours = nullptr;
    int* _directions = nullptr;
    int* _moves = nullptr;

    KnightTour* _latestTour = nullptr;

    bool RemainsSolvable(int, int) const;

    int CountNeighbours(int) const;

    void InitialiseNeighbours();

    std::pair<int, int> MakeCoord(int) const;

    void StoreTour();
};

#endif // __KNIGHT_BOARD_H__
