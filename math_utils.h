#ifndef MATH_H
#define MATH_H

#include <QVector>

class Math
{
public:
    static QVector<double> FormGaussCoeffs(double i_deviation, int i_number_of_coeffs);
    static QVector<int> FormIndexes(int i_number);

private:
    static double _Gauss(double i_deviation, double x);

    Math();
};

#endif // MATH_H
