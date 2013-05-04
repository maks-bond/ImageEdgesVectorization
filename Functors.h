#ifndef FUNCTORS_H
#define FUNCTORS_H

class QPoint;

class IFunctor
{
public:
    virtual bool operator() (const QPoint& i_a, const QPoint& i_b, const QPoint& i_c) const = 0;
    virtual ~IFunctor(){}
};

class AngleFunctor : public IFunctor
{
public:
    AngleFunctor(double i_angle) : m_angle(i_angle){}
    virtual ~AngleFunctor(){}

    bool operator() (const QPoint& i_a, const QPoint& i_b, const QPoint& i_c) const;

private:
    double m_angle;
};

class LengthFunctor : public IFunctor
{
public:
    LengthFunctor(double i_length) : m_length(i_length){}
    virtual ~LengthFunctor(){}

    bool operator() (const QPoint& i_a, const QPoint& i_b, const QPoint& i_c) const;

private:
    double m_length;
};

#endif // FUNCTORS_H
