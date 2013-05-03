#ifndef MATH_H
#define MATH_H

#include <QVector>

class QPoint;

class Math
{
public:
    static QVector<double> FormGaussCoeffs(double i_deviation, int i_number_of_coeffs);
    static QVector<int> FormIndexes(int i_number);

    static double FromGradusToRadian(double i_gradus);
    static double FromRadianToGradus(double i_radian);

    static double LineAngleInGradus(const QPoint& i_a, const QPoint& i_b);

private:
    static double _Gauss(double i_deviation, double x);

    Math();
};

#endif // MATH_H
