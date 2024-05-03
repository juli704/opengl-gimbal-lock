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

public slots:

    void setOuterRingAngle(const int angle);
    void setMiddleRingAngle(const int angle);
    void setInnerRingAngle(const int angle);

private:

    Model* outerRingModel;
    Model* middleRingModel;
    Model* innerRingModel;

    QMatrix4x4 outerRingRotationMatrix;
    QMatrix4x4 middleRingRotationMatrix;
    QMatrix4x4 innerRingRotationMatrix;

    QMatrix4x4 viewMatrix;

    int outerRingAngle;
    int middleRingAngle;
    int innerRingAngle;
};

#endif // OPENGLWIDGET_H
