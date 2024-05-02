#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "model.h"

#include <QOpenGLFunctions_3_3_Core>
#include <QtOpenGLWidgets/QOpenGLWidget>

class OpenGLWidget : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
public:

    OpenGLWidget(QWidget *parent);
    ~OpenGLWidget();

    void initializeGL() override;
    void paintGL() override;

private:

    Model* outerRingModel;
    Model* middleRingModel;
    Model* innerRingModel;

    QMatrix4x4 outerRingRotationMatrix;
    QMatrix4x4 middleRingRotationMatrix;
    QMatrix4x4 innerRingRotationMatrix;

    QMatrix4x4 viewMatrix;
};

#endif // OPENGLWIDGET_H
