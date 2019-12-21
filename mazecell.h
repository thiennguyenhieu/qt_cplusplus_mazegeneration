#ifndef MAZECELL_H
#define MAZECELL_H

class Cell
{
public:
    enum eWALL
    {
        WALL_NONE   = 0,  // 00000000
        WALL_NORTH  = 1,  // 00000001
        WALL_EAST   = 2,  // 00000010
        WALL_SOUTH  = 4,  // 00000100
        WALL_WEST   = 8,  // 00001000
        WALL_ALL    = 15  // 00001111
    };

    Cell(int rowIdx, int colIdx);
    ~Cell();

    void setVisited(bool bVisited);
    bool isVisited();

    void setWall(char cWall);
    char getWall();
    void removeWall(char direction);
    bool isNorthWall();
    bool isEastWall();
    bool isSouthWall();
    bool isWestWall();

    void setRowIdx(int rowIndex);
    int getRowIdx();

    void setColIdx(int colIndex);
    int getColIdx();

    void setParentCell(Cell* cell);
    Cell* getParentCell();

private:
    bool m_bVisited;
    char m_cWall;
    int m_ptRowIdx;
    int m_ptColIdx;
    Cell* m_pParentCell;
};

#endif // MAZECELL_H
