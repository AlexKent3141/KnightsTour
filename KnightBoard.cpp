#include "KnightBoard.h"
#include <cstring>
#include <iostream>

KnightBoard::KnightBoard(int n) : _n(n)
{
    _movesInTour = _n*_n - 1;
    _paddedN = _n + 4;
    int paddedSize = _paddedN * _paddedN;

    // Setup the arrays.
    _moves = new int[n*n];

    // Define the knight's moves.
    _directions = new int[8] { 2*_paddedN-1, 2*_paddedN+1,
                               _paddedN+2, -_paddedN+2,
                               -2*_paddedN+1, -2*_paddedN-1,
                               _paddedN-2, -_paddedN-2 };

    _board = new int[paddedSize];
    memset(_board, OffBoard, paddedSize*sizeof(int));

    // The "middle" of the padded board should be zeroed.
    for (int r = 2; r < _paddedN-2; r++)
    {
        for (int c = 2; c < _paddedN-2; c++)
        {
            _board[r*_paddedN+c] = 0;
        }
    }

    // Set the starting location for the tour.
    _currentLoc = 2*_paddedN+2;
    _board[_currentLoc] = 1;
}

KnightBoard::~KnightBoard()
{
    if (_board != nullptr)
    {
        delete[] _board;
        _board = nullptr;
    }
    
    if (_directions != nullptr)
    {
        delete[] _directions;
        _directions = nullptr;
    }
    
    if (_moves != nullptr)
    {
        delete[] _moves;
        _moves = nullptr;
    }

    if (_latestTour != nullptr)
    {
        delete _latestTour;
        _latestTour = nullptr;
    }
}

int KnightBoard::GetMoves(int* buf) const
{
    int n = 0;
    const int MaxMoves = 8;
    for (int i = 0; i < MaxMoves; i++)
    {
        int move = _directions[i];
        if (_board[_currentLoc + move] == 0)
        {
            // The target is on the board and has not been visited before.
            buf[n++] = move;
        }
        
    }

    return n;
}

void KnightBoard::MakeMove(int move)
{
    _currentLoc += move;
    ++_board[_currentLoc];
    _moves[_numberOfMoves++] = move;

    // If this move completes a tour then store it.
    if (_numberOfMoves == _movesInTour) StoreTour();
}

void KnightBoard::UndoMove()
{
    int move = _moves[--_numberOfMoves];
    --_board[_currentLoc];
    _currentLoc -= move;
}

std::pair<int, int> KnightBoard::MakeCoord(int sq) const
{
    int row = sq % _paddedN - 1;
    int col = sq / _paddedN - 1;
    return std::make_pair(col, row);
}

void KnightBoard::StoreTour()
{
    std::vector<std::pair<int, int>> coords;

    int current = 2*_paddedN+2;
    coords.push_back(MakeCoord(current));

    for (size_t i = 0; i < _numberOfMoves; i++)
    {
        current += _moves[i];
        coords.push_back(MakeCoord(current));
    }

    _latestTour = new KnightTour(_n, coords);
}
