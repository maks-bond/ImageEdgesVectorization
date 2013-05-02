#include "Algorithms.h"

#include <QImage>
#include <QPair>

#include <algorithm>
#include <stdexcept>
#include <QFile>
#include <QTextStream>
#include <iostream>

#include "geometryutils.h"

namespace
{
    enum _BitColors
    {
        Black,
        White
    };

    typedef QPair<QPoint, QRgb> Pixel;

    void _WriteImageToFile(const QImage& i_image, const QString& i_file)
    {
        QFile file(i_file);
        file.open(QIODevice::WriteOnly);

        QTextStream out(&file);
        for(int i = 0; i<i_image.width(); ++i)
        {
            for(int j = 0; j<i_image.height(); ++j)
            {
                out<<i<<" "<<j<<" "<<i_image.pixel(i, j)<<'\n';
            }
        }
    }

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

    QPair<QRgb, QRgb> _BlackAndWhiteOnImage(const QImage& i_image)
    {
        QVector<QRgb> color_table = i_image.colorTable();

        if(color_table.size() != 2)
            throw std::logic_error("Bad image!");

        QRgb black = color_table[0] < color_table[1] ? color_table[0] : color_table[1];
        QRgb white = color_table[0] > color_table[1] ? color_table[0] : color_table[1];
        return qMakePair(black, white);
    }

    void _ProcessBorders(Lines& io_lines, const QImage& i_image)
    {
        QRgb black = _BlackAndWhiteOnImage(i_image).first;

        for(int i = 0; i<i_image.width(); ++i)
        {
            if(i_image.pixel(i, 0) == black)
            {
                QLine line(QLine(QPoint(i, 0), QPoint(i+1, 0)));
                io_lines.m_lines.push_back(line);
            }

            if(i_image.pixel(i, i_image.height() - 1) == black)
            {
                QLine line(QLine(QPoint(i+1, i_image.height()), QPoint(i, i_image.height())));
                io_lines.m_lines.push_back(line);
            }
        }

        for(int i = 0; i<i_image.height(); ++i)
        {
            if(i_image.pixel(0, i) == black)
            {
                QLine line(QPoint(0, i+1), QPoint(0, i));
                io_lines.m_lines.push_back(QLine(line));
            }

            if(i_image.pixel(i_image.width() - 1, i) == black)
            {
                QLine line(QPoint(i_image.width(), i), QPoint(i_image.width(), i + 1));
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

void BitMapToLines(Lines &o_lines, const QImage &i_image)
{
    if(i_image.format() != QImage::Format_Mono)
        throw std::logic_error("Bad image format");

    o_lines.m_lines.clear();
    o_lines.m_max_width = i_image.width();
    o_lines.m_max_height = i_image.height();

    for(int i = 0; i<i_image.width(); ++i)
    {
        for(int j = 0; j<i_image.height(); ++j)
        {
            if(j < i_image.height() - 1 && i_image.pixel(i, j) != i_image.pixel(i, j + 1))
            {
                QLine line(_ProcessPixel(_FormPixel(i_image, i, j), _FormPixel(i_image, i, j + 1)));
                o_lines.m_lines.push_back(line);
            }

            if(i < i_image.width() - 1 && i_image.pixel(i, j) != i_image.pixel(i + 1, j))
            {
                QLine line(_ProcessPixel(_FormPixel(i_image, i, j), _FormPixel(i_image, i + 1, j)));
                o_lines.m_lines.push_back(line);
            }
        }
    }

    _ProcessBorders(o_lines, i_image);
}

void PreprocessBitMapCollision(QImage &io_image)
{
    bool was_collision = false;

    do
    {
        was_collision = false;
        for(int i = 0; i<io_image.width(); i+=1)
        {
            for(int j = 0; j<io_image.height(); j+=1)
            {
                if(_IsCrossQuadro(io_image, i, j))
                {
                    _MakeBlackQuadro(io_image, i, j);
                    was_collision = true;
                }
            }
        }
    } while(was_collision);
}

bool WhitePredicate(const QRgb& i_rgb)
{
    if(qGray(i_rgb) > 127)
        return White;

    return Black;
}
