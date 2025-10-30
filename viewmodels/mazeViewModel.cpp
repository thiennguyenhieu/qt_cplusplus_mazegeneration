#include "mazeViewModel.hpp"

MazeViewModel::MazeViewModel(QObject *parent)
    : QObject(parent)
{
    connect(&m_model, &MazeModel::mazeUpdated,
            this, &MazeViewModel::onMazeUpdated);

    connect(&m_model, &MazeModel::solverUpdated,
            this, &MazeViewModel::onSolverUpdated);
}

void MazeViewModel::generateMaze()
{
    m_model.generateMaze();
}

void MazeViewModel::solveMaze(int type)
{
    m_model.solveMaze(static_cast<MazeModel::SolveType>(type));
}

QList<QVariantList> MazeViewModel::mazeData() const
{
    return m_mazeData;
}

QVariantList MazeViewModel::startPosition() const
{
    return m_startPosition;
}

QVariantList MazeViewModel::endPosition() const
{
    return m_endPosition;
}

QList<QVariantList> MazeViewModel::pathSolved() const
{
    return m_pathSolved;
}

QList<QVariantList> MazeViewModel::pathVisited() const
{
    return m_pathVisited;
}

void MazeViewModel::onMazeUpdated(const QVector<QVector<int>> &walls,
                                  const QPair<int,int> &startPos,
                                  const QPair<int,int> &endPos)
{
    QList<QVariantList> outer;
    for (const auto &row : walls)
    {
        QVariantList inner;
        for (int v : row)
        {
            inner.append(v);
        }
        outer.append(inner);
    }
    m_mazeData = outer;
    emit mazeDataChanged();

    m_startPosition = QVariantList{ startPos.first, startPos.second };
    m_endPosition = QVariantList{ endPos.first, endPos.second };
    emit startEndChanged();
}

void MazeViewModel::onSolverUpdated(const QVector<QPair<int,int>> &pathSolved,
                                    const QVector<QPair<int,int>> &pathVisited)
{
    QList<QVariantList> solved, visited;
    for (const auto &pt : pathSolved)
    {
        solved.append(QVariantList{ pt.first, pt.second });
    }
    for (const auto &pt : pathVisited)
    {
        visited.append(QVariantList{ pt.first, pt.second });
    }

    m_pathSolved = solved;
    m_pathVisited = visited;

    emit pathDataChanged();
}
