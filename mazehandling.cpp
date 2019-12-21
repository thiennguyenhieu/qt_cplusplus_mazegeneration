#include "mazehandling.h"
#include "mazecell.h"
#include <stdlib.h>
#include <time.h>
#include <QPainter>
#include <stack>
#include <queue>
#include <chrono>
#include "mainwindow.h"

#define MAZE_ROWS 50
#define MAZE_COLS 50
#define CELL_SIZE 16
#define CELL_LINE 4

MazeHandling::MazeHandling(QWidget* parent) : QWidget(parent)
{
    m_bMazeDataAvailable = false;
    m_bSolveDataAvailable = false;
    m_pStartCell = nullptr;
    m_pEndCell = nullptr;
    m_eSolveMazeType = SOLVEMAZE_DFS;
}

MazeHandling::~MazeHandling()
{
    releaseMazeData();
}

void MazeHandling::releaseMazeData()
{
    if (m_arrayMazeData.size() > 0)
    {
        for (size_t nRowIndex = 0; nRowIndex < MAZE_ROWS; ++nRowIndex)
        {
            if (m_arrayMazeData[nRowIndex].size() > 0)
            {
                for (size_t nColIndex = 0; nColIndex < MAZE_COLS; ++nColIndex)
                {
                    Cell* cell = m_arrayMazeData[nRowIndex][nColIndex];
                    if (cell) delete cell;
                }
                m_arrayMazeData[nRowIndex].clear();
            }
        }

        m_arrayMazeData.clear();
    }
}

void MazeHandling::resetVisitedFlag()
{
    if (m_arrayMazeData.size() > 0)
    {
        for (size_t nRowIndex = 0; nRowIndex < MAZE_ROWS; ++nRowIndex)
        {
            if (m_arrayMazeData[nRowIndex].size() > 0)
            {
                for (size_t nColIndex = 0; nColIndex < MAZE_COLS; ++nColIndex)
                {
                    Cell* cell = m_arrayMazeData[nRowIndex][nColIndex];
                    if (cell) cell->setVisited(false);
                }
            }
        }
    }
}

void MazeHandling::setSolveMazeType(eSOLVEMAZE_TYPE eSolveMazeType)
{
    m_eSolveMazeType = eSolveMazeType;
}

void MazeHandling::generateMazeData()
{
    releaseMazeData();
    m_bMazeDataAvailable = false;

    // Generate new maze, also clear solved data of previous maze
    m_bSolveDataAvailable = false;
    m_pathSolvedData.clear();
    m_pathTraversedData.clear();

    for (int nRowIndex = 0; nRowIndex < MAZE_ROWS; ++nRowIndex)
    {
        std::vector<Cell*> vectorCol;
        for (int nColIndex = 0; nColIndex < MAZE_COLS; ++nColIndex)
        {
            Cell* cell = new Cell(nRowIndex, nColIndex);
            vectorCol.push_back(cell);
        }
        m_arrayMazeData.push_back(vectorCol);
    }

    carveDFS();

    // Set start and end points
    m_pStartCell = m_arrayMazeData[0][0];
    m_pEndCell = m_arrayMazeData[MAZE_ROWS - 1][MAZE_COLS - 1];

    m_bMazeDataAvailable = true;
    this->update();
}

void MazeHandling::carveDFS()
{
    /*
       1. Choose the initial cell, mark it as visited and push it to the stack
       2. While the stack is not empty
           1. Pop a cell from the stack and make it a current cell
           2. If the current cell has any neighbors which have not been visited
               1. Push the current cell to the stack
               2. Choose one of the unvisited neighbors
               3. Remove the wall between the current cell and the chosen cell
               4. Mark the chosen cell as visited and push it to the stack
    */

    // Random a initial cell
    srand(static_cast<unsigned int>(time(nullptr)));
    size_t ranRowIndex = rand() % MAZE_ROWS;
    size_t ranColIndex = rand() % MAZE_COLS;

    Cell* randomCell = m_arrayMazeData[ranRowIndex][ranColIndex];
    if (randomCell)
        randomCell->setVisited(true);

    std::stack<Cell*> stackVisitedCell;
    stackVisitedCell.push(randomCell);

    while( !stackVisitedCell.empty() )
    {
        Cell* curCell = stackVisitedCell.top();
        NEIGHBOR_INFO infoNeighbor = getRandomGenNeighborDir(curCell);

        // If there is no unvisited neighbor, remove the last visited cell from the stack
        // Then, back track to process the top cell of the stack
        char nextDir = infoNeighbor.first;
        if (nextDir == -1)
        {
            stackVisitedCell.pop();
            continue;
        }

        Cell* nextCell = infoNeighbor.second;
        if ((curCell != nullptr) && (nextCell != nullptr))
        {
            nextCell->setVisited(true);
            stackVisitedCell.push(nextCell);

            if (nextDir == Cell::WALL_NORTH)
            {
                curCell->removeWall(Cell::WALL_NORTH);
                nextCell->removeWall(Cell::WALL_SOUTH);
            }
            else if (nextDir == Cell::WALL_EAST)
            {
                curCell->removeWall(Cell::WALL_EAST);
                nextCell->removeWall(Cell::WALL_WEST);
            }
            else if (nextDir == Cell::WALL_SOUTH)
            {
                curCell->removeWall(Cell::WALL_SOUTH);
                nextCell->removeWall(Cell::WALL_NORTH);
            }
            else
            {
                curCell->removeWall(Cell::WALL_WEST);
                nextCell->removeWall(Cell::WALL_EAST);
            }
        }
    }
}

NEIGHBOR_INFO MazeHandling::getRandomGenNeighborDir(Cell* cell)
{
    NEIGHBOR_INFO retInfo(-1, nullptr);

    if (cell == nullptr)
        return retInfo;

    int dirNorth = cell->getRowIdx() - 1;
    int dirEast  = cell->getColIdx() + 1;
    int dirSouth = cell->getRowIdx() + 1;
    int dirWest  = cell->getColIdx() - 1;

    std::vector<NEIGHBOR_INFO> vectorAvaiNeightbor;

    if (dirNorth >= 0)
    {
        Cell* cellNorth = m_arrayMazeData[static_cast<size_t>(dirNorth)][static_cast<size_t>(cell->getColIdx())];
        if ( (cellNorth) && (cellNorth->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_NORTH, cellNorth));
    }

    if (dirEast < MAZE_COLS)
    {
        Cell* cellEast = m_arrayMazeData[static_cast<size_t>(cell->getRowIdx())][static_cast<size_t>(dirEast)];
        if ( (cellEast) && (cellEast->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_EAST, cellEast));
    }

    if (dirSouth < MAZE_ROWS)
    {
        Cell* cellSouth = m_arrayMazeData[static_cast<size_t>(dirSouth)][static_cast<size_t>(cell->getColIdx())];
        if ( (cellSouth) && (cellSouth->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_SOUTH, cellSouth));
    }

    if (dirWest >= 0)
    {
        Cell* cellWest = m_arrayMazeData[static_cast<size_t>(cell->getRowIdx())][static_cast<size_t>(dirWest)];
        if ( (cellWest) && (cellWest->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_WEST, cellWest));
    }

    // Get a random neighbor
    if (vectorAvaiNeightbor.size() > 0)
    {
        size_t ranAvaiNeightbor = static_cast<size_t>(rand()) % vectorAvaiNeightbor.size();
        retInfo = vectorAvaiNeightbor.at(ranAvaiNeightbor);
    }

    return retInfo;
}

void MazeHandling::solveMazeData()
{
    if (!m_bMazeDataAvailable)
        return;

    resetVisitedFlag();
    m_bSolveDataAvailable = false;
    m_pathSolvedData.clear();
    m_pathTraversedData.clear();

    srand(static_cast<unsigned int>(time(nullptr)));

    auto t1 = std::chrono::high_resolution_clock::now();

    switch (m_eSolveMazeType)
    {
    case SOLVEMAZE_DFS:
        solveDFS();
        break;
    case SOLVEMAZE_BFS:
        solveBFS();
        break;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    int nExecuteTime = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());

    QObject* pParent = this->parent();
    static_cast<MainWindow*>(pParent)->updateExecuteTime(nExecuteTime);

    m_bSolveDataAvailable = true;
    this->update();
}

void MazeHandling::solveDFS()
{
    /*
    Depth-First-Search( Maze m )
        Mark m.StartNode "Visited"
        PushStack( m.StartNode )

        While Stack.NotEmpty
            c <- TopStack
            If c is the goal
                Exit
            Else
                If there are unvisited available neighbors
                    Chose a random neighbor n of c
                        Mark n "Visited"
                        PushStack( n )
                Else
                    PopStack(c)
    End procedure
    */

    if ((m_pStartCell == nullptr) || (m_pEndCell == nullptr))
        return;

    std::stack<Cell*> stackVisitedCell;

    m_pStartCell->setVisited(true);
    m_pStartCell->setParentCell(nullptr);
    stackVisitedCell.push(m_pStartCell);

    while ( !stackVisitedCell.empty() )
    {
        Cell* curCell = stackVisitedCell.top();

        if ((curCell->getRowIdx() == m_pEndCell->getRowIdx()) && (curCell->getColIdx() == m_pEndCell->getColIdx()))
        {
            break;  // End point is found, exit the loop
        }
        else
        {
            NEIGHBOR_INFO infoNeighbor = getRandomSolveNeighborDir(curCell);
            char nextDir = infoNeighbor.first;

            if (nextDir != -1)
            {
                Cell* nextCell = infoNeighbor.second;
                if (nextCell != nullptr)
                {
                    nextCell->setVisited(true);
                    nextCell->setParentCell(curCell);
                    stackVisitedCell.push(nextCell);
                }
            }
            else
            {
                m_pathTraversedData.push_back(curCell);
                stackVisitedCell.pop();
            }
        }
    }

    if ( !stackVisitedCell.empty() )
    {
        while ( !stackVisitedCell.empty() )
        {
            Cell* curCell = stackVisitedCell.top();
            m_pathSolvedData.push_back(curCell);
            stackVisitedCell.pop( );
        }
    }
}

void MazeHandling::solveBFS()
{
    /*
    Breadth-First-Search( Maze m )
        Mark m.StartNode "Visited"
        EnQueue( m.StartNode )

        While Queue.NotEmpty
            c <- DeQueue
            If c is the goal
                Exit
            Else
                Foreach unvisted neighbors n of c
                    Mark n "Visited"
                    EnQueue( n )
                Mark c "Examined"
    End procedure
    */

    if ((m_pStartCell == nullptr) || (m_pEndCell == nullptr))
        return;

    std::queue<Cell*> queueVisitedCell;
    std::vector<Cell*> vecExaminedCell;

    m_pStartCell->setVisited(true);
    m_pStartCell->setParentCell(nullptr);
    queueVisitedCell.push(m_pStartCell);

    while ( !queueVisitedCell.empty() )
    {
        // first: cell, second: it's parent
        Cell* curCell = queueVisitedCell.front();
        queueVisitedCell.pop();

        if ((curCell->getRowIdx() == m_pEndCell->getRowIdx()) && (curCell->getColIdx() == m_pEndCell->getColIdx()))
        {
            vecExaminedCell.push_back(curCell);
            break;  // End point is found, exit the loop
        }
        else
        {
            std::vector<Cell *> listNeighborCell;
            getAvailableSolveNeightbors(curCell, listNeighborCell);

            for (size_t nCellIdx = 0; nCellIdx < listNeighborCell.size(); ++nCellIdx)
            {
                Cell* neighborCell = listNeighborCell.at(nCellIdx);

                neighborCell->setVisited(true);
                neighborCell->setParentCell(curCell);

                queueVisitedCell.push(neighborCell);
            }

            vecExaminedCell.push_back(curCell);
        }
    }

    // Because examined list is containing unexpected cells,
    // now we find the path from end cell by checking availability of parent cell
    size_t nExaminedCellSize = vecExaminedCell.size();
    if (nExaminedCellSize > 3) // at least there are some cells except start and end cells
    {
        Cell* curCell = vecExaminedCell[nExaminedCellSize - 1];
        m_pathSolvedData.push_back(curCell);

        size_t nPreviousIndex = nExaminedCellSize - 2;
        Cell* previousCell = nullptr;

        while ( curCell->getParentCell() != nullptr )
        {
            previousCell = vecExaminedCell[nPreviousIndex];

            if (previousCell == curCell->getParentCell())
            {
                m_pathSolvedData.push_back(previousCell);

                if (previousCell->getParentCell() == nullptr)
                    break;

                curCell = previousCell;
            }
            else
            {
                m_pathTraversedData.push_back(previousCell);
            }

            nPreviousIndex--;
        }
    }
}

NEIGHBOR_INFO MazeHandling::getRandomSolveNeighborDir(Cell* cell)
{
    NEIGHBOR_INFO retInfo(-1, nullptr);

    if (cell == nullptr)
        return retInfo;

    int dirNorth = cell->getRowIdx() - 1;
    int dirEast  = cell->getColIdx() + 1;
    int dirSouth = cell->getRowIdx() + 1;
    int dirWest  = cell->getColIdx() - 1;
    std::vector<NEIGHBOR_INFO> vectorAvaiNeightbor;

    if ((dirNorth >= 0) && (!cell->isNorthWall()))
    {
        Cell* cellNorth = m_arrayMazeData[static_cast<size_t>(dirNorth)][static_cast<size_t>(cell->getColIdx())];
        if ( (cellNorth) && (cellNorth->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_NORTH, cellNorth));
    }

    if ((dirEast < MAZE_COLS) && (!cell->isEastWall()))
    {
        Cell* cellEast = m_arrayMazeData[static_cast<size_t>(cell->getRowIdx())][static_cast<size_t>(dirEast)];
        if ( (cellEast) && (cellEast->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_EAST, cellEast));
    }

    if ((dirSouth < MAZE_ROWS) &&(!cell->isSouthWall()))
    {
        Cell* cellSouth = m_arrayMazeData[static_cast<size_t>(dirSouth)][static_cast<size_t>(cell->getColIdx())];
        if ( (cellSouth) && (cellSouth->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_SOUTH, cellSouth));
    }

    if ((dirWest >= 0) && (!cell->isWestWall()))
    {
        Cell* cellWest = m_arrayMazeData[static_cast<size_t>(cell->getRowIdx())][static_cast<size_t>(dirWest)];
        if ( (cellWest) && (cellWest->isVisited() == false) )
            vectorAvaiNeightbor.push_back(NEIGHBOR_INFO(Cell::WALL_WEST, cellWest));
    }

    // Get a random neighbor
    if (vectorAvaiNeightbor.size() > 0)
    {
        size_t ranAvaiNeightbor = static_cast<size_t>(rand()) % vectorAvaiNeightbor.size();
        retInfo = vectorAvaiNeightbor.at(ranAvaiNeightbor);
    }

    return retInfo;
}

void MazeHandling::getAvailableSolveNeightbors(Cell *cell, std::vector<Cell *> &listCell)
{
    if (cell == nullptr)
        return;

    int dirNorth = cell->getRowIdx() - 1;
    int dirEast  = cell->getColIdx() + 1;
    int dirSouth = cell->getRowIdx() + 1;
    int dirWest  = cell->getColIdx() - 1;

    if ((dirNorth >= 0) && (!cell->isNorthWall()))
    {
        Cell* cellNorth = m_arrayMazeData[static_cast<size_t>(dirNorth)][static_cast<size_t>(cell->getColIdx())];
        if ( (cellNorth) && (cellNorth->isVisited() == false) )
            listCell.push_back(cellNorth);
    }

    if ((dirEast < MAZE_COLS) && (!cell->isEastWall()))
    {
        Cell* cellEast = m_arrayMazeData[static_cast<size_t>(cell->getRowIdx())][static_cast<size_t>(dirEast)];
        if ( (cellEast) && (cellEast->isVisited() == false) )
            listCell.push_back(cellEast);
    }

    if ((dirSouth < MAZE_ROWS) &&(!cell->isSouthWall()))
    {
        Cell* cellSouth = m_arrayMazeData[static_cast<size_t>(dirSouth)][static_cast<size_t>(cell->getColIdx())];
        if ( (cellSouth) && (cellSouth->isVisited() == false) )
            listCell.push_back(cellSouth);
    }

    if ((dirWest >= 0) && (!cell->isWestWall()))
    {
        Cell* cellWest = m_arrayMazeData[static_cast<size_t>(cell->getRowIdx())][static_cast<size_t>(dirWest)];
        if ( (cellWest) && (cellWest->isVisited() == false) )
            listCell.push_back(cellWest);
    }
}

void MazeHandling::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen myPen(Qt::black, CELL_LINE, Qt::SolidLine);
    painter.setPen(myPen);

    size_t nSolveDataSize = m_pathSolvedData.size();
    if ((m_bSolveDataAvailable) && (nSolveDataSize > 0))
    {
        // Draw path from 2nd ~ (n - 1)th cell because 1st cell is starting point and (n)th is end point
        for (size_t nSolveIdx = 1; nSolveIdx < nSolveDataSize - 1; ++nSolveIdx)
        {
            Cell* cell = m_pathSolvedData[nSolveIdx];

            if (cell == nullptr)
                continue;

            int ptX = static_cast<int>(cell->getColIdx() * CELL_SIZE) + CELL_LINE/2;
            int ptY = static_cast<int>(cell->getRowIdx() * CELL_SIZE) + CELL_LINE/2;

            QPainterPath path;
            path.addRect(QRectF(ptX, ptY, CELL_SIZE, CELL_SIZE));
            painter.fillPath(path, Qt::green);
        }
    }

    size_t nTraversedDataSize = m_pathTraversedData.size();
    if ((m_bSolveDataAvailable) && (nTraversedDataSize > 0))
    {
        for (size_t nTraverseIdx = 0; nTraverseIdx < nTraversedDataSize; ++nTraverseIdx)
        {
            Cell* cell = m_pathTraversedData[nTraverseIdx];

            if (cell == nullptr)
                continue;

            int ptX = static_cast<int>(cell->getColIdx() * CELL_SIZE) + CELL_LINE/2;
            int ptY = static_cast<int>(cell->getRowIdx() * CELL_SIZE) + CELL_LINE/2;

            QPainterPath path;
            path.addRect(QRectF(ptX, ptY, CELL_SIZE, CELL_SIZE));
            painter.fillPath(path, Qt::gray);
        }
    }

    if (m_bMazeDataAvailable)
    {
        for (size_t nRowIndex = 0; nRowIndex < MAZE_ROWS; ++nRowIndex)
        {
            for (size_t nColIndex = 0; nColIndex < MAZE_COLS; ++nColIndex)
            {
                Cell* cell = m_arrayMazeData[nRowIndex][nColIndex];

                if (cell == nullptr)
                    continue;

                int ptX = static_cast<int>(nColIndex * CELL_SIZE) + CELL_LINE/2;
                int ptY = static_cast<int>(nRowIndex * CELL_SIZE) + CELL_LINE/2;

                if ((m_pStartCell) && (m_pStartCell->getRowIdx() == cell->getRowIdx())
                        && (m_pStartCell->getColIdx() == cell->getColIdx()))
                {
                    QPainterPath path;
                    path.addRoundedRect(QRectF(ptX + CELL_LINE/2, ptY + CELL_LINE/2, CELL_SIZE - CELL_LINE, CELL_SIZE - CELL_LINE), CELL_SIZE/2, CELL_SIZE/2);
                    painter.fillPath(path, Qt::blue);
                }

                if ((m_pEndCell) && (m_pEndCell->getRowIdx() == cell->getRowIdx())
                        && (m_pEndCell->getColIdx() == cell->getColIdx()))
                {
                    QPainterPath path;
                    path.addRoundedRect(QRectF(ptX + CELL_LINE/2, ptY + CELL_LINE/2, CELL_SIZE - CELL_LINE, CELL_SIZE - CELL_LINE), CELL_SIZE/2, CELL_SIZE/2);
                    painter.fillPath(path, Qt::red);
                }

                if (cell->isNorthWall())
                    painter.drawLine(ptX, ptY, ptX + CELL_SIZE, ptY);

                if (cell->isEastWall())
                    painter.drawLine(ptX + CELL_SIZE, ptY, ptX + CELL_SIZE, ptY + CELL_SIZE);

                if (cell->isSouthWall())
                    painter.drawLine(ptX, ptY + CELL_SIZE, ptX + CELL_SIZE, ptY + CELL_SIZE);

                if (cell->isWestWall())
                    painter.drawLine(ptX, ptY, ptX, ptY + CELL_SIZE);
            }
        }
    }
}
