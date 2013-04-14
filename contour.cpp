#include "contour.h"

#include <stdexcept>

Contour::Contour()
{
}

bool Contour::IsPointInContour(const QPoint &i_point) const
{
    return m_contour.constFind(i_point) != m_contour.constEnd();
}

void Contour::AddContourPoint(const TContourPoint &i_point)
{
    if(IsPointInContour(i_point.first))
        throw std::logic_error("Point is in contour!!!");

    m_contour.insert(i_point.first, i_point.second);
}

TContour Contour::GetContour() const
{
    return m_contour;
}

