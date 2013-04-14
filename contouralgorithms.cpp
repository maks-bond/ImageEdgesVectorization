#include "contouralgorithms.h"

#include <stdexcept>

ContourAlgorithms::ContourAlgorithms()
{
}

TContours ContourAlgorithms::LinesToContours(const Lines &i_lines)
{
    return TContours();
}


TLinkedPoints ContourAlgorithms::LinesToLinkedPoints(const Lines &i_lines)
{
    TLinkedPoints linked_points;


    for(int i = 0; i<i_lines.m_lines.size(); ++i)
    {
        if((!i_lines.m_lines[i].p2().isNull() && !linked_points[i_lines.m_lines[i].p1()].second.isNull())
                || (!i_lines.m_lines[i].p1().isNull() && !linked_points[i_lines.m_lines[i].p2()].first.isNull()))
            throw std::logic_error("Bad!");

        linked_points[i_lines.m_lines[i].p1()].second = i_lines.m_lines[i].p2();
        linked_points[i_lines.m_lines[i].p2()].first = i_lines.m_lines[i].p1();
    }

    return linked_points;
}

TContours ContourAlgorithms::LinkedPointsToContours(const TLinkedPoints &i_linked_points)
{
    return TContours();
}
