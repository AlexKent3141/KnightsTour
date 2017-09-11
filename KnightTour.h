#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__

#include <vector>
#include <string>
#include <utility>

class KnightTour
{
public:
    KnightTour() = delete;

    KnightTour(int size, const std::vector<std::pair<int, int>>& coords)
        : _boardSize(size), _coords(coords)
    {
    }

    KnightTour(const KnightTour& other)
        : _boardSize(other._boardSize), _coords(other._coords)
    {
    }

    inline int BoardSize() const { return _boardSize; }
    inline const std::vector<std::pair<int, int>>& Coords() const { return _coords; }

    // Serialise this tour.
    std::string ToString() const
    {
        std::string str = std::to_string(_boardSize) + "\n";
        for (const auto& coord : _coords)
        {
            str += std::to_string(coord.first) + " "+ std::to_string(coord.second) + "\n";
        }

        return str;
    }

private:
    int _boardSize;
    std::vector<std::pair<int, int>> _coords;
};

#endif // __KNIGHT_TOUR_H__
