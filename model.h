#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QString>

class Model : QOpenGLFunctions
{
public:

    Model(
        const QString& objectFilePath,
        const QString& fragmentShaderFilePath);

    ~Model();

    void draw(
        const QMatrix4x4& projectionMatrix,
        const QMatrix4x4& viewMatrix,
        const QMatrix4x4& gimbalAngle,
        const float scaleFactor);

private:

    QOpenGLVertexArrayObject vao;

    // Vertex- and index buffer objects
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;

    unsigned int iboLength = 0;

    // Vertex and fragment shader program
    QOpenGLShaderProgram* shaderProgram;

    // Object owner
    Assimp::Importer importer;

    // Loads the contents of an object file into the importer
    void loadObjectFromFile(const QString& objectFilePath);

    // Generates a VBO/IBO from a loaded model
    void generateVBO(GLfloat* const vbo) const;
    void generateIBO(GLuint* const ibo) const;

    // Calculates the length of the aggregated VBO/IBO
    unsigned int lengthOfVBO() const;
    unsigned int lengthOfIBO() const;
};

#endif // MODEL_H
