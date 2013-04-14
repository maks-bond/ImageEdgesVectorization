#ifndef CONTOUR_H
#define CONTOUR_H

#include "geometryutils.h"
#include "DataStructures.h"

#include <QSet>
#include <QPoint>
#include <QLine>
#include <QPair>
#include <QMap>
#include <QVector>
#include <QList>

#include <QMetaType>

typedef QList<QPoint> TPoints;
//typedef QVector<QLine> TLines;

unsigned qHash(const QPoint& i_point);

//Clockwise
class Contour
{
public:
    Contour();

    bool IsPointInContour(const QPoint& i_point) const;
    //void AddLine(const QLine& i_line);
    bool AddPoint(const QPoint& i_point);
    void Clear();
    TPoints GetContourPoints() const;
    void MakeClosed();
    //TLines GetContourLines() const;
    bool IsClosed() const;
    bool IsEmpty() const;
    bool operator==(const Contour& i_other);

private:
    bool m_is_closed;
    QSet<QPoint> m_points;
    QPoint m_begin_point;

    //TLines m_lines;
};

Q_DECLARE_METATYPE(Contour)

typedef QVector<Contour> TContours;

#endif // CONTOUR_H
