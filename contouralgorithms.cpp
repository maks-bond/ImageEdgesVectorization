#include "contouralgorithms.h"

#include <stdexcept>

ContourAlgorithms::ContourAlgorithms()
{
}

TContours ContourAlgorithms::LinesToContours(const Lines &i_lines)
{
    return LinkedPointsToContours(LinesToLinkedPoints(i_lines));
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
    if(i_linked_points.empty())
        return TContours();

    TLinkedPoints linked_points = i_linked_points;
    TContours contours;
    Contour current_contour;
    QPoint current_point = linked_points.cbegin()->first;

    while(!linked_points.empty())
    {
        if(!current_contour.AddPoint(current_point))
        {
            contours.push_back(current_contour);
            current_contour.Clear();
        }

        QPoint prev_point = current_point;

        if(linked_points.constFind(current_point) == linked_points.cend())
            throw std::logic_error("Bad linked points");

        current_point = linked_points[current_point].second;

        if(linked_points.constFind(current_point) != linked_points.cend()
                && prev_point != linked_points[current_point].first)
            throw std::logic_error("Bad linked points");

        linked_points.remove(prev_point);
    }

    if(!current_contour.IsEmpty())
    {
        current_contour.MakeClosed();
        contours.push_back(current_contour);
    }

    return contours;
}
