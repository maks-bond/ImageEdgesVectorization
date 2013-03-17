#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Algorithms.h"
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
        BitMapToLines(m_lines, m_points_lines, m_bitmap_image);
        _DrawLines(m_lines_image);
    }
}

void MainWindow::OnCombineLines()
{
    if(m_lines.m_lines.isEmpty())
        return;

    m_lines = CombineLines(m_lines, m_points_lines, &DirectCombiner);
    _DrawLines(m_combined_lines_image);
}

void MainWindow::_DrawLines(QImage& i_image)
{
    DrawLines(i_image, m_lines);
    _SetUpImage(i_image);
    ui->editLines->setText(QString::number(m_lines.m_lines.size()));
}
