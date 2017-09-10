#ifndef __KNIGHT_BOARD_H__
#define __KNIGHT_BOARD_H__

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

    // Get the available moves in the position.
    int GetMoves(int*) const;

    // Make the specified knight move.
    void MakeMove(int);

    // Undo the last move.
    void UndoMove();

    void PrintMoves() const;

private:
    const int OffBoard = -1;

    int _n;
    int _paddedN; // Pad with off-board squares for easier edge detection.
    size_t _numberOfMoves = 0;
    size_t _currentLoc;

    int* _board = nullptr;
    int* _directions = nullptr;
    int* _moves = nullptr;

    std::string GetSquareString(int) const;
};

#endif // __KNIGHT_BOARD_H__
