#ifndef MATH_H
#define MATH_H

#include <QVector>

class Math
{
public:
    static QVector<double> FormGaussCoeffs(double i_deviation, int i_number_of_coeffs);

private:
    static double _Gauss(double i_deviation, double x);
    static QVector<int> _FormIndexes(int i_number);

    Math();
};

#endif // MATH_H
