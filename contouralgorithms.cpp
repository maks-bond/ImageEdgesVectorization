#include "contouralgorithms.h"

#include <stdexcept>

namespace
{
    bool _DoesPointsOnOneLine(const QPoint& i_a, const QPoint& i_b, const QPoint& i_c)
    {
        QLine line1(i_a, i_b);
        QLine line2(i_b, i_c);
        if((line1.dy() == 0 && line2.dy() == 0) || 1.0*line1.dx()/line1.dy() == 1.0*line2.dx()/line2.dy())
            return true;

        return false;
    }
}

ContourAlgorithms::ContourAlgorithms()
{
}

TContours ContourAlgorithms::LinesToContours(const Lines &i_lines)
{
    return _LinkedPointsToContours(_LinesToLinkedPoints(i_lines));
}

void ContourAlgorithms::CombineLinesInContours(TContours &i_contours)
{
    for(int i = 0; i<i_contours.size(); ++i)
    {
        _CombineLinesInContour(i_contours[i]);
    }
}


TLinkedPoints ContourAlgorithms::_LinesToLinkedPoints(const Lines &i_lines)
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

TContours ContourAlgorithms::_LinkedPointsToContours(const TLinkedPoints &i_linked_points)
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
            current_point = linked_points.begin()->first;
            current_contour.AddPoint(current_point);
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

void ContourAlgorithms::_CombineLinesInContour(Contour &i_contour)
{
    TPoints points = i_contour.GetContourPoints();

    if(points.size() <= 2)
        return;

    int i = 0, j = 1, k = 2;

    while(k < points.size())
    {
        if(_DoesPointsOnOneLine(points[i], points[j], points[k]))
            points.remove(j);
        else
            ++i, ++j, ++k;
    }

    if(points.size() >2 && i_contour.IsClosed())
    {
        if(_DoesPointsOnOneLine(points[points.size() - 2], points[points.size() - 1], points[0]))
            points.pop_back();

        if(points.size() > 2 &&
                _DoesPointsOnOneLine(points[points.size() - 1], points[0], points[1]))
            points.pop_front();
    }

    if(points.size() < i_contour.GetContourPoints().size())
        i_contour.SetContourPoints(points);
}
