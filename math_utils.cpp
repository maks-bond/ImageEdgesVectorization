#include "math_utils.h"

#include <cmath>
#include <stdexcept>

#include <QLine>
#include <QPoint>

namespace
{
    const double g_pi = acos(-1.0);
    const double epsilon = 1e-2;
}

QVector<double> Math::FormGaussCoeffs(double i_deviation, int i_number_of_coeffs)
{
    if(i_number_of_coeffs%2 == 0)
        throw std::logic_error("Bad!");

    QVector<double> res;
    QVector<double> not_norm_res;
    QVector<int> indexes = FormIndexes(i_number_of_coeffs);

    double sum = 0;

    for(int i = 0; i < indexes.size(); ++i)
    {
        double gauss = _Gauss(i_deviation, indexes[i]);
        sum += gauss;
        not_norm_res.push_back(gauss);
    }


    for(int i = 0; i < not_norm_res.size(); ++i)
        res.push_back(not_norm_res[i]/sum);

    return res;
}

double Math::_Gauss(double i_deviation, double x)
{
    return exp(-(x*x)/(2*i_deviation*i_deviation))/(i_deviation*sqrt(2*g_pi));
}

QVector<int> Math::FormIndexes(int i_number)
{
    QVector<int> result;

    for(int i = -(i_number/2); i <= (i_number/2); ++i)
        result.push_back(i);

    return result;
}

double Math::FromGradusToRadian(double i_gradus)
{
    return g_pi*i_gradus/180;
}

double Math::FromRadianToGradus(double i_radian)
{
    return i_radian*180/g_pi;
}

double Math::LineAngleInGradus(const QPoint &i_a, const QPoint &i_b)
{
    QLine line(i_a, i_b);

    if(line.dx() == 0)
        return 90;

    double tangens = line.dy()*1.0/line.dx();
    double angle_in_radian = atan(tangens);
    return FromRadianToGradus(angle_in_radian);
}

bool Math::DoesLinesWithSameAngle(const QPoint &i_a, const QPoint &i_b, const QPoint &i_c, double i_deviation)
{
    double angle1 = LineAngleInGradus(i_a, i_b);
    double angle2 = LineAngleInGradus(i_b, i_c);

    return double_equal(fabs(fabs(angle1 - angle2) - i_deviation), 0)
            || fabs(angle1 - angle2) < i_deviation;
}

double Math::Distance(const QPoint &i_a, const QPoint &i_b)
{
    QLine line(i_a, i_b);

    return sqrt(line.dx()*line.dx() + line.dy()*line.dy());
}

Math::Math()
{
}


bool Math::double_equal(double i_x, double i_y)
{
    return fabs(i_x - i_y) < epsilon;
}
