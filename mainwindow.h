#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contour.h"
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
    void _DrawContours(QImage& i_image);

    Ui::MainWindow *ui;
    QImage m_original_image;
    QImage m_bitmap_image;
    QImage m_contours_image;
    QImage m_combined_lines_image;
    TContours m_contours;
    int m_max_height;
    int m_max_width;
};

#endif // MAINWINDOW_H
