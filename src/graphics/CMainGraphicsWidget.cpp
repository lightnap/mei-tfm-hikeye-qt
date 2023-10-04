#include "CMainGraphicsWidget.hpp"

#include "STerrain.hpp"

//#include <GL/glew.h>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QVector3D> // TODO: Consider using the glm library.
#include <iostream>
#include <memory>
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
    f64 AspectRatio {static_cast<f64>(aWidth) / static_cast<f64>(aHeight)};
    mCamera->UpdateAspectRatio(AspectRatio);
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

void CMainGraphicsWidget::mousePressEvent(QMouseEvent* aMouseInfo)
{
    mLastClickPosX = aMouseInfo->globalX();
    mLastClickPosY = aMouseInfo->globalY();

    update();
}

void CMainGraphicsWidget::mouseMoveEvent(QMouseEvent* aMouseInfo)
{
    const s32 CurrentMouseX {aMouseInfo->globalX()}; // TODO: Turn this into a vector of int.
    const s32 CurrentMouseY {aMouseInfo->globalY()};

    const s32 MouseDisplacedX {mLastClickPosX - CurrentMouseX};
    const s32 MouseDisplacedY {mLastClickPosY - CurrentMouseY};

    if (aMouseInfo->buttons() & Qt::LeftButton)
    {
        const f64 SPIN_SPEED_X {0.01};
        const f64 SPIN_SPEED_Y {0.005};

        const f64 DisplacementX {MouseDisplacedX * SPIN_SPEED_X};
        const f64 DisplacementY {MouseDisplacedY * SPIN_SPEED_Y};
        mCamera->HorizontalRotateArroundAt(DisplacementX);
        mCamera->VerticalRotateArroundAt(DisplacementY);
    }
    else if (aMouseInfo->buttons() & Qt::RightButton)
    {
        const f64 ZOOM_SPEED {0.002f};

        // TODO: Could use move radial with MoveAt = false.
        const Math::Vector3D PreviousAt {mCamera->GetAt()};
        const f64            ViewDistance {Math::Norm(mCamera->GetAt() - mCamera->GetEye())};

        mCamera->RadialTranslate(MouseDisplacedX * ViewDistance * ZOOM_SPEED);
        mCamera->SetAt(PreviousAt);
    }
    else if (aMouseInfo->buttons() & Qt::MidButton)
    {
        const f64 TRANSLATE_SPEED_X {1.0};
        const f64 TRANSLATE_SPEED_Y {1.0};

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
    // TODO: Could use radial move.
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
