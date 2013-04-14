#ifndef CONTOURALGORITHMS_H
#define CONTOURALGORITHMS_H

#include "contour.h"

class Lines;

class ContourAlgorithms
{
public:
    static TContours LinesToContours(const Lines& i_lines);

private:
    ContourAlgorithms();
};

#endif // CONTOURALGORITHMS_H
