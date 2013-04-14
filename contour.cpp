#include "contour.h"

#include <stdexcept>

Contour::Contour()
    : m_is_closed(false)
{
}

bool Contour::IsPointInContour(const QPoint &i_point) const
{
    return m_points.indexOf(i_point) != -1;
}

bool Contour::AddPoint(const QPoint &i_point)
{
    if(m_is_closed)
        return false;

    if(!m_points.empty() && i_point == m_points[0])
    {
        m_is_closed = true;
        return true;
    }

    m_points.append(i_point);
    return true;
}


TPoints Contour::GetContourPoints() const
{
    return m_points;
}

bool Contour::IsClosed() const
{
    return m_is_closed;
}

bool Contour::operator ==(const Contour &i_other)
{
    return m_points == i_other.m_points;
}
