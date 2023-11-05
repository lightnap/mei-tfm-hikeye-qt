#include "CMainGraphicsWidget.hpp"

#include "common/Math.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"

//#include <GL/glew.h>
#include <QImage>
#include <QMatrix4x4> // TODO: HK-47 Consider using the glm library.
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QVector3D> // TODO: HK-47 Consider using the glm library.
#include <iostream>
#include <memory>
#include <string>

namespace
{
const QVector3D   BACKGROUND_COLOR {0.5, 0.7, 1.0}; //!< Scene background color.
const GLuint      EMPTY_VAO {0U};                   //!< Specifies that no vertex array object is being bound.
const GLuint      EMPTY_SHADER {0U};                //!< Specifies that no shader program is being bound.
const std::string SHADERS_PATH {"../src/shaders/"}; //!< Path to shader files.
}

CMainGraphicsWidget::CMainGraphicsWidget(QWidget* apParent)
  : QOpenGLWidget(apParent)
  , mShaderProgram(nullptr)
  , mModelLoaded(false)
  , mTriangleVAOId(0U)
  , mVertexAttributeId(0U)
  , mTransformUniformId(0U)
  , mTerrainData()
  , mWindowWidth(100)
  , mWindowHeight(100)
  , mLastClickPosX(0)
  , mLastClickPosY(0)
{
    setFocusPolicy(Qt::StrongFocus);
    mCamera = std::make_unique<CCamera>();
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

    // TODO: HK-45 Clean this.

    makeCurrent();
    ResetTerrainBuffers();

    mTerrainData.oTriangleCount = aTerrain.oTriangleCount;
    glGenVertexArrays(1, &mTerrainData.oVertexAttributeId);
    glBindVertexArray(mTerrainData.oVertexAttributeId);

    // glUseProgram(mShaderProgram);

    // GLuint attribVertexLoc = glGetAttribLocation(mShaderProgram, "a_position");

    const auto& Vertices {aTerrain.oVertices};
    const auto& Triangles {aTerrain.oTriangles};

    glGenBuffers(1, &mTerrainData.oVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, mTerrainData.oVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(mVertexAttributeId, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mVertexAttributeId);

    glGenBuffers(1, &mTerrainData.oTriangleBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTerrainData.oTriangleBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Triangles.size(), &Triangles[0], GL_STATIC_DRAW);

    glBindVertexArray(EMPTY_VAO);
    // glUseProgram(EMPTY_SHADER);

    mSceneBox = aTerrain.oBounds;

    mModelLoaded = true;
    mCamera->LookAt(aTerrain.oBounds);
    update();
}

void CMainGraphicsWidget::ResetTerrainBuffers()
{
    if (mTerrainData.oVertexBufferId > 0)
    {
        glDeleteBuffers(1, &(mTerrainData.oVertexBufferId));
    }

    if (mTerrainData.oTriangleBufferId > 0)
    {
        glDeleteBuffers(1, &(mTerrainData.oTriangleBufferId));
    }

    if (mTerrainData.oVertexAttributeId > 0)
    {
        glDeleteBuffers(1, &(mTerrainData.oVertexAttributeId));
    }

    mTerrainData.oTriangleCount = 0;
}

void CMainGraphicsWidget::LoadTexture(const STexture& aTexture)
{
    std::cout << "[MainGraphicsWidget]: Loading terrain texture" << std::endl;

    makeCurrent();

    const QImage& Image {aTexture.oTexture};

    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image.width(), Image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    update();
}

void CMainGraphicsWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // setMouseTracking(true);
    glEnable(GL_DEPTH_TEST);
    glClearColor(BACKGROUND_COLOR.x(), BACKGROUND_COLOR.y(), BACKGROUND_COLOR.z(), 1.0);
    LoadShaders();
    CreateGLBuffers();

    // TODO: Move this to a function (and undertand it).
    unsigned char foo {255};
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(&foo));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
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

    // TODO: This does not go here.
    f64 CameraDistance {Math::Norm(mCamera->GetEye() - mSceneBox.GetCenter())};
    f64 SceneRadius {mSceneBox.GetRadius()};

    if (CameraDistance < SceneRadius)
    {
        mCamera->SetPlanes(100, 2 * SceneRadius);
    }
    else
    {
        mCamera->SetPlanes(CameraDistance - SceneRadius, 2 * SceneRadius + CameraDistance);
    }

    QMatrix4x4 TransformMatrix {mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix()};

    glUniformMatrix4fv(mTransformUniformId, 1, GL_FALSE, TransformMatrix.data());

    // TODO: Clean this up.
    glUniform2f(glGetUniformLocation(mShaderProgram->programId(), "u_worldMin"), mSceneBox.oMin.oX, mSceneBox.oMin.oY);
    glUniform2f(glGetUniformLocation(mShaderProgram->programId(), "u_worldSize"), mSceneBox.GetWidth(), mSceneBox.GetHeight());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glUniform1i(glGetUniformLocation(mShaderProgram->programId(), "u_texture"), 0);

    // Clear frame buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw scene.
    if (!mModelLoaded)
    {
        // glBindVertexArray(mTriangleVAOId);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else
    {
        glBindVertexArray(mTerrainData.oVertexAttributeId);
        glDrawElements(GL_TRIANGLES, 3 * mTerrainData.oTriangleCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(EMPTY_VAO);
}

void CMainGraphicsWidget::resizeGL(int aWidth, int aHeight)
{
    mWindowWidth = aWidth;
    mWindowHeight = aHeight;
    f64 AspectRatio {static_cast<f64>(aWidth) / static_cast<f64>(aHeight)};
    mCamera->UpdateAspectRatio(AspectRatio);
}

void CMainGraphicsWidget::CreateGLBuffers()
{
    QVector3D TriangleVertices[36] {{-1, -1, 1},  {1, -1, 1},  {1, 1, 1},   {-1, -1, 1},  {1, 1, 1},    {-1, 1, 1},  {1, -1, 1},  {1, -1, -1}, {1, 1, -1},
                                    {1, -1, 1},   {1, 1, -1},  {1, 1, 1},   {1, -1, -1},  {-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}, {-1, 1, -1}, {1, 1, -1},
                                    {-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1},  {-1, -1, -1}, {-1, 1, 1},   {-1, 1, -1}, {-1, 1, 1},  {1, 1, 1},   {1, 1, -1},
                                    {-1, 1, 1},   {1, 1, -1},  {-1, 1, -1}, {1, -1, 1},   {-1, -1, -1}, {1, -1, -1}, {1, -1, 1},  {-1, -1, 1}, {-1, -1, -1}};

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
    // TODO: HK-46 Create shader for sky.
    QOpenGLShader FragmentShader(QOpenGLShader::Fragment, this);
    QOpenGLShader VertexShader(QOpenGLShader::Vertex, this);

    // TODO: HK-46 Move shader names to variables.
    FragmentShader.compileSourceFile((SHADERS_PATH + "terrain.frag").c_str());
    VertexShader.compileSourceFile((SHADERS_PATH + "basicShader.vert").c_str());

    // TODO: HK-46 Turn this into a smart pointer.
    mShaderProgram = new QOpenGLShaderProgram(this);

    mShaderProgram->addShader(&FragmentShader);
    mShaderProgram->addShader(&VertexShader);
    mShaderProgram->link();
    mShaderProgram->bind();

    // TODO: HK-46 Move shader attributes to unordered map if there are more of them.
    mVertexAttributeId = glGetAttribLocation(mShaderProgram->programId(), "vertex");

    // TODO: HK-46 Move shader uniforms to unodrdered map if there are more of them.
    mTransformUniformId = glGetUniformLocation(mShaderProgram->programId(), "transformMatrix");
}

// TODO: HK-43 Move all these functions to a camera controller that wraps the camera and has the speeds and all.
// Oooh camera operator is a good name...
// TODO: HK-43 Change camera controls so that you can fly around and get close to interesting places.
void CMainGraphicsWidget::mousePressEvent(QMouseEvent* aMouseInfo)
{
    mLastClickPosX = aMouseInfo->globalX();
    mLastClickPosY = aMouseInfo->globalY();

    update();
}

void CMainGraphicsWidget::mouseMoveEvent(QMouseEvent* aMouseInfo)
{
    const s32 CurrentMouseX {aMouseInfo->globalX()}; // TODO: HK-47 Turn this into a vector of int.
    const s32 CurrentMouseY {aMouseInfo->globalY()};

    const s32 MouseDisplacedX {CurrentMouseX - mLastClickPosX};
    const s32 MouseDisplacedY {CurrentMouseY - mLastClickPosY};

    if (aMouseInfo->buttons() & Qt::LeftButton)
    {
        const f64 SPIN_SPEED_X {0.01};
        const f64 SPIN_SPEED_Y {0.005};

        const f64 DisplacementX {-MouseDisplacedX * SPIN_SPEED_X};
        const f64 DisplacementY {-MouseDisplacedY * SPIN_SPEED_Y};
        mCamera->HorizontalRotateArroundAt(DisplacementX);
        mCamera->VerticalRotateArroundAt(DisplacementY);
    }
    else if (aMouseInfo->buttons() & Qt::RightButton)
    {
        const f64 ZOOM_SPEED {0.002f};

        // TODO: HK-43 Could use move radial with MoveAt = false.
        const Math::Vector3D PreviousAt {mCamera->GetAt()};
        const f64            ViewDistance {Math::Norm(mCamera->GetAt() - mCamera->GetEye())};

        mCamera->RadialTranslate(MouseDisplacedX * ViewDistance * ZOOM_SPEED);
        mCamera->SetAt(PreviousAt);
    }
    else if (aMouseInfo->buttons() & Qt::MiddleButton)
    {
        // TODO: HK-43 Put all these speed constansts in the same place.
        const f64 TRANSLATE_SPEED_X {0.03};
        const f64 TRANSLATE_SPEED_Y {0.03};

        mCamera->HorizontalTranslate(MouseDisplacedX * TRANSLATE_SPEED_X);
        mCamera->VerticalTranslate(MouseDisplacedY * TRANSLATE_SPEED_Y);
    }

    mLastClickPosX = aMouseInfo->globalX();
    mLastClickPosY = aMouseInfo->globalY();

    update();
}

void CMainGraphicsWidget::mouseReleaseEvent(QMouseEvent*)
{
    update();
}

void CMainGraphicsWidget::wheelEvent(QWheelEvent* aWheelInfo)
{
    // TODO: HK-43 Could use radial move.
    const f64 WHEEL_ZOOM_SPEED {0.1};
    const s32 ANGLE_DELTA_TO_DEG {8};
    const s32 DEGREES_TO_WHEEL_POSITION {15};

    s32 DegreesRotated {aWheelInfo->angleDelta().y() / ANGLE_DELTA_TO_DEG};
    s32 PositionsAdvanced {DegreesRotated / DEGREES_TO_WHEEL_POSITION};

    if (!(aWheelInfo->modifiers() & Qt::ShiftModifier))
    {
        const Math::Vector3D ViewVector {mCamera->GetAt() - mCamera->GetEye()};
        const f64            ViewDistance {Math::Norm(ViewVector)};
        const Math::Vector3D ViewDirection = ViewVector / ViewDistance;

        const f64 NewViewDistance {ViewDistance * WHEEL_ZOOM_SPEED * PositionsAdvanced};
        mCamera->SetEye(mCamera->GetEye() + ViewDirection * NewViewDistance);
    }

    update();
}
