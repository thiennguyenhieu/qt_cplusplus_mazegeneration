#include "cell.hpp"

Cell::Cell(int row, int col)
    : m_row(row),
    m_col(col),
    m_visited(false),
    m_wall(WALL_ALL),
    m_parent(nullptr)
{
}

Cell::~Cell() = default;

void Cell::setVisited(bool visited)
{
    m_visited = visited;
}

bool Cell::isVisited() const
{
    return m_visited;
}

void Cell::setWall(char wall)
{
    m_wall = wall;
}

char Cell::wall() const
{
    return m_wall;
}

void Cell::removeWall(char direction)
{
    m_wall = (m_wall & ~direction);
}

bool Cell::isNorthWall() const
{
    return (m_wall & WALL_NORTH) == WALL_NORTH;
}

bool Cell::isEastWall() const
{
    return (m_wall & WALL_EAST) == WALL_EAST;
}

bool Cell::isSouthWall() const
{
    return (m_wall & WALL_SOUTH) == WALL_SOUTH;
}

bool Cell::isWestWall() const
{
    return (m_wall & WALL_WEST) == WALL_WEST;
}

int Cell::row() const
{
    return m_row;
}

int Cell::col() const
{
    return m_col;
}

void Cell::setRow(int row)
{
    m_row = row;
}

void Cell::setCol(int col)
{
    m_col = col;
}

void Cell::setParent(Cell *parent)
{
    m_parent = parent;
}

Cell *Cell::parent() const
{
    return m_parent;
}
