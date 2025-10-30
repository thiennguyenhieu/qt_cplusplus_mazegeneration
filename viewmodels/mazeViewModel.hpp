#pragma once
#include <QObject>
#include <QVector>
#include <QPair>
#include <QVariantList>
#include "../models/mazeModel.hpp"

class MazeViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QVariantList> mazeData READ mazeData NOTIFY mazeDataChanged)
    Q_PROPERTY(QVariantList startPosition READ startPosition NOTIFY startEndChanged)
    Q_PROPERTY(QVariantList endPosition READ endPosition NOTIFY startEndChanged)
    Q_PROPERTY(QList<QVariantList> pathSolved READ pathSolved NOTIFY pathDataChanged)
    Q_PROPERTY(QList<QVariantList> pathVisited READ pathVisited NOTIFY pathDataChanged)

public:
    explicit MazeViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void generateMaze();
    Q_INVOKABLE void solveMaze(int type);

    // Read functions for QML bindings
    QList<QVariantList> mazeData() const;
    QVariantList startPosition() const;
    QVariantList endPosition() const;
    QList<QVariantList> pathSolved() const;
    QList<QVariantList> pathVisited() const;

signals:
    void mazeDataChanged();
    void startEndChanged();
    void pathDataChanged();

private slots:
    void onMazeUpdated(const QVector<QVector<int>> &walls,
                       const QPair<int,int> &startPos,
                       const QPair<int,int> &endPos);

    void onSolverUpdated(const QVector<QPair<int,int>> &pathSolved,
                         const QVector<QPair<int,int>> &pathVisited);

private:
    MazeModel m_model;

    QList<QVariantList> m_mazeData;
    QVariantList m_startPosition;   // [row, col]
    QVariantList m_endPosition;     // [row, col]
    QList<QVariantList> m_pathSolved;
    QList<QVariantList> m_pathVisited;
};
