#include "Functors.h"

#include "math_utils.h"

#include <QPoint>
#include <QLine>


bool AngleFunctor::operator ()(const QPoint &i_a, const QPoint &i_b, const QPoint &i_c) const
{
    return Math::DoesLinesWithSameAngle(i_a, i_b, i_c, m_angle);
}

bool LengthFunctor::operator ()(const QPoint &i_a, const QPoint &i_b, const QPoint &i_c) const
{
    return Math::Distance(i_a, i_b) <= m_length || Math::Distance(i_b, i_c) <= m_length;
}
