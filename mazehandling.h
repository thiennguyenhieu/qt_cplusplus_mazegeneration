#ifndef MAZEHANDLING_H_
#define MAZEHANDLING_H_

#include <QWidget>
#include <vector>

class Cell;

typedef std::pair<char, Cell*> NEIGHBOR_INFO;
typedef std::vector<std::vector<Cell*>> MAZEDATA;

class MazeHandling : public QWidget
{
public:
    enum eSOLVEMAZE_TYPE
    {
        SOLVEMAZE_DFS,
        SOLVEMAZE_BFS
    };

    explicit MazeHandling(QWidget* parent = nullptr);
    ~MazeHandling() override;

    void setSolveMazeType(eSOLVEMAZE_TYPE eSolveMazeType);

    void generateMazeData();
    void solveMazeData();

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void releaseMazeData();
    void resetVisitedFlag();

    void carveDFS();
    NEIGHBOR_INFO getRandomGenNeighborDir(Cell* cell);

    void solveDFS();
    void solveBFS();

    NEIGHBOR_INFO getRandomSolveNeighborDir(Cell* cell);
    void getAvailableSolveNeightbors(Cell* cell, std::vector<Cell*>& listCell);

private:
    MAZEDATA m_arrayMazeData;
    bool m_bMazeDataAvailable;

    std::vector<Cell*> m_pathSolvedData;
    std::vector<Cell*> m_pathTraversedData;
    bool m_bSolveDataAvailable;

    Cell* m_pStartCell;
    Cell* m_pEndCell;

    eSOLVEMAZE_TYPE m_eSolveMazeType;
};

#endif // MAZEHANDLING_H_
