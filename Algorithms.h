#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "DataStructures.h"

#include <QImage>
#include <QLine>

class QPoint;

typedef bool ColorPredicate(const QRgb& i_rgb);
typedef  QLine LinesCombiner(const QLine &i_a, const QLine &i_b);

QImage ImageToBitMap(const QImage& i_image, ColorPredicate i_color_predicate);
void BitMapToLines(Lines &o_lines, PointsLines &o_points_lines, const QImage &i_image);
void PreprocessBitMapCollision(QImage& io_image);

Lines CombineLines(const Lines &i_lines, const PointsLines &i_points_lines, LinesCombiner i_lines_combiner);

bool WhitePredicate(const QRgb& i_rgb);

QLine DirectCombiner(const QLine &i_a, const QLine &i_b);

#endif // ALGORITHMS_H
