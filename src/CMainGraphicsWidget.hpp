#ifndef C_MAIN_GRAPHICS_WIDGET_H
#define C_MAIN_GRAPHICS_WIDGET_H

#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

/**
 * Main rendering widget class.
 */
class CMainGraphicsWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param apParent Parent widget in hierarchy.
     */
    CMainGraphicsWidget(QWidget* apParent = nullptr);

    /**
     * @brief Destructor.
     */
    ~CMainGraphicsWidget();

    /**
     * @brief Loads 3D Terrain model.
     */
    void LoadModel();

    /**
     * @brief Loads texture and binds it to model.
     */
    void LoadTexture();

  protected:
    /**
     * @brief Initializes graphic context.
     */
    void initializeGL() override;

    /**
     * Performs the openGL rendering.
     */
    void paintGL() override;

    /**
     * @brief Called whenever widget is resized.
     * @param aWidth The new width of the window.
     * @param aHeight The new height of the window.
     */
    void resizeGL(int aWidth, int aHeight) override;

  private:
    /**
     * @brief Creates VAO and VBO buffers for the object we want to draw.
     */
    void CreateGLBuffers();

    /**
     * @brief Loads shaders from file and prepares shader program.
     */
    void LoadShaders();

  private:
    QOpenGLShaderProgram* mShaderProgram; //!< Shader program.

    GLuint mTriangleVAOId;     //!< Id of the VAO of the triangle we want to draw.
    GLuint mVertexAttributeId; //!< Identifier for shader attribute "vertex".
    GLint  mWindowWidth;       //!< Current window width.
    GLint  mWindowHeight;      //!< Current window height.
};

#endif // C_MAIN_GRAPHICS_WIDGET_H
