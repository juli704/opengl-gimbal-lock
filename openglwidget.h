#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class OpenGLWidget : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
public:

    OpenGLWidget(QWidget* parent);
    ~OpenGLWidget();

    void initializeGL() override;
    void paintGL() override;
};
