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

typedef QPair<QLine, QLine> TLinesPair;
typedef QPair<QPoint, TLinesPair> TContourPoint;
typedef QMap<QPoint, TLinesPair > TContour;

class Contour
{
public:
    Contour();

    bool IsPointInContour(const QPoint& i_point) const;
    void AddContourPoint(const TContourPoint& i_point);
    TContour GetContour() const;

private:

    TContour m_contour;
};

typedef QVector<TContour> TContours;

#endif // CONTOUR_H
