#include "Rendering.h"

#include <QPainter>

namespace
{
    void _DrawContour(QImage& o_image, const Contour& i_contour)
    {
        if(i_contour.GetContourPoints().size() < 2)
            return;

        QPainter painter(&o_image);
        TPoints points = i_contour.GetContourPoints();

        for(int i = 0; i < points.size() - 1; ++i)
        {
            painter.drawLine(points[i]*2, points[i+1]*2);
        }

        if(i_contour.IsClosed())
            painter.drawLine(points.back()*2, points.first()*2);
    }
}

void DrawLines(QImage& o_image, const Lines& i_lines)
{
    QSize wide_size(i_lines.m_max_width*2+1, i_lines.m_max_height*2+1);
    o_image = QImage(wide_size, QImage::Format_ARGB32);
    o_image.fill(Qt::white);

    QPainter painter(&o_image);

    for(int i = 0; i < i_lines.m_lines.size(); ++i)
    {
        QPoint p1(i_lines.m_lines[i].p1().x()*2, i_lines.m_lines[i].p1().y()*2);
        QPoint p2(i_lines.m_lines[i].p2().x()*2, i_lines.m_lines[i].p2().y()*2);
        painter.drawLine(p1, p2);
    }
}

void DrawContours(QImage &o_image, const TContours &i_contours, int i_max_height, int i_max_width)
{
    QSize wide_size(i_max_width*2+1, i_max_height*2+1);
    o_image = QImage(wide_size, QImage::Format_ARGB32);
    o_image.fill(Qt::white);

    for(int i = 0; i < i_contours.size(); ++i)
    {
        _DrawContour(o_image, i_contours[i]);
    }
}
