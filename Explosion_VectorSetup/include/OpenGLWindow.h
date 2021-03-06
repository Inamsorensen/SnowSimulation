#ifndef OPENGLWINDOW
#define OPENGLWINDOW

#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>
#include <QTime>
#include <QOpenGLWindow>
#include <memory>

#include "Emitter.h"
#include "Grid.h"


class OpenGLWindow : public QOpenGLWindow
{
  Q_OBJECT
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    OpenGLWindow();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~OpenGLWindow();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when the window is re-sized
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(QResizeEvent *_event);
    void resizeGL(int _w, int _h);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets up simulation
    //----------------------------------------------------------------------------------------------------------------------
    void simulationSetup();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Build VAO for bounding box
    //----------------------------------------------------------------------------------------------------------------------
    void buildVAO();


private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Grid setup for fluid calculations
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Grid/bounding box position (give position of back lower corner)
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_gridPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Grid/bounding box size (set as cube so equal size in all directions)
    //----------------------------------------------------------------------------------------------------------------------
    float m_gridSize;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Number of cells along one of the lengths of the grid (same for the other two). Total number of cells is m_noCell^3
    //----------------------------------------------------------------------------------------------------------------------
    int m_noCells;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Grid pointer
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<Grid> m_grid;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Particle setup
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Position of emitter
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_emitterPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Emitter radius
    //----------------------------------------------------------------------------------------------------------------------
    float m_emitterRadius;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Total number of particles
    //----------------------------------------------------------------------------------------------------------------------
    int m_noParticles;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Number of particles emitted per time step
    //----------------------------------------------------------------------------------------------------------------------
    int m_emissionRate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Emitter pointer
    //----------------------------------------------------------------------------------------------------------------------
    Emitter* m_emitter;


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Timer setup for simulation
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Time step of simulation
    //----------------------------------------------------------------------------------------------------------------------
    float m_simTimeStep;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Total elapsed time of simulation
    //----------------------------------------------------------------------------------------------------------------------
    float m_simElapsedTime;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Number of frames in simulation
    //----------------------------------------------------------------------------------------------------------------------
    int m_noFrames;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera m_camera;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Bounding box VAO
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<ngl::VertexArrayObject> m_vao;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief window width
    //----------------------------------------------------------------------------------------------------------------------
    int m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief window height
    //----------------------------------------------------------------------------------------------------------------------
    int m_height;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform matrices to the shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event);

    void timerEvent(QTimerEvent *);


};


#endif // OPENGLWINDOW


