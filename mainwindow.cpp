#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Algorithms.h"
#include "contouralgorithms.h"
#include "Rendering.h"

#include <QResizeEvent>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OnOpen()));
    connect(ui->buttonToMask, SIGNAL(clicked()), this, SLOT(OnToMask()));
    connect(ui->buttonFormLines, SIGNAL(clicked()), this, SLOT(OnFormLines()));
    connect(ui->buttonCombineLines, SIGNAL(clicked()), this, SLOT(OnCombineLines()));
}

void MainWindow::resizeEvent(QResizeEvent *ip_event)
{
    QWidget::resizeEvent(ip_event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnOpen()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open image"), ".", "Image (*.jpg *.bmp)");

    if(!file_name.isEmpty())
    {
        m_original_image.load(file_name);
        _SetUpImage(m_original_image);
    }
}

void MainWindow::OnToMask()
{
    m_bitmap_image = ImageToBitMap(m_original_image, &WhitePredicate);
    PreprocessBitMapCollision(m_bitmap_image);
    _SetUpImage(m_bitmap_image);
}

void MainWindow::_SetUpImage(const QImage& i_image)
{
    QPixmap image = QPixmap::fromImage(i_image);
    //QPixmap image = QPixmap::fromImage(i_image.scaled(ui->label->size(), Qt::KeepAspectRatio));
    ui->label->setPixmap(image);
}

void MainWindow::OnFormLines()
{
    if(!m_bitmap_image.isNull())
    {
        Lines lines;
        BitMapToLines(lines, m_bitmap_image);
        m_max_height = lines.m_max_height;
        m_max_width = lines.m_max_width;
        m_contours = ContourAlgorithms::LinesToContours(lines);
        _DrawContours(m_contours_image);
    }
}

void MainWindow::OnCombineLines()
{
    ContourAlgorithms::CombineLinesInContours(m_contours);
    _DrawContours(m_contours_image);
}

void MainWindow::_DrawContours(QImage& i_image)
{
    DrawContours(i_image, m_contours, m_max_height, m_max_width);
    _SetUpImage(i_image);
    int number_of_points = 0;

    for(int i = 0; i<m_contours.size(); ++i)
        number_of_points+=m_contours[i].GetContourPoints().size();

    ui->editLines->setText(QString::number(number_of_points));
}
