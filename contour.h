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

#include <QMetaType>

typedef QVector<QPoint> TPoints;
//typedef QVector<QLine> TLines;

//Clockwise
class Contour
{
public:
    Contour();

    bool IsPointInContour(const QPoint& i_point) const;
    //void AddLine(const QLine& i_line);
    bool AddPoint(const QPoint& i_point);
    TPoints GetContourPoints() const;
    //TLines GetContourLines() const;
    bool IsClosed() const;
    bool operator==(const Contour& i_other);

private:
    bool m_is_closed;
    TPoints m_points;
    //TLines m_lines;
};

Q_DECLARE_METATYPE(Contour)

typedef QVector<Contour> TContours;

#endif // CONTOUR_H
