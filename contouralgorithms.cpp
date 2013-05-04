#include "contouralgorithms.h"

#include "math_utils.h"

#include <QFile>
#include <QTextStream>

#include <cmath>
#include <stdexcept>

namespace
{
    double math_round(double i_x)
    {
        return std::floor(i_x + 0.5);
    }

    void _WriteCoeffs(const QVector<double>& i_coeffs, const QString& i_file)
    {
        QFile file(i_file);
        file.open(QFile::WriteOnly);
        QTextStream out(&file);

        for(int i = 0; i < i_coeffs.size(); ++i)
            out<<QString::number(i_coeffs[i])<<" ";

        file.close();
    }
}

ContourAlgorithms::ContourAlgorithms()
{
}

void ContourAlgorithms::ApplyGauss(TContours &i_contours, double i_deviation, int i_number_of_coeffs)
{
    for(int i = 0; i<i_contours.size(); ++i)
        _ApplyGauss(i_contours[i], i_deviation, i_number_of_coeffs);
}

TContours ContourAlgorithms::LinesToContours(const Lines &i_lines)
{
    return _LinkedPointsToContours(_LinesToLinkedPoints(i_lines));
}

void ContourAlgorithms::CombineLinesInContours(TContours &i_contours, const IFunctor* const i_functor)
{
    for(int i = 0; i<i_contours.size(); ++i)
    {
        _CombineLinesInContour(i_contours[i], i_functor);
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

void ContourAlgorithms::_CombineLinesInContour(Contour &i_contour, const IFunctor* const i_functor)
{
    TPoints points = i_contour.GetContourPoints();

    if(points.size() <= 2)
        return;

    int i = 0, j = 1, k = 2;

    while(k < points.size())
    {
        if((*i_functor)(points[i], points[j], points[k]))
            points.remove(j);
        else
            ++i, ++j, ++k;
    }

    if(points.size() >2 && i_contour.IsClosed())
    {
        if((*i_functor)(points[points.size() - 2], points[points.size() - 1], points[0]))
            points.pop_back();

        if(points.size() > 2 &&
                (*i_functor)(points[points.size() - 1], points[0], points[1]))
            points.pop_front();
    }

    if(points.size() < i_contour.GetContourPoints().size())
        i_contour.SetContourPoints(points);
}

void ContourAlgorithms::_ApplyGauss(Contour &i_contour, double i_deviation, int i_number_of_coeffs)
{
    //if(i_contour.GetContourPoints().size() <= i_number_of_coeffs)
      //  return;

    if(!i_contour.IsClosed())
        throw std::logic_error("Bad!");

    QVector<double> coeffs(Math::FormGaussCoeffs(i_deviation, i_number_of_coeffs));
    QVector<int> indexes(Math::FormIndexes(i_number_of_coeffs));

    //Write temp
    static bool was_written = false;

    if(!was_written)
    {
        _WriteCoeffs(coeffs, "a.out");
        was_written = true;
    }

    TPoints new_points;
    TPoints points = i_contour.GetContourPoints();

    for(int i = 0; i < points.size(); ++i)
    {
        QPointF new_float_point(0, 0);

        for(int j = 0; j<indexes.size(); ++j)
        {
            int index = (i+indexes[j]+10*points.size())%points.size();
            new_float_point += QPointF(points[index])*coeffs[j];
        }

        new_points.push_back(QPoint(math_round(new_float_point.x()), math_round(new_float_point.y())));
    }

    i_contour.SetContourPoints(new_points);
}
