#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL()
{
    // Load OpenGL functionality
    Q_ASSERT(initializeOpenGLFunctions());

    // Set scene's clear color to light gray
    glClearColor(0.9f, 0.9f, 0.9f, 0.5f);

    // Create ring models
    const QString objectFilePath = ":/objects/ring.obj";
    this->outerRingModel = new Model(objectFilePath, ":/fragshader/mint.frag");
    this->middleRingModel = new Model(objectFilePath, ":/fragshader/storm.frag");
    this->innerRingModel = new Model(objectFilePath, ":/fragshader/beige.frag");

    // Mount camera
    this->viewMatrix.lookAt(
        QVector3D(0.0, 0.0, 10.0), // Eye
        QVector3D(0.0, 0.0, 0.0),  // Focal Point
        QVector3D(0.0, 0.1, 0.0)   // Up vector
    );
}

void OpenGLWidget::paintGL()
{
    // Clear screen //

    glClear(GL_COLOR_BUFFER_BIT);


    // Calculate current projection matrix //

    const float aspectRatio = float(this->width()) / float(this->height());
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(
        45,             // Vertical angle
        aspectRatio,    // Aspect ratio
        0.1,            // Near plane
        1000.0          // Far plane
    );


    // Calculate ring rotations //

    this->outerRingRotationMatrix = QMatrix4x4();
    this->outerRingRotationMatrix.rotate(50, QVector3D(1,0,0));

    this->middleRingRotationMatrix = QMatrix4x4();
    this->middleRingRotationMatrix.rotate(50, QVector3D(0,1,0));
    this->middleRingRotationMatrix = this->outerRingRotationMatrix * this->middleRingRotationMatrix;

    this->innerRingRotationMatrix = QMatrix4x4();
    this->innerRingRotationMatrix.rotate(50, QVector3D(1,0,0));
    this->innerRingRotationMatrix = this->middleRingRotationMatrix * this->innerRingRotationMatrix;


    // Draw to scene //

    this->outerRingModel->draw(projectionMatrix, this->viewMatrix, this->outerRingRotationMatrix, 3.0);
    this->middleRingModel->draw(projectionMatrix, this->viewMatrix, this->middleRingRotationMatrix, 1.9);
    this->innerRingModel->draw(projectionMatrix, this->viewMatrix, this->innerRingRotationMatrix, 1.0);

    this->update();
}
