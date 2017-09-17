#include "KnightBoard.h"
#include <cstring>

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
            _board[r*_paddedN+c] = Empty;
        }
    }

    // Temporary board array for calculations.
    _temp = new int[paddedSize];

    // Set the starting location for the tour.
    _initialLoc = (2+startRow)*_paddedN + 2+startCol;
    _currentLoc = _initialLoc;
    _board[_currentLoc] = Visited;
}

KnightBoard::~KnightBoard()
{
    if (_board != nullptr) delete[] _board;
    if (_temp != nullptr) delete[] _temp;
    if (_directions != nullptr) delete[] _directions;
    if (_moves != nullptr) delete[] _moves;
    if (_latestTour != nullptr) delete _latestTour;

    _board = nullptr;
    _temp = nullptr;
    _directions = nullptr;
    _moves = nullptr;
    _latestTour = nullptr;
}

int KnightBoard::GetMoves(int* buf)
{
    int n = 0;
    const int MaxMoves = 8;
    for (int i = 0; i < MaxMoves; i++)
    {
        int move = _directions[i];
        int dest = _currentLoc + move;
        if (_board[dest] == Empty && RemainsSolvable(dest))
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
    _board[_currentLoc] = Visited;
    _moves[_numberOfMoves++] = move;

    // If this move completes a tour then store it.
    if (_numberOfMoves == _movesInTour) StoreTour();
}

void KnightBoard::UndoMove()
{
    int move = _moves[--_numberOfMoves];
    _board[_currentLoc] = Empty;
    _currentLoc -= move;
}

// Check that the board remains solvable i.e. the rest of the squares are connected, after the
// specified move.
bool KnightBoard::RemainsSolvable(int dest)
{
    const int RestOfBoardSize = TotalSquares() - SquaresVisited() - 1;

    // Duplicate the current occupancy state.
    memcpy(_temp, _board, _paddedN*_paddedN*sizeof(int));
    _temp[dest] = Visited;
    int numNeighbours = 0;

    // Pick one neighbour of the dest as a starting point.
    const int MaxMoves = 8;
    int n;
    for (int i = 0; i < MaxMoves; i++)
    {
        if (_board[dest + _directions[i]] == Empty)
        {
            n = dest + _directions[i];
            _temp[n] |= Neighbour;
            ++numNeighbours;
        }
    }

    if (numNeighbours == 0) return RestOfBoardSize == 0;

    // BFS from this neighbour and attempt to visit all neighbours.
    _todo.push(n);
    _temp[n] = Visited;
    int neighboursVisited = 1;
    while (neighboursVisited < numNeighbours && !_todo.empty())
    {
        int loc = _todo.front();
        _todo.pop();

        // Consider the neighbours of this location.
        int next;
        for (int i = 0; i < MaxMoves; i++)
        {
            next = loc + _directions[i];
            if (_temp[next] & Empty)
            {
                if (_temp[next] & Neighbour)
                {
                    ++neighboursVisited;
                }

                _temp[next] = Visited;
                _todo.push(next);
            }
        }
    }

    while (!_todo.empty()) _todo.pop();

    return neighboursVisited == numNeighbours;
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
