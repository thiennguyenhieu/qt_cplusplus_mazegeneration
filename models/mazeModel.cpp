#include "mazeModel.hpp"
#include <cstdlib>
#include <ctime>
#include <stack>
#include <queue>

MazeModel::MazeModel(QObject *parent)
    : QObject(parent),
    m_startCell(nullptr),
    m_endCell(nullptr)
{
}

MazeModel::~MazeModel()
{
    releaseMazeData();
}

void MazeModel::generateMaze()
{
    releaseMazeData();

    // Initialize grid
    for (int row = 0; row < MAZE_ROWS; ++row)
    {
        std::vector<Cell*> colVec;
        for (int col = 0; col < MAZE_COLS; ++col)
        {
            colVec.push_back(new Cell(row, col));
        }
        m_mazeData.push_back(colVec);
    }

    carveDFS();
    emitMazeData();

    m_pathSolved.clear();
    m_pathVisited.clear();
    emit solverUpdated(m_pathSolved, m_pathVisited);
}

void MazeModel::solveMaze(SolveType type)
{
    if (m_mazeData.empty())
        return;

    resetSolverData();

    if (type == SOLVE_DFS)
        solveDFS();
    else
        solveBFS();

    emit solverUpdated(m_pathSolved, m_pathVisited);
}

void MazeModel::releaseMazeData()
{
    for (auto &row : m_mazeData)
    {
        for (auto *cell : row)
        {
            delete cell;
        }
        row.clear();
    }
    m_mazeData.clear();
}

void MazeModel::resetSolverData()
{
    m_pathSolved.clear();
    m_pathVisited.clear();

    for (auto &row : m_mazeData)
    {
        for (auto *cell : row)
        {
            if (cell)
            {
                cell->setVisited(false);
                cell->setParent(nullptr);
            }
        }
    }
}

void MazeModel::carveDFS()
{
    /*
    Maze Generation using Depth-First Search (Recursive Backtracker)

    1. Choose a random initial cell
       - Mark it as visited
       - Push it onto the stack

    2. While the stack is not empty:
        a. Let the top of the stack be the current cell
        b. Get a random unvisited neighbor of the current cell
        c. If an unvisited neighbor exists:
             i.   Remove the wall between the current cell and the chosen neighbor
             ii.  Mark the neighbor as visited
             iii. Push the neighbor onto the stack
           Else:
             i.   Pop the current cell from the stack (backtrack)

    3. Repeat until the stack becomes empty
       - All cells have been visited
       - The maze is fully carved (a perfect maze with no loops)
    */

    // Random a initial cell
    size_t startRow = static_cast<size_t>(rand() % MAZE_ROWS);
    size_t startCol = static_cast<size_t>(rand() % MAZE_COLS);

    Cell *start = m_mazeData[startRow][startCol];
    start->setVisited(true);

    std::stack<Cell*> stack;
    stack.push(start);

    while (!stack.empty())
    {
        Cell *cur = stack.top();
        NeighborInfo nextInfo = getRandomUnvisitedNeighbor(cur, false);
        char dir = nextInfo.first;

        if (dir == -1)
        {
            stack.pop();
            continue;
        }

        Cell *next = nextInfo.second;
        if (next)
        {
            next->setVisited(true);
            stack.push(next);

            // Knock down walls between current and next
            if (dir == Cell::WALL_NORTH)
            {
                cur->removeWall(Cell::WALL_NORTH);
                next->removeWall(Cell::WALL_SOUTH);
            }
            else if (dir == Cell::WALL_EAST)
            {
                cur->removeWall(Cell::WALL_EAST);
                next->removeWall(Cell::WALL_WEST);
            }
            else if (dir == Cell::WALL_SOUTH)
            {
                cur->removeWall(Cell::WALL_SOUTH);
                next->removeWall(Cell::WALL_NORTH);
            }
            else if (dir == Cell::WALL_WEST)
            {
                cur->removeWall(Cell::WALL_WEST);
                next->removeWall(Cell::WALL_EAST);
            }
        }
    }

    // Set start and end points
    m_startCell = m_mazeData[START_ROW][START_COL];
    m_endCell = m_mazeData[END_ROW][END_COL];
}

void MazeModel::solveDFS()
{
    /*
    Depth-First-Search

    1. Mark m_startCell as visited and push it onto the stack

    2. While the stack is not empty:
        a. Let the top of the stack be the current cell
        b. If the current cell == m_endCell:
               Break   // Goal reached
        c. Else:
            i.   Get a random unvisited neighbor of the current cell
            ii.  If a neighbor exists:
                     - Mark it as visited
                     - Set its Parent = current cell
                     - Push it onto the stack
            iii. Else:
                     - Pop the current cell from the stack (backtrack)

    3. After the loop:
        a. Backtrack from m_endCell to m_startCell using Parent pointers
        b. Store this sequence in m_pathSolved (final path for visualization)
    */

    if (!m_startCell || !m_endCell)
        return;

    std::stack<Cell*> stack;
    m_startCell->setVisited(true);
    m_startCell->setParent(nullptr);
    stack.push(m_startCell);

    // Record starting cell as visited
    m_pathVisited.push_back(qMakePair(m_startCell->row(), m_startCell->col()));

    while (!stack.empty())
    {
        Cell *cur = stack.top();

        if (cur == m_endCell)
            break;

        NeighborInfo next = getRandomUnvisitedNeighbor(cur, true);
        if (next.first != -1 && next.second)
        {
            next.second->setVisited(true);
            next.second->setParent(cur);
            stack.push(next.second);

            // Record each newly visited cell (not just backtracked ones)
            m_pathVisited.push_back(qMakePair(next.second->row(), next.second->col()));
        }
        else
        {
            stack.pop();
        }
    }

    while (!stack.empty()) {
        m_pathSolved.push_back(qMakePair(stack.top()->row(), stack.top()->col()));
        stack.pop();
    }
}

void MazeModel::solveBFS()
{
    /*
    Maze Solving using Breadth-First Search (BFS)

    1. Initialize:
       - Mark m_startCell as visited
       - Enqueue(m_startCell)

    2. While the queue is not empty:
        a. Dequeue the front cell and make it the current cell
        b. If the current cell == m_endCell:
               Break   // Goal reached
        c. Get all unvisited, accessible neighbors of the current cell
        d. For each neighbor n:
               i.   Mark n as visited
               ii.  Set n->Parent = current cell
               iii. Enqueue(n)

    3. After the loop:
        a. If m_endCell was reached:
               - Backtrack from m_endCell to m_startCell using Parent pointers
               - Store this sequence in m_pathSolved (shortest path)
    */

    if (!m_startCell || !m_endCell)
        return;

    std::queue<Cell*> queue;
    m_startCell->setVisited(true);
    m_startCell->setParent(nullptr);
    queue.push(m_startCell);

    // Record starting cell as visited
    m_pathVisited.push_back(qMakePair(m_startCell->row(), m_startCell->col()));

    while (!queue.empty())
    {
        Cell *cur = queue.front();
        queue.pop();

        if (cur == m_endCell)
            break;

        std::vector<Cell*> neighbors;
        getUnvisitedNeighbors(cur, neighbors);
        for (Cell *n : neighbors)
        {
            n->setVisited(true);
            n->setParent(cur);
            queue.push(n);

            // Record each newly visited cell (not just backtracked ones)
            m_pathVisited.push_back(qMakePair(n->row(), n->col()));

            if (n == m_endCell) {
                break;
            }
        }
    }

    // Backtrack from end to start
    Cell *cur = m_endCell;
    while (cur)
    {
        m_pathSolved.push_back(qMakePair(cur->row(), cur->col()));
        cur = cur->parent();
    }
}

NeighborInfo MazeModel::getRandomUnvisitedNeighbor(Cell *cell, bool forSolving)
{
    NeighborInfo ret(-1, nullptr);
    if (!cell) return ret;

    int row = cell->row();
    int col = cell->col();
    std::vector<NeighborInfo> neighbors;

    // --- NORTH ---
    if (row - 1 >= 0)
    {
        Cell *north = m_mazeData[row - 1][col];
        // If solving: only add if there's NO wall to the north
        if ((!forSolving || !cell->isNorthWall()) && !north->isVisited())
            neighbors.emplace_back(Cell::WALL_NORTH, north);
    }

    // --- EAST ---
    if (col + 1 < MAZE_COLS)
    {
        Cell *east = m_mazeData[row][col + 1];
        if ((!forSolving || !cell->isEastWall()) && !east->isVisited())
            neighbors.emplace_back(Cell::WALL_EAST, east);
    }

    // --- SOUTH ---
    if (row + 1 < MAZE_ROWS)
    {
        Cell *south = m_mazeData[row + 1][col];
        if ((!forSolving || !cell->isSouthWall()) && !south->isVisited())
            neighbors.emplace_back(Cell::WALL_SOUTH, south);
    }

    // --- WEST ---
    if (col - 1 >= 0)
    {
        Cell *west = m_mazeData[row][col - 1];
        if ((!forSolving || !cell->isWestWall()) && !west->isVisited())
            neighbors.emplace_back(Cell::WALL_WEST, west);
    }

    // --- Random pick ---
    if (!neighbors.empty())
    {
        size_t idx = static_cast<size_t>(rand() % neighbors.size());
        ret = neighbors[idx];
    }

    return ret;
}

void MazeModel::getUnvisitedNeighbors(Cell *cell, std::vector<Cell*> &neighbors)
{
    int row = cell->row();
    int col = cell->col();

    if (row - 1 >= 0 && !cell->isNorthWall())
    {
        Cell *north = m_mazeData[row - 1][col];
        if (!north->isVisited())
            neighbors.push_back(north);
    }
    if (col + 1 < MAZE_COLS && !cell->isEastWall())
    {
        Cell *east = m_mazeData[row][col + 1];
        if (!east->isVisited())
            neighbors.push_back(east);
    }
    if (row + 1 < MAZE_ROWS && !cell->isSouthWall())
    {
        Cell *south = m_mazeData[row + 1][col];
        if (!south->isVisited())
            neighbors.push_back(south);
    }
    if (col - 1 >= 0 && !cell->isWestWall())
    {
        Cell *west = m_mazeData[row][col - 1];
        if (!west->isVisited())
            neighbors.push_back(west);
    }
}

void MazeModel::emitMazeData()
{
    QVector<QVector<int>> wallData;
    wallData.resize(MAZE_ROWS);

    for (int r = 0; r < MAZE_ROWS; ++r) {
        wallData[r].resize(MAZE_COLS);
        for (int c = 0; c < MAZE_COLS; ++c) {
            Cell *cell = m_mazeData[r][c];
            if (cell)
                wallData[r][c] = cell->wall();
            else
                wallData[r][c] = Cell::WALL_ALL;
        }
    }

    QPair<int,int> startPos(START_ROW, START_COL);
    QPair<int,int> endPos(END_ROW, END_COL);

    emit mazeUpdated(wallData, startPos, endPos);
}
