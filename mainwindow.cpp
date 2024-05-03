#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->outerRingSlider, &QSlider::sliderMoved, ui->openGLWidget, &OpenGLWidget::setOuterRingAngle);
    connect(ui->middleRingSlider, &QSlider::sliderMoved, ui->openGLWidget, &OpenGLWidget::setMiddleRingAngle);
    connect(ui->innerRingSlider, &QSlider::sliderMoved, ui->openGLWidget, &OpenGLWidget::setInnerRingAngle);
}

MainWindow::~MainWindow()
{
    delete ui;
}
