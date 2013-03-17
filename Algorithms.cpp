#include "Algorithms.h"

#include <QImage>
#include <QPair>

#include <algorithm>
#include <stdexcept>

namespace
{
    enum _BitColors
    {
        Black,
        White
    };

    typedef QPair<QPoint, QRgb> Pixel;


    QLine _ProcessPixel(Pixel i_pixel, Pixel i_neighbour_pixel)
    {
        if(i_pixel.second == i_neighbour_pixel.second)
            throw std::logic_error("Bad pixel");

        QPoint p1;
        QPoint p2;
        if(i_neighbour_pixel.first.x() == i_pixel.first.x() + 1)
        {
            p1 = QPoint(i_neighbour_pixel.first.x(), i_pixel.first.y());
            p2 = QPoint(p1 + QPoint(0, 1));
        }
        else if(i_neighbour_pixel.first.y() == i_pixel.first.y() + 1)
        {
            p1 = QPoint(i_pixel.first.x() + 1, i_pixel.first.y() + 1);
            p2 = QPoint(p1 - QPoint(1, 0));
        }
        else
            throw std::logic_error("Bad pixels");

        if(i_pixel.second  > i_neighbour_pixel.second)//White
            std::swap(p1, p2);

        return QLine(p1, p2);
    }

    Pixel _FormPixel(const QImage& i_image, int i, int j)
    {
        return qMakePair(QPoint(i, j), i_image.pixel(i, j));
    }

    void _ProcessPointsLines(PointsLines& io_points_lines, const QLine& i_line)
    {
        QPair<QLine, QLine> pair = io_points_lines[i_line.p1()];
        pair.second = i_line;
        io_points_lines[i_line.p1()] = pair;

        pair = io_points_lines[i_line.p2()];
        pair.first = i_line;
        io_points_lines[i_line.p2()] = pair;
    }

    //We must guarentee that the unique contour is exists
    void _ProcessContour(Lines& o_lines_to_delete, Lines& o_lines_to_insert, const PointsLines& i_points_lines
                         , LinesCombiner i_lines_combiner)
    {
        QLine begin_line = o_lines_to_delete.m_lines[0];
        QLine cur_line = begin_line;
        QLine first_line_to_insert;
        bool first_was_inserted = false;
        while(1)
        {
            QLine next_line = i_points_lines[cur_line.p2()].second;
            QLine combined_line = i_lines_combiner(cur_line, next_line);
            int idx_of_cur_line = o_lines_to_delete.m_lines.indexOf(cur_line);

            if(idx_of_cur_line != -1)
                o_lines_to_delete.m_lines.erase(o_lines_to_delete.m_lines.begin() + idx_of_cur_line);

            int idx_of_next_line = o_lines_to_delete.m_lines.indexOf(next_line);

            if(idx_of_next_line != -1)
                o_lines_to_delete.m_lines.erase(o_lines_to_delete.m_lines.begin() + idx_of_next_line);

            if(combined_line.isNull())
            {
                if(!first_was_inserted)
                {
                    first_was_inserted = true;
                    first_line_to_insert = cur_line;
                }
                else
                    o_lines_to_insert.m_lines.push_back(cur_line);

                cur_line = next_line;

                if(cur_line.p2() == begin_line.p1())
                {
                    QLine combined_line2 = i_lines_combiner(first_line_to_insert, next_line);
                    if(combined_line2.isNull())
                    {
                        o_lines_to_insert.m_lines.push_back(next_line);
                        o_lines_to_insert.m_lines.push_back(first_line_to_insert);
                    }
                    else
                        o_lines_to_insert.m_lines.push_back(combined_line2);

                    break;
                }
            }
            else
            {
                cur_line = combined_line;
                if(cur_line.p2() == begin_line.p1())
                {
                    QLine combined_line2 = i_lines_combiner(first_line_to_insert, combined_line);
                    if(combined_line2.isNull())
                    {
                        o_lines_to_insert.m_lines.push_back(combined_line);
                        o_lines_to_insert.m_lines.push_back(first_line_to_insert);
                    }
                    else
                        o_lines_to_insert.m_lines.push_back(combined_line2);

                    break;
                }
            }
        }
    }

    QPair<QRgb, QRgb> _BlackAndWhiteOnImage(const QImage& i_image)
    {
        QVector<QRgb> color_table = i_image.colorTable();

        if(color_table.size() != 2)
            throw std::logic_error("Bad image!");

        QRgb black = color_table[0] < color_table[1] ? color_table[0] : color_table[1];
        QRgb white = color_table[0] > color_table[1] ? color_table[0] : color_table[1];
        return qMakePair(black, white);
    }

    void _ProcessBorders(Lines& io_lines, PointsLines& io_points_lines, const QImage& i_image)
    {
        QRgb black = _BlackAndWhiteOnImage(i_image).first;

        for(int i = 0; i<i_image.width(); ++i)
        {
            if(i_image.pixel(i, 0) == black)
            {
                QLine line(QLine(QPoint(i, 0), QPoint(i+1, 0)));
                _ProcessPointsLines(io_points_lines, line);
                io_lines.m_lines.push_back(line);
            }

            if(i_image.pixel(i, i_image.height() - 1) == black)
            {
                QLine line(QLine(QPoint(i+1, i_image.height()), QPoint(i, i_image.height())));
                _ProcessPointsLines(io_points_lines, line);
                io_lines.m_lines.push_back(line);
            }
        }

        for(int i = 0; i<i_image.height(); ++i)
        {
            if(i_image.pixel(0, i) == black)
            {
                QLine line(QPoint(0, i+1), QPoint(0, i));
                _ProcessPointsLines(io_points_lines, line);
                io_lines.m_lines.push_back(QLine(line));
            }

            if(i_image.pixel(i_image.width() - 1, i) == black)
            {
                QLine line(QPoint(i_image.width(), i), QPoint(i_image.width(), i + 1));
                _ProcessPointsLines(io_points_lines, line);
                io_lines.m_lines.push_back(QLine(line));
            }
        }
    }

    bool _IsCrossQuadro(const QImage& i_image, int i, int j)
    {
        if(i_image.width() > i + 1 && i_image.height() > j + 1)
        {
            if(i_image.pixel(i, j) == i_image.pixel(i+1, j+1)
                    && i_image.pixel(i+1, j) == i_image.pixel(i, j+1)
                    && i_image.pixel(i, j) != i_image.pixel(i+1, j))
                return true;
            else
                return false;
        }
        else
            return false;
    }

    void _MakeBlackQuadro(QImage& io_image, int i, int j)
    {
        QRgb black = _BlackAndWhiteOnImage(io_image).first;
        io_image.setPixel(i, j, 0);
        io_image.setPixel(i+1, j, 0);
        io_image.setPixel(i, j+1, 0);
        io_image.setPixel(i+1, j+1, 0);
    }
}

QImage ImageToBitMap(const QImage& i_image, ColorPredicate i_color_predicate)
{
    QSize image_size = i_image.size();
    QImage bitmap(image_size, QImage::Format_Mono);

    for(int i = 0; i<image_size.width(); ++i)
        for(int j = 0; j<image_size.height(); ++j)
            bitmap.setPixel(i, j, i_color_predicate(i_image.pixel(i, j)));

    return bitmap;
}

void BitMapToLines(Lines &o_lines, PointsLines &o_points_lines, const QImage &i_image)
{
    if(i_image.format() != QImage::Format_Mono)
        throw std::logic_error("Bad image format");

    o_lines.m_lines.clear();
    o_points_lines.clear();
    o_lines.m_max_width = i_image.width();
    o_lines.m_max_height = i_image.height();

    for(int i = 0; i<i_image.width(); ++i)
    {
        for(int j = 0; j<i_image.height(); ++j)
        {
            if(j < i_image.height() - 1 && i_image.pixel(i, j) != i_image.pixel(i, j + 1))
            {
                QLine line(_ProcessPixel(_FormPixel(i_image, i, j), _FormPixel(i_image, i, j + 1)));
                _ProcessPointsLines(o_points_lines, line);
                o_lines.m_lines.push_back(line);
            }

            if(i < i_image.width() - 1 && i_image.pixel(i, j) != i_image.pixel(i + 1, j))
            {
                QLine line(_ProcessPixel(_FormPixel(i_image, i, j), _FormPixel(i_image, i + 1, j)));
                _ProcessPointsLines(o_points_lines, line);
                o_lines.m_lines.push_back(line);
            }
        }
    }

    _ProcessBorders(o_lines, o_points_lines, i_image);
}

void PreprocessBitMapCollision(QImage &io_image)
{
    for(int i = 0; i<io_image.width(); i+=1)
    {
        for(int j = 0; j<io_image.height(); j+=1)
        {
            if(_IsCrossQuadro(io_image, i, j))
                _MakeBlackQuadro(io_image, i, j);
        }
    }
}

Lines CombineLines(const Lines &i_lines, const PointsLines &i_points_lines, LinesCombiner i_lines_combiner)
{
    Lines current_lines = i_lines;
    Lines result_lines;

    while(current_lines.m_lines.size() > 0)
        _ProcessContour(current_lines, result_lines, i_points_lines, i_lines_combiner);

    return result_lines;
}

bool WhitePredicate(const QRgb& i_rgb)
{
    if(qGray(i_rgb) > 127)
        return White;

    return Black;
}

QLine DirectCombiner(const QLine &i_a, const QLine &i_b)
{
    if((i_a.dy() == 0 && i_b.dy() == 0) || 1.0*i_a.dx()/i_a.dy() == 1.0*i_b.dx()/i_b.dy())
    {
        if(i_a.p1() == i_b.p2())
            return QLine(i_b.p1(), i_a.p2());

        if(i_b.p1() == i_a.p2())
            return QLine(i_a.p1(), i_b.p2());
    }

    return QLine();
}

bool operator<(const QPoint& i_p1, const QPoint& i_p2)
{
    if(i_p1.x() < i_p2.x())
        return true;

    if(i_p1.x() == i_p2.x())
        return i_p1.y() < i_p2.y();

    return false;
}
