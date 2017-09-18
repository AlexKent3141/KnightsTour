#include "TourFinder.h"
#include <cstring>

TourFinder::TourFinder(int boardSize, OnTourCallback callback)
    : _boardSize(boardSize), _callback(callback)
{
    const int BoardArea = _boardSize * _boardSize;

    // Allocate memory for moves at each depth.
    const int MaxMoves = 8;
    const int TourLength = BoardArea;

    _moveStorage = new int*[TourLength];
    for (int i = 0; i < TourLength; i++)
    {
        _moveStorage[i] = new int[MaxMoves];
    }

    // Allocate memory for the results.
    _toursFound = new int[BoardArea];
}

TourFinder::~TourFinder()
{
    if (_moveStorage != nullptr)
    {
        // Free the memory that was used for move storage.
        const int TourLength = _boardSize * _boardSize;
        for (int i = 0; i < TourLength; i++)
        {
            delete[] _moveStorage[i];
        }

        delete[] _moveStorage;
        _moveStorage = nullptr;
    }

    if (_toursFound != nullptr)
    {
        delete[] _toursFound;
        _toursFound = nullptr;
    }
}

int TourFinder::ToursFound() const
{
    const int BoardArea = _boardSize * _boardSize;
    int toursFound = 0;
    for (int i = 0; i < BoardArea; i++) toursFound += _toursFound[i];
    return toursFound;
}

std::vector<int> TourFinder::ToursFoundAtPositions() const
{
    const int BoardArea = _boardSize * _boardSize;
    std::vector<int> numTours;
    for (int i = 0; i < BoardArea; i++) numTours.push_back(_toursFound[i]);
    return numTours;
}

// Spawn a searching thread.
void TourFinder::Start()
{
    _stopSearching = false;

    const int BoardArea = _boardSize * _boardSize;
    memset(_toursFound, 0, BoardArea*sizeof(int));

    // Only need to look at a triangular region - the rest can be inferred by symmetry.
    const int RowsMax = _boardSize % 2 ? _boardSize/2 : _boardSize/2 - 1;
    for (int r = 0; r <= RowsMax; r++)
    {
        for (int c = 0; c <= r; c++)
        {
            int tours = FindTours(r, c);

            // Store this in all symmetric positions.
            StoreSymmetries(r, c, tours);
            StoreSymmetries(c, r, tours);
        }
    }
}

void TourFinder::Stop()
{
    _stopSearching = true;
}

void TourFinder::StoreSymmetries(int r, int c, int tours)
{
    int ir = _boardSize-r-1, ic = _boardSize-c-1;
    _toursFound[_boardSize*r + c] = tours;
    _toursFound[_boardSize*r + ic] = tours;
    _toursFound[_boardSize*ir + c] = tours;
    _toursFound[_boardSize*ir + ic] = tours;
}

int TourFinder::FindTours(int startRow, int startCol)
{
    KnightBoard board(_boardSize, startRow, startCol);

    int numTours = 0;
    int* buf = _moveStorage[0];
    int n = board.GetMoves(buf);
    for (int i = 0; i < n && !_stopSearching; i++)
    {
        const auto& move = buf[i];
        board.MakeMove(move);
        numTours += FindSubTours(&board, 1);
        board.UndoMove();
    }

    return numTours;
}

int TourFinder::FindSubTours(KnightBoard* board, int depth)
{
    int numTours = 0;
    int* buf = _moveStorage[depth];
    int n = board->GetMoves(buf);
    if (n == 0)
    {
        if (board->SquaresVisited() == board->TotalSquares())
        {
            ++numTours;
            _callback(board->LatestTour());
        }
    }
    else
    {
        for (int i = 0; i < n && !_stopSearching; i++)
        {
            const auto& move = buf[i];
            board->MakeMove(move);
            numTours += FindSubTours(board, depth+1);
            board->UndoMove();
        }
    }

    return numTours;
}
