#ifndef CONTOURALGORITHMS_H
#define CONTOURALGORITHMS_H

#include "contour.h"
#include "DataStructures.h"

class Lines;

class ContourAlgorithms
{
public:
    static TLinkedPoints LinesToLinkedPoints(const Lines& i_lines);
    static TContours LinkedPointsToContours(const TLinkedPoints& i_linked_points);

    static TContours LinesToContours(const Lines& i_lines);

private:
    ContourAlgorithms();
};

#endif // CONTOURALGORITHMS_H
