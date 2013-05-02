#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QVector>
#include <QLine>
#include <QMap>

#include <QMetaType>

struct Lines
{
    QVector<QLine> m_lines;
    int m_max_width;
    int m_max_height;
};

typedef QPair<QPoint, QPoint> TPointsPair;
typedef QMap<QPoint, TPointsPair > TLinkedPoints;

Q_DECLARE_METATYPE(Lines)
Q_DECLARE_METATYPE(TLinkedPoints)

#endif // DATASTRUCTURES_H
