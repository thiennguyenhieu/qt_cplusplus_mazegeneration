#include "mazecell.h"

Cell::Cell(int rowIdx, int colIdx)
{
    m_ptRowIdx = rowIdx;
    m_ptColIdx = colIdx;
    m_bVisited = false;
    m_cWall = WALL_ALL;
    m_pParentCell = nullptr;
}

Cell::~Cell()
{
}

void Cell::setVisited(bool bVisited)
{
    m_bVisited = bVisited;
}

bool Cell::isVisited()
{
    return m_bVisited;
}

void Cell::setWall(char cWall)
{
    m_cWall = cWall;
}

char Cell::getWall()
{
    return m_cWall;
}

void Cell::removeWall(char direction)
{
    m_cWall = (m_cWall & ~direction);
}

bool Cell::isNorthWall()
{
    return ((getWall() & Cell::WALL_NORTH) == Cell::WALL_NORTH);
}

bool Cell::isEastWall()
{
    return ((getWall() & Cell::WALL_EAST) == Cell::WALL_EAST);
}

bool Cell::isSouthWall()
{
    return ((getWall() & Cell::WALL_SOUTH) == Cell::WALL_SOUTH);
}

bool Cell::isWestWall()
{
    return ((getWall() & Cell::WALL_WEST) == Cell::WALL_WEST);
}

void Cell::setRowIdx(int rowIndex)
{
    m_ptRowIdx = rowIndex;
}

int Cell::getRowIdx()
{
    return m_ptRowIdx;
}

void Cell::setColIdx(int colIndex)
{
    m_ptColIdx = colIndex;
}

int Cell::getColIdx()
{
    return m_ptColIdx;
}

void Cell::setParentCell(Cell *cell)
{
    m_pParentCell = cell;
}

Cell* Cell::getParentCell()
{
    return m_pParentCell;
}
