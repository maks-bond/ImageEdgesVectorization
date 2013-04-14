#ifndef CONTOUR_H
#define CONTOUR_H

#include <QSet>
#include <QPoint>
#include <QLine>
#include <QPair>
#include <QMap>

typedef QPair<QLine, QLine> TLinesPair;
typedef QMap<QPoint, TLinesPair > TContourPoint;

class Contour
{
public:
    Contour();

    bool IsPointInContour(const TContourPoint& i_point) const;
    void AddContourPoint(const TContourPoint& i_point);
    QSet<TContourPoint> GetContour() const;

private:

    QSet<TContourPoint> m_contour;
};

#endif // CONTOUR_H
