#include <QString>
#include <QtTest>

#include "contouralgorithms.h"

class Test : public QObject
{
    Q_OBJECT
    
public:
    Test();
    
private Q_SLOTS:
    void LinesToContoursTest();
    void LinesToContoursTest_data();

    void LinesToTLinkedPointsTest();
    void LinesToTLinkedPointsTest_data();

    void TLinkedPointsToContoursTest();
    void TLinkedPointsToContoursTest_data();
};

Test::Test()
{
}


void Test::LinesToContoursTest()
{
    QFETCH(Lines, lines);
    QFETCH(TContours, result);

    QCOMPARE(ContourAlgorithms::LinesToContours(lines), result);
}

void Test::LinesToContoursTest_data()
{
    QTest::addColumn<Lines>("lines");
    QTest::addColumn<TContours>("result");

    Lines lines;
    lines.m_lines.push_back(QLine(0,0,0,1));
    lines.m_lines.push_back(QLine(0,1,1,1));
    lines.m_lines.push_back(QLine(1,1,1,0));
    lines.m_lines.push_back(QLine(1,0,0,0));

    TContours contours;
    Contour contour;
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(1, 1));
    contour.AddPoint(QPoint(1, 0));
    contour.MakeClosed();
    contours.push_back(contour);

    QTest::newRow("1")<<lines<<contours;
}

void Test::LinesToTLinkedPointsTest()
{
    QFETCH(Lines, lines);
    QFETCH(TLinkedPoints, result);

    QCOMPARE(ContourAlgorithms::LinesToLinkedPoints(lines), result);
}

void Test::LinesToTLinkedPointsTest_data()
{
    QTest::addColumn<Lines>("lines");
    QTest::addColumn<TLinkedPoints>("result");

    Lines lines;
    lines.m_lines.push_back(QLine(0,0,0,1));
    lines.m_lines.push_back(QLine(0,1,1,1));
    lines.m_lines.push_back(QLine(1,1,1,0));
    lines.m_lines.push_back(QLine(1,0,0,0));

    TLinkedPoints linked_points;
    linked_points.insert(QPoint(0, 0), qMakePair(QPoint(1, 0), QPoint(0, 1)));
    linked_points.insert(QPoint(0, 1), qMakePair(QPoint(0, 0), QPoint(1, 1)));
    linked_points.insert(QPoint(1, 1), qMakePair(QPoint(0, 1), QPoint(1, 0)));
    linked_points.insert(QPoint(1, 0), qMakePair(QPoint(1, 1), QPoint(0, 0)));

    QTest::newRow("1")<<lines<<linked_points;
}

void Test::TLinkedPointsToContoursTest()
{
    QFETCH(TLinkedPoints, linked_points);
    QFETCH(TContours, result);

    QCOMPARE(ContourAlgorithms::LinkedPointsToContours(linked_points), result);
}

void Test::TLinkedPointsToContoursTest_data()
{
    QTest::addColumn<TLinkedPoints>("linked_points");
    QTest::addColumn<TContours>("result");

    TLinkedPoints linked_points;
    linked_points.insert(QPoint(0, 0), qMakePair(QPoint(1, 0), QPoint(0, 1)));
    linked_points.insert(QPoint(0, 1), qMakePair(QPoint(0, 0), QPoint(1, 1)));
    linked_points.insert(QPoint(1, 1), qMakePair(QPoint(0, 1), QPoint(1, 0)));
    linked_points.insert(QPoint(1, 0), qMakePair(QPoint(1, 1), QPoint(0, 0)));

    TContours contours;
    Contour contour;
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(1, 1));
    contour.AddPoint(QPoint(1, 0));
    contour.MakeClosed();
    contours.push_back(contour);

    QTest::newRow("1")<<linked_points<<contours;
}

QTEST_APPLESS_MAIN(Test)

#include "test.moc"
