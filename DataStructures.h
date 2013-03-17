#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QVector>
#include <QLine>
#include <QMap>

struct Lines
{
    QList<QLine> m_lines;
    int m_max_width;
    int m_max_height;
};

typedef QMap<QPoint, QPair<QLine, QLine> > PointsLines;
#endif // DATASTRUCTURES_H
