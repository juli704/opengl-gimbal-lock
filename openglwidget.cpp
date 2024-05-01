#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL()
{
    // Load the OpenGL functionality
    Q_ASSERT(initializeOpenGLFunctions());

    // Set the clear color to dark-gray
    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
}

void OpenGLWidget::paintGL()
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    this->update();
}
