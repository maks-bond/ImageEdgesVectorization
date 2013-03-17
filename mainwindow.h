#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataStructures.h"

#include <QMainWindow>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    virtual void resizeEvent(QResizeEvent *ip_event);

private slots:
    void OnOpen();
    void OnToMask();
    void OnFormLines();
    void OnCombineLines();

private:
    void _SetUpImage(const QImage& i_image);
    void _DrawLines(QImage& i_image);

    Ui::MainWindow *ui;
    QImage m_original_image;
    QImage m_bitmap_image;
    QImage m_lines_image;
    QImage m_combined_lines_image;
    Lines m_lines;
    PointsLines m_points_lines;
};

#endif // MAINWINDOW_H
