#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QFile>
#include <QFileInfo>

Model::Model(
    const QString& objectFilePath,
    const QString& fragmentShaderFilePath) :

    vbo(QOpenGLBuffer::VertexBuffer),
    ibo(QOpenGLBuffer::IndexBuffer)
{
    // Load object from file into memory //

    // Make GL functions available
    initializeOpenGLFunctions();

    // Generate buffer objects
    this->vao.create();
    this->vbo.create();
    this->ibo.create();

    // Tell the program to use the generated objects
    this->vao.bind();
    this->vbo.bind();
    this->ibo.bind();

    // Load an object file into the asset importer
    this->loadObjectFromFile(objectFilePath);

    // Populate the VBO with data derived from the loaded object
    this->vbo.allocate(this->lengthOfVBO() * sizeof(GLfloat));
    GLvoid* vboPtr = this->vbo.map(QOpenGLBuffer::WriteOnly);
    Q_ASSERT(vboPtr);
    this->generateVBO(static_cast<GLfloat*>(vboPtr));
    this->vbo.unmap();

    // Populate the IBO with data derived from the loaded object
    this->iboLength = this->lengthOfIBO();
    this->ibo.allocate(this->iboLength * sizeof(GLuint));
    GLvoid* iboPtr = this->ibo.map(QOpenGLBuffer::WriteOnly);
    Q_ASSERT(iboPtr);
    this->generateIBO(static_cast<GLuint*>(iboPtr));
    this->ibo.unmap();


    // Communicate the buffer layout to OpenGL //

    const std::size_t stride = 3 * sizeof(GLfloat);

    // Define the position and layout of each vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 0));


    // Compile and link shader //

    this->shaderProgram = new QOpenGLShaderProgram();

    // Compile and attach shader stages for linking
    this->shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertshader/model.vert");
    this->shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFilePath);

    // Link attached stages
    this->shaderProgram->link();

    // Abort the program if any of these steps failed
    Q_ASSERT(this->shaderProgram->isLinked());
}

Model::~Model()
{
    this->vao.destroy();
    this->ibo.destroy();
    this->vbo.destroy();

    delete this->shaderProgram;
}

void Model::draw(
    const QMatrix4x4& projectionMatrix,
    const QMatrix4x4& viewMatrix,
    const QMatrix4x4& gimbalAngle,
    const float scaleFactor)
{
    // Generate the roation Matrix //

    this->shaderProgram->bind();

    QMatrix4x4 model;
    model.scale(QVector3D(scaleFactor, scaleFactor, scaleFactor));
    model = model * gimbalAngle;

    const auto mvpMatrix = projectionMatrix * viewMatrix * model;
    this->shaderProgram->setUniformValue(0, mvpMatrix);


    // Render //

    this->vao.bind();
    glDrawElements(GL_TRIANGLES, this->iboLength, GL_UNSIGNED_INT, nullptr);
    this->vao.release();

    this->shaderProgram->release();
}

void Model::loadObjectFromFile(const QString& objectFilePath)
{
    // Load the file's contents into memory //

    QFile file(objectFilePath);

    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error(file.errorString().toStdString());

    const auto buffer = file.readAll();

    file.close();


    // Parse the loaded contents and store them in importer //

    // Retrieve file extension to give the importer a hint which module to use
    const auto fileExtension = QFileInfo(objectFilePath).completeSuffix().toLatin1();

    // Load scene
    const auto scene = this->importer.ReadFileFromMemory(
        buffer.constData(),
        buffer.length(),
        aiProcess_Triangulate
            | aiProcess_JoinIdenticalVertices
            | aiProcess_GenSmoothNormals,
        fileExtension.constData()
    );

    // Assert import successful
    if(!scene)
        throw std::runtime_error(this->importer.GetErrorString());

    // Assert there is at least one mesh
    if (scene->mNumMeshes == 0 || scene->mMeshes[0]->mNumFaces == 0)
    {
        importer.FreeScene();
        throw std::runtime_error("No meshes loaded");
    }

}

void Model::generateVBO(GLfloat* const vbo) const
{
    // Get the mesh from the importer //

    if(!importer.GetScene()->HasMeshes())
        throw std::invalid_argument("Scene has no meshes");

    const auto* const mesh = this->importer.GetScene()->mMeshes[0];


    // Flatten data to output //

    unsigned int n = 0;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        n = 0;

        vbo[i*3+n++] = mesh->mVertices[i].x;
        vbo[i*3+n++] = mesh->mVertices[i].y;
        vbo[i*3+n++] = mesh->mVertices[i].z;
    }
}

void Model::generateIBO(GLuint* const ibo) const
{
    // Get the mesh from the importer //

    if(!importer.GetScene()->HasMeshes())
        throw std::invalid_argument("Scene has no mesh");

    const auto* const mesh = this->importer.GetScene()->mMeshes[0];


    // Flatten data to output //

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        ibo[i*3+0] = mesh->mFaces[i].mIndices[0];
        ibo[i*3+1] = mesh->mFaces[i].mIndices[1];
        ibo[i*3+2] = mesh->mFaces[i].mIndices[2];
    }
}

unsigned int Model::lengthOfVBO() const
{
    // Get the mesh from the importer //

    if(!importer.GetScene()->HasMeshes())
        throw std::invalid_argument("Scene has no meshes");

    const auto* const mesh = this->importer.GetScene()->mMeshes[0];


    // Calculate VBO length //

    return mesh->mNumVertices * 3;
}

unsigned int Model::lengthOfIBO() const
{
    // Get the mesh from the importer //

    if(!importer.GetScene()->HasMeshes())
        throw std::invalid_argument("Scene has no meshes");

    const auto* const mesh = this->importer.GetScene()->mMeshes[0];


    // Calculate array length //

    if (mesh->mNumFaces == 0)
        return 0;

    return mesh->mNumFaces * mesh->mFaces[0].mNumIndices;
}
