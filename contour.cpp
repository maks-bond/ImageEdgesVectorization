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
        return false;
    }

    m_points.append(i_point);
    return true;
}

void Contour::Clear()
{
    m_is_closed = false;
    m_points.clear();
}

const TPoints& Contour::GetContourPoints() const
{
    return m_points;
}

void Contour::MakeClosed()
{
    m_is_closed = true;
}

void Contour::MakeUnclosed()
{
    m_is_closed = false;
}

void Contour::SetContourPoints(const TPoints &i_points)
{
    m_points = i_points;
}

bool Contour::IsClosed() const
{
    return m_is_closed;
}

bool Contour::IsEmpty() const
{
    return m_points.empty();
}

/*bool Contour::operator ==(const Contour &i_other)
{
    if(m_is_closed && i_other.m_is_closed)
        return m_points == i_other.m_points;

    return m_points == i_other.m_points && m_begin_point == i_other.m_begin_point && m_is_closed == i_other.m_is_closed;
}*/

/*unsigned qHash(const QPoint& i_point)
{
    return qHash(i_point.x()) ^ qHash(i_point.y());
}*/
