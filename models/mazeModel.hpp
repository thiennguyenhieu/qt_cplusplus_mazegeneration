#pragma once

#include <QObject>
#include <QVector>
#include <vector>
#include "cell.hpp"

typedef std::pair<char, Cell*> NeighborInfo;

class MazeModel : public QObject
{
    Q_OBJECT

public:
    enum SolveType {
        SOLVE_DFS,
        SOLVE_BFS
    };
    Q_ENUM(SolveType)

    explicit MazeModel(QObject *parent = nullptr);
    ~MazeModel();

    void generateMaze();
    void solveMaze(SolveType type);

signals:
    void mazeUpdated(const QVector<QVector<int>> &walls,
                     const QPair<int,int> &startPos,
                     const QPair<int,int> &endPos);

    void solverUpdated(const QVector<QPair<int,int>> &pathSolved,
                       const QVector<QPair<int,int>> &pathVisited);

private:
    void releaseMazeData();
    void resetSolverData();

    void carveDFS();
    void solveDFS();
    void solveBFS();

    NeighborInfo getRandomUnvisitedNeighbor(Cell *cell, bool forSolving);
    void getUnvisitedNeighbors(Cell *cell, std::vector<Cell*> &neighbors);

    void emitMazeData();

private:
    std::vector<std::vector<Cell*>> m_mazeData;
    Cell *m_startCell;
    Cell *m_endCell;

    QVector<QPair<int,int>> m_pathSolved;
    QVector<QPair<int,int>> m_pathVisited;

    static constexpr int MAZE_ROWS = 50;
    static constexpr int MAZE_COLS = 50;
    static constexpr int START_ROW = 0;
    static constexpr int START_COL = 0;
    static constexpr int END_ROW = 49;
    static constexpr int END_COL = 49;
};
