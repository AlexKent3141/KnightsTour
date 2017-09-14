#include "TourFinder.h"

TourFinder::TourFinder(int boardSize, OnTourCallback callback)
    : _boardSize(boardSize), _callback(callback)
{
    // Allocate memory for moves at each depth.
    const int MaxMoves = 8;
    const int TourLength = _boardSize * _boardSize;

    _moveStorage = new int*[TourLength];
    for (int i = 0; i < TourLength; i++)
    {
        _moveStorage[i] = new int[MaxMoves];
    }
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
}

// Spawn a searching thread.
void TourFinder::Start()
{
    _numToursFound = 0;
    _stopSearching = false;
    FindTours();
}

void TourFinder::Stop()
{
    _stopSearching = true;
}

void TourFinder::FindTours()
{
    KnightBoard board(_boardSize);
    int* buf = _moveStorage[0];
    int n = board.GetMoves(buf);
    for (int i = 0; i < n && !_stopSearching; i++)
    {
        const auto& move = buf[i];
        board.MakeMove(move);
        FindSubTours(&board, 1);
        board.UndoMove();
    }
}

void TourFinder::FindSubTours(KnightBoard* board, int depth)
{
    int* buf = _moveStorage[depth];
    int n = board->GetMoves(buf);
    if (n == 0)
    {
        if (board->SquaresVisited() == board->TotalSquares())
        {
            ++_numToursFound;
            _callback(board->LatestTour());
        }
    }
    else
    {
        for (int i = 0; i < n && !_stopSearching; i++)
        {
            const auto& move = buf[i];
            board->MakeMove(move);
            FindSubTours(board, depth+1);
            board->UndoMove();
        }
    }
}
