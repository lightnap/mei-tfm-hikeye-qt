#ifndef C_MAIN_GRAPHICS_WIDGET_H
#define C_MAIN_GRAPHICS_WIDGET_H

#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include <CCamera.hpp>
#include <Types.hpp>

#include <memory>

struct STerrain;

/**
 * Main rendering widget class.
 */
class CMainGraphicsWidget
  : public QOpenGLWidget
  , protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

  private:
    /**
     * @brief Struct to contain the data OpenGL keeps about a terrain model.
     */
    struct STerrainGlData
    {
        GLuint oVertexAttributeId {0U}; //!< Location of all the buffers related to this terrain model.
        GLuint oVertexBufferId {0U};    //!< Location of the buffer containing the vertices.
        GLuint oTriangleBufferId {0U};  //!< Location of the buffer containing the triangle indices.

        s32 oTriangleCount {0}; //!< Number of triangles of the model.
    };

  public slots:
    /**
     * @brief Gets called each time the mouse is pressed.
     * @param aMouseInfo: Info (event) about the mouse.
     */
    virtual void mousePressEvent(QMouseEvent* aMouseInfo) override;

    /**
     * @brief Gets called each time the mouse is moved.
     * @param aMouseInfo: Info (event) about the mouse.
     */
    virtual void mouseMoveEvent(QMouseEvent* aMouseInfo) override;

    /**
     * @brief Gets called each time a mouse button is released.
     * @param aMouseInfo: Info (event) about the mouse.
     */
    virtual void mouseReleaseEvent(QMouseEvent* aMouseInfo) override;

    /**
     * @brief Gets called each time the mousewheel is moved.
     * @param aWheelInfo: Info (event) about the mouse wheel.
     */
    virtual void wheelEvent(QWheelEvent* aWheelInfo) override;

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
     * @param aTerrain: Information on the terrain model to load.
     */
    void LoadModel(const STerrain& aTerrain);

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

    /**
     * @brief Resets the terrain openGlData by deleting the old buffers.
     */
    void ResetTerrainBuffers();

  private:
    QOpenGLShaderProgram* mShaderProgram; //!< Shader program.

    bool           mModelLoaded;        //!< Indicate whether a terrain model has been loaded.
    GLuint         mTriangleVAOId;      //!< Id of the VAO of the triangle we want to draw.
    GLuint         mVertexAttributeId;  //!< Identifier for shader attribute "vertex".
    GLuint         mTransformUniformId; //!< Identifier for shader uniform "transformMatrix".
    STerrainGlData mTerrainData;        //!< Keeps the Gl data of the currently loaded terrain.

    GLint mWindowWidth;  //!< Current window width.
    GLint mWindowHeight; //!< Current window height.

    std::unique_ptr<CCamera> mCamera; //!< Camera of the scene.

    // TODO: HK-47 Turn this into a Vector or something.
    s32 mLastClickPosX {0}; //!< Last position where the mouse clicked, X coordinate.
    s32 mLastClickPosY {0}; //!< Last position where the mouse clicked, Y coordinate. // TODO: HK-47 Turn into a vector of int.
};

#endif // C_MAIN_GRAPHICS_WIDGET_H
