#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <shaders.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include<creategemoetry.h>
#include <readgcode.h>
#include <QKeyEvent>
#include <QBasicTimer>
#include <machineTool.h>


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)


class MyOpenGLWidget:public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();

    static bool isTransparent() { return m_transparent; }
    static void setTransparent(bool t) { m_transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setXTranslation(int distance);
    void setYTranslation(int distance);
    void setZoom(int distance);
    void cleanup();
    void setColor(QVector3D color);
    void startMoving();


protected:
    void initializeGL() override;    //The initializeGL() function is used to perform useful initialization
    //tasks that are needed to render the 3D scene.

    void paintGL() override;               //The paintGL() function is used to paint the contents of the
    //scene onto the widget.

    void resizeGL(int width, int height) override; //The resizeGL() function is used to ensure that the OpenGL
    //implementation renders the scene onto a viewport that matches
    //the size of the widget, using the correct transformation from
    //3D coordinates to 2D viewport coordinates. The function is
    //called whenever the widget's dimensions change, and is
    //supplied with the new width and height.

    void createObject(QVector<QString> structureOrder,QVector<float> motionForEachAxis);
    void createURDFObject();
    void moveComponent(const QVector<GLfloat> data, int count);   //need to use pass by reference

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *e) override;


private:
    void setupVertexAttribs();

    MachineTool MT;
    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    int m_xTran;
    int m_yTran;
    int m_zTran;
    QPoint m_lastPos;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_geoemtryVbo;      //Vertex Buffer Objects
    QOpenGLShaderProgram *m_program;  //shader program object
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    int m_colorLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    QMatrix4x4 m_X_Translate;
    QMatrix4x4 m_Y_Translate;
    QMatrix4x4 m_Z_Translate;
    QMatrix4x4 m_SPINDLE_Translate;
    QMatrix4x4 m_A_Rotate;
    QMatrix4x4 m_B_Rotate;
    QMatrix4x4 m_C_Rotate;

    QVector3D m_color;
    static bool m_transparent;
    CreateGemoetry m_geometry;
    ReadGCode m_gcode;

    QBasicTimer timer;

    int startNubmer;
    float moveX;
    float moveY;
    float moveA;
    float moveB;
    float moveC;
    float moveZ;
    float fov;

    int count_nc_line;
    QVector<QString> structureOrder;
    QVector<QString> stlFileOrder;
    QVector<float> motionForEachAxis;

};

#endif // MYOPENGLWIDGET_H
