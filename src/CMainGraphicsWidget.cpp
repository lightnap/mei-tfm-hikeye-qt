#include "CMainGraphicsWidget.hpp"

#include "STerrain.hpp"

//#include <GL/glew.h>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QVector3D> // TODO: Consider using the glm library.
#include <iostream>
#include <string>

namespace
{
const QVector3D   BACKGROUND_COLOR {0.5, 0.7, 1.0}; //!< Scene background color.
const GLuint      EMPTY_VAO {0};                    //!< Specifies that no vertex array object is being bound.
const std::string SHADERS_PATH {"../src/shaders/"}; //!< Path to shader files.
}

CMainGraphicsWidget::CMainGraphicsWidget(QWidget* apParent)
  : QOpenGLWidget(apParent)
  , mShaderProgram(nullptr)
  , mTriangleVAOId(0)
  , mVertexAttributeId(0)
  , mWindowWidth(100)
  , mWindowHeight(100)
{
    setFocusPolicy(Qt::StrongFocus);
}

CMainGraphicsWidget::~CMainGraphicsWidget()
{
    if (mShaderProgram != nullptr)
    {
        delete mShaderProgram;
    }
}

void CMainGraphicsWidget::LoadModel(const STerrain& aTerrain)
{

    std::cout << "[MainGraphicsWidget]: Loading terrain model with # vertices = : " << aTerrain.oVertices.size() << std::endl;
    // TODO: Implement this.
}

void CMainGraphicsWidget::LoadTexture()
{
    std::cout << "[MainGraphicsWidget]: Loading terrain texture" << std::endl;
    // TODO: Implement this.
}

void CMainGraphicsWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(BACKGROUND_COLOR.x(), BACKGROUND_COLOR.y(), BACKGROUND_COLOR.z(), 1.0);
    LoadShaders();
    CreateGLBuffers();
}

void CMainGraphicsWidget::paintGL()
{
// Necessary for macs with retina screen.
#ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    mWindowWidth = vp[2];
    mWindowHeight = vp[3];
#endif

    glViewport(0, 0, mWindowWidth, mWindowHeight);

    // Clear frame buffer.
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw scene.
    glBindVertexArray(mTriangleVAOId);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glBindVertexArray(EMPTY_VAO);
}

void CMainGraphicsWidget::resizeGL(int aWidth, int aHeight)
{
    mWindowWidth = aWidth;
    mWindowHeight = aHeight;
}

void CMainGraphicsWidget::CreateGLBuffers()
{
    QVector3D TriangleVertices[9] {{0.0, 0.0, 0.0},
                                   {1.0, 0.0, 0.0},
                                   {0.5, 1.0, 0.0},
                                   {0.0, -1.0, 0.0},
                                   {1.0, -1.0, 0.0},
                                   {0.0, 0.0, 0.0},
                                   {0.0, 0.0, 0.0},
                                   {1.0, -1.0, 0.0},
                                   {1.0, 0.0, 0.0}};

    // Genrate Vertex array object (VAO).
    glGenVertexArrays(1, &mTriangleVAOId);
    glBindVertexArray(mTriangleVAOId);

    // Generate vertex buffer.
    GLuint TriangleVertexBufferId;
    glGenBuffers(1, &TriangleVertexBufferId);

    // Bind vertex buffer to VAO.
    glBindBuffer(GL_ARRAY_BUFFER, TriangleVertexBufferId);

    // Fill vertex buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

    // Bind vertex attributes to VAO.
    glVertexAttribPointer(mVertexAttributeId, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mVertexAttributeId);

    glBindVertexArray(EMPTY_VAO);
}

void CMainGraphicsWidget::LoadShaders()
{
    // TODO: Create shader for sky.
    QOpenGLShader FragmentShader(QOpenGLShader::Fragment, this);
    QOpenGLShader VertexShader(QOpenGLShader::Vertex, this);

    // TODO: Move shader names to variables.
    FragmentShader.compileSourceFile((SHADERS_PATH + "basicShader.frag").c_str());
    VertexShader.compileSourceFile((SHADERS_PATH + "basicShader.vert").c_str());

    // TODO: Turn this into a smart pointer.
    mShaderProgram = new QOpenGLShaderProgram(this);

    mShaderProgram->addShader(&FragmentShader);
    mShaderProgram->addShader(&VertexShader);
    mShaderProgram->link();
    mShaderProgram->bind();

    // TODO: Move shader attributes to unordered map if there are more of them.
    mVertexAttributeId = glGetAttribLocation(mShaderProgram->programId(), "vertex");
}
