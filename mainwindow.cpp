#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Algorithms.h"
#include "contouralgorithms.h"
#include "Functors.h"
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
    connect(ui->buttonGauss, SIGNAL(clicked()), this, SLOT(OnApplyGauss()));
    connect(ui->buttonBack, SIGNAL(clicked()), this, SLOT(OnBack()));
    connect(ui->buttonCombine, SIGNAL(clicked()), this, SLOT(OnCombine()));
    connect(ui->buttonCombineLength, SIGNAL(clicked()), this, SLOT(OnCombineLength()));
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

void MainWindow::OnApplyGauss()
{
    m_prev_image = m_contours_image;
    m_prev_contours = m_contours;
    ContourAlgorithms::ApplyGauss(m_contours, ui->spinDeviation->value(), ui->spinValues->value());
    _DrawContours(m_contours_image);
}

void MainWindow::OnBack()
{
    if(!m_prev_image.isNull() && !m_prev_contours.empty())
    {
        m_contours = m_prev_contours;
        m_contours_image = m_prev_image;
        _SetUpImage(m_prev_image);
        m_prev_image = QImage();
        m_prev_contours.clear();
    }
}

void MainWindow::OnCombine()
{
    m_prev_contours = m_contours;
    m_prev_image = m_contours_image;
    AngleFunctor* p_functor = new AngleFunctor(ui->spinAngle->value());
    ContourAlgorithms::CombineLinesInContours(m_contours, p_functor);
    delete p_functor;
    _DrawContours(m_contours_image);
}

void MainWindow::OnCombineLength()
{
    m_prev_contours = m_contours;
    m_prev_image = m_contours_image;
    LengthFunctor* p_functor = new LengthFunctor(ui->spinLength->value());
    ContourAlgorithms::CombineLinesInContours(m_contours, p_functor);
    delete p_functor;
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
