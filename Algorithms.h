#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "DataStructures.h"

#include <QImage>
#include <QLine>

class QPoint;

typedef bool ColorPredicate(const QRgb& i_rgb);

QImage ImageToBitMap(const QImage& i_image, ColorPredicate i_color_predicate);
void BitMapToLines(Lines &o_lines, const QImage &i_image);
void PreprocessBitMapCollision(QImage& io_image);

bool WhitePredicate(const QRgb& i_rgb);

#endif // ALGORITHMS_H
