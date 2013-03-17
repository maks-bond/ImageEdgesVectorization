#include "Rendering.h"

#include <QPainter>

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

