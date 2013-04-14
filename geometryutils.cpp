#include "geometryutils.h"

#include <QPoint>

bool operator<(const QPoint& i_p1, const QPoint& i_p2)
{
    if(i_p1.x() < i_p2.x())
        return true;

    if(i_p1.x() == i_p2.x())
        return i_p1.y() < i_p2.y();

    return false;
}

/*bool operator<(const QLine& i_a, const QLine& i_b)
{
    if(i_a.p1() != i_b.p1())
        return i_a.p1() < i_b.p1();

    if(i_a.p2() != i_b.p2())
        return i_a.p2() < i_b.p2();

    return false;
}*/

/*bool operator<(const TLinesPair& i_a, const TLinesPair& i_b)
{
    if(i_a.p1() != i_b.p1())
        return i_a.first < i_b.first;

    if(i_a.p2() != i_b.p2())
        return i_a.second < i_b.second;

    return false;
}*/
