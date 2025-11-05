#pragma once

class Cell
{
public:
    enum Wall : char {
        WALL_NONE  = 0,
        WALL_NORTH = 1,
        WALL_EAST  = 2,
        WALL_SOUTH = 4,
        WALL_WEST  = 8,
        WALL_ALL   = 15
    };

    explicit Cell(int row = 0, int col = 0);
    ~Cell();

    void setVisited(bool visited);
    bool isVisited() const;

    void setWall(char wall);
    char wall() const;
    void removeWall(char direction);

    bool isNorthWall() const;
    bool isEastWall() const;
    bool isSouthWall() const;
    bool isWestWall() const;

    void setRow(int row);
    int row() const;

    void setCol(int col);
    int col() const;

    void setParent(Cell *parent);
    Cell *parent() const;

private:
    bool m_visited;
    char m_wall;
    int m_row;
    int m_col;
    Cell *m_parent;
};

struct Node { // Use for A*
    Cell* cell;
    int gCost;  // distance from start
    int hCost;  // heuristic estimate to goal
    int fCost() const { return gCost + hCost; }
};
