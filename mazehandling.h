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
    enum eGENMAZE_TYPE
    {
        GENMAZE_DFS,
        GENMAZE_KRUSKAL,
        GENMAZE_PRIM,
        GENMAZE_WILSON
    };

    enum eSOLVEMAZE_TYPE
    {
        SOLVEMAZE_DFS,
        SOLVEMAZE_BFS
    };

    explicit MazeHandling(QWidget* parent = nullptr);
    ~MazeHandling() override;

    void setGenMazeType(eGENMAZE_TYPE eGenMazeType);
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
    NEIGHBOR_INFO getRandomSolveNeighborDir(Cell* cell);

private:
    MAZEDATA m_arrayMazeData;
    bool m_bMazeDataAvailable;

    std::vector<Cell*> m_pathSolvedData;
    bool m_bSolveDataAvailable;

    Cell* m_pStartCell;
    Cell* m_pEndCell;

    eGENMAZE_TYPE m_eGenMazeType;
    eSOLVEMAZE_TYPE m_eSolveMazeType;
};

#endif // MAZEHANDLING_H_
