#include "KnightBoard.h"
#include <cstring>
#include <iostream>

// Construct a KnightBoard with given dimensions.
// The start of the tour is specified.
KnightBoard::KnightBoard(int n, int startRow, int startCol) : _n(n)
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

    _neighbours = new int[paddedSize];
    memset(_neighbours, 0, paddedSize*sizeof(int));

    InitialiseNeighbours();

    // Set the starting location for the tour.
    _initialLoc = (2+startRow)*_paddedN + 2+startCol;
    _currentLoc = _initialLoc;
    _board[_currentLoc] = 1;
}

KnightBoard::~KnightBoard()
{
    if (_board != nullptr) delete[] _board;
    if (_neighbours != nullptr) delete[] _neighbours;
    if (_directions != nullptr) delete[] _directions;
    if (_moves != nullptr) delete[] _moves;
    if (_latestTour != nullptr) delete _latestTour;

    _board = nullptr;
    _neighbours = nullptr;
    _directions = nullptr;
    _moves = nullptr;
    _latestTour = nullptr;
}

int KnightBoard::GetMoves(int* buf) const
{
    int n = 0;
    const int MaxMoves = 8;
    for (int i = 0; i < MaxMoves; i++)
    {
        int move = _directions[i];
        int dest = _currentLoc + move;
        if (_board[dest] == 0 && RemainsSolvable(_currentLoc, dest))
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

    // Decrease neighbours.
    const int MaxMoves = 8;
    for (int i = 0; i < MaxMoves; i++)
    {
        int n = _currentLoc + _directions[i];
        if (_board[n] != OffBoard)
        {
            --_neighbours[n];
        }
    }
}

void KnightBoard::UndoMove()
{
    // Increase neighbours.
    const int MaxMoves = 8;
    for (int i = 0; i < MaxMoves; i++)
    {
        int n = _currentLoc + _directions[i];
        if (_board[n] != OffBoard)
        {
            ++_neighbours[n];
        }
    }

    int move = _moves[--_numberOfMoves];
    --_board[_currentLoc];
    _currentLoc -= move;
}

// Check that the board still appears solvable after the specified destination.
// We do this by confirming that the neighbours are still reachable.
bool KnightBoard::RemainsSolvable(int source, int dest) const
{
    bool solvable = true;
    if (SquaresVisited() < TotalSquares()-2)
    {
        // Check the neighbours of this square.
        const int MaxMoves = 8;
        for (int i = 0; i < MaxMoves; i++)
        {
            int n = dest + _directions[i];
            if (_board[n] == 0 && _neighbours[n] <= 1)
            {
                solvable = false;
                break;
            }
        }
    }

    return solvable;
}

int KnightBoard::CountNeighbours(int loc) const
{
    int count = 0;
    const int MaxMoves = 8;
    for (int i = 0; i < MaxMoves; i++)
    {
        int move = _directions[i];
        if (_board[loc + move] != OffBoard) ++count;
    }

    return count;
}

void KnightBoard::InitialiseNeighbours()
{
    for (int r = 2; r < _paddedN-2; r++)
    {
        for (int c = 2; c < _paddedN-2; c++)
        {
            int loc = r*_paddedN+c;
            _neighbours[loc] = CountNeighbours(loc);
        }
    }
}

std::pair<int, int> KnightBoard::MakeCoord(int sq) const
{
    int row = sq % _paddedN - 2;
    int col = sq / _paddedN - 2;
    return std::make_pair(col, row);
}

void KnightBoard::StoreTour()
{
    if (_latestTour != nullptr) delete _latestTour;

    std::vector<std::pair<int, int>> coords;

    int current = _initialLoc;
    coords.push_back(MakeCoord(current));

    for (size_t i = 0; i < _numberOfMoves; i++)
    {
        current += _moves[i];
        coords.push_back(MakeCoord(current));
    }

    _latestTour = new KnightTour(_n, coords);
}
