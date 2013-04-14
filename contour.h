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

private:

    QSet<TContourPoint> m_contour;
};

#endif // CONTOUR_H
