#ifndef RENDERING_H
#define RENDERING_H

#include "Contour.h"
#include "DataStructures.h"

#include <QImage>

void DrawLines(QImage& o_image, const Lines& i_lines);
void DrawContours(QImage& o_image, const TContours& i_contours, int i_max_height, int i_max_width);

#endif // RENDERING_H
