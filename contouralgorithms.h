#ifndef CONTOURALGORITHMS_H
#define CONTOURALGORITHMS_H

#include "contour.h"
#include "DataStructures.h"

class Lines;

class ContourAlgorithms
{
public:
    static TContours LinesToContours(const Lines& i_lines);
    static void CombineLinesInContours(TContours& i_contours);

private:
    ContourAlgorithms();

    static TLinkedPoints _LinesToLinkedPoints(const Lines& i_lines);
    static TContours _LinkedPointsToContours(const TLinkedPoints& i_linked_points);
    static void _CombineLinesInContour(Contour& i_contour);
};

#endif // CONTOURALGORITHMS_H
