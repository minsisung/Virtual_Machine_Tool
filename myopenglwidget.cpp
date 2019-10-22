#include "myopenglwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <shaders.h>
#include <QtOpenGL>
#include <qdebug.h>

bool MyOpenGLWidget::m_transparent = false;

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    :QOpenGLWidget(parent),
      m_xRot(0), m_yRot(0), m_zRot(0),
      m_xTran(0), m_yTran(0), m_zTran(-4500), //original camera position
      m_program(nullptr), m_color(1.0,1.0,1.0),startNubmer(0),
      moveX(0),moveY(0),moveA(0),moveB(0),moveC(0),moveZ(0),
      fov(45.0f),count_nc_line(0)

{
    setFocusPolicy( Qt::StrongFocus ); //This property holds the way the widget accepts keyboard focus

    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    cleanup();
}

QSize MyOpenGLWidget::minimumSizeHint() const  //holds the recommended minimum size for the widget
{
    return QSize(100, 100);
}

QSize MyOpenGLWidget::sizeHint() const    //holds the recommended size for the widget
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}  void MyOpenGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        update();
    }
}

void MyOpenGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        update();
    }
}

void MyOpenGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        update();
    }
}

void MyOpenGLWidget::setXTranslation(int distance)
{
    if (distance != m_xTran) {
        m_xTran = distance;
        update();
    }
}

void MyOpenGLWidget::setYTranslation(int distance)
{
    if (distance != m_yTran) {
        m_yTran = distance;
        update();
    }
}

void MyOpenGLWidget::setZoom(int yoffset)
{
    if(fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov > 90.0f)
        fov = 90.0f;

    resizeGL(MyOpenGLWidget::width(),MyOpenGLWidget::height());
    update();
}

void MyOpenGLWidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    m_geoemtryVbo.destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void MyOpenGLWidget::setColor(QVector3D color)
{
    //    //Set default color for the component
    //    m_program->setUniformValue(m_colorLoc, color);
    //    update();
    m_color = color;
}

void MyOpenGLWidget::startMoving()
{
    // Use QBasicTimer because its faster than QTimer
    timer.start(5, this);
}

void MyOpenGLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyOpenGLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.3f, 0.3f, 0.3f, m_transparent ? 0 : 1);

    motionForEachAxis<<moveX<<moveY<<moveZ<<moveA<<moveB<<moveC;

    ////    RT!!!!!!!
    //        structureOrder<<"BASE"<<"Y"<<"X"<<"A"<<"C"<<"Z"<<"SPINDLE";

    //        stlFileOrder<<"C:/Files_For_QT/TMV-710/TMV-710A - Base-1.stl"<<"C:/Files_For_QT/TMV-710/TMV-710A - Y-1.stl"
    //                <<"C:/Files_For_QT/TMV-710/TMV-710A - X-1.stl"<<"C:/Files_For_QT/TMV-710/TMV-710A - A-1.stl"
    //                <<"C:/Files_For_QT/TMV-710/TMV-710A - C-1.stl"<<"C:/Files_For_QT/TMV-710/TMV-710A - Z-1.stl"
    //                <<"C:/Files_For_QT/TMV-710/TMV-710A - SPINDLE-1.stl";


    //    [HAAS VF-2TR] (RT)!!!!!!!
    structureOrder<<"BASE"<<"Y"<<"X"<<"X"<<"A"<<"C"<<"Z";

    stlFileOrder<<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_BASE_LG-1.stl"<<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_AXES_Y_LG-1.stl"
               <<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_AXES_X_LG-1.stl" <<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_AXES_X_DG-1.stl"
              <<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_AXES_A_LG-1.stl"<<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_AXES_C_DG-1.stl"
             <<"C:/Files_For_QT/VF-2TR/VF-2TR_S_XT_2013-08 - VF-2TR_AXES_Z_DG-1.stl";


    ////    SR!!!!!!
    //        structureOrder<<"BASE"<<"X"<<"Y"<<"Z"<<"A"<<"B"<<"SPINDLE";

    //        stlFileOrder
    //                <<"C:/Files_For_QT/VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_BASE-1.stl"  <<"C:/Files_For_QT//VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_X-1.stl"
    //                <<"C:/Files_For_QT/VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_Y-1.stl"<<"C:/Files_For_QT/VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_Z-1.stl"
    //                <<"C:/Files_For_QT/VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_A-1.stl"<<"C:/Files_For_QT/VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_B-1.stl"
    //                <<"C:/Files_For_QT/VMT_MINSI_WYXAB/VMT_MINSI_WYXAB - WYXAB_SPINDLE-1.stl";

    ////    //HT!!!!!!!
    //    structureOrder<<"BASE"<<"X"<<"Y"<<"A"<<"Z"<<"B"<<"SPINDLE";

    //    stlFileOrder
    //            <<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_BASE-1.stl"  <<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_X-1.stl"
    //           <<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_Y-1.stl"<<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_A-1.stl"
    //          <<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_Z-1.stl"<<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_B-1.stl"
    //         <<"C:/Files_For_QT/VMT_MINSI_WAYXB/VMT_MINSI_WAYXB - WAYXB_SPINDLE-1.stl";



    qDebug()<<structureOrder;

    for(int i = 0;i<structureOrder.length();++i){
        if(structureOrder.at(i)=="BASE")
            m_geometry.readSTL("BASE",stlFileOrder.at(i));

        if(structureOrder.at(i)=="Y")
            m_geometry.readSTL("Y",stlFileOrder.at(i));

        if(structureOrder.at(i)=="X")
            m_geometry.readSTL("X",stlFileOrder.at(i));

        if(structureOrder.at(i)=="A")
            m_geometry.readSTL("A",stlFileOrder.at(i));

        if(structureOrder.at(i)=="B")
            m_geometry.readSTL("B",stlFileOrder.at(i));

        if(structureOrder.at(i)=="C")
            m_geometry.readSTL("C",stlFileOrder.at(i));

        if(structureOrder.at(i)=="Z")
            m_geometry.readSTL("Z",stlFileOrder.at(i));

        if(structureOrder.at(i)=="SPINDLE")
            m_geometry.readSTL("SPINDLE",stlFileOrder.at(i));
    }

    //read NC Code
    m_gcode.read("C:/Files_For_QT/TMV-710/trial_Gcode.txt");


    m_program = new QOpenGLShaderProgram;  //shader program object

    //add shaders
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);

    // link shaders
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();
    //Link all of the loaded shaders together

    m_program->bind();
    //Bind the shader so that it is the current active shader

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");
    m_colorLoc = m_program->uniformLocation("color");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    //To use a VAO, all you have to do is bind the VAO

    // Setup our vertex buffer object.
    m_geoemtryVbo.create();
    //Create a buffer for dynamic allocation later

    m_geoemtryVbo.bind();
    //Bind the buffer so that it is the current active buffer

    m_geoemtryVbo.allocate(m_geometry.constData(), m_geometry.totalCount() * sizeof(GLfloat));
    //Allocate and initialize the information

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();


    m_camera.setToIdentity();
    //    qDebug()<<m_geometry.getX_avg()<<""<<m_geometry.getY_avg()<<""<<m_geometry.getZ_avg();  //auto center
    //        m_camera.translate(m_geometry.getX_avg(),m_geometry.getY_avg(), m_geometry.getZ_avg());

    m_camera.translate(m_xTran,m_yTran, m_zTran);   //for machine tool

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(-2000, 2000, 2000));

    //Set default color for the component
    m_program->setUniformValue(m_colorLoc, m_color);

    m_program->release();
}

void MyOpenGLWidget::paintGL()
{

    //    deltaTime = timer.restart();
    //    qDebug()<<deltaTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //clear the widget using the background color and depth information that we defined in the initializeGL() function
    //otherwise we would still see the results from the previous iteration

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(-90.0f, 1, 0, 0);        //Transform to intuitive machine tool coordinate
    m_world.rotate(m_xRot / 16.0f, 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    m_X_Translate= m_world;
    m_Y_Translate= m_world;
    m_Z_Translate= m_world;

    m_camera.setToIdentity();
    m_camera.translate(m_xTran,m_yTran,m_zTran) ;

    //create object               //base->y->x->a->c   //base->z->spindle
    createObject(structureOrder,motionForEachAxis);

    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    //x axis
    glBegin( GL_LINES );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 600, 0., 0. );

    // arrow
    glVertex3f(600, 0.0f, 0.0f);
    glVertex3f(500, 100, 0.0f);

    glVertex3f(600, 0.0f, 0.0f);
    glVertex3f(500, -100, 0.0f);
    glEnd();
    glFlush();

    //y axis
    glBegin( GL_LINES );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., 400., 0. );

    // arrow
    glVertex3f(0, 400.0f, 0.0f);
    glVertex3f(0, 300, 100.0f);

    glVertex3f(0, 400.0f, 0.0f);
    glVertex3f(0, 300, -100.0f);
    glEnd();
    glFlush();

    //z axis
    glBegin( GL_LINES );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0, 0., 400. );

    // arrow
    glVertex3f(0, 0.0f, 400.0f);
    glVertex3f(0, 100, 300.0f);

    glVertex3f(0, 0.0f, 400.0f);
    glVertex3f(0, -100, 300.0f);
    glEnd();
    glFlush();

    m_program->release();
}

void MyOpenGLWidget::createObject(QVector<QString> structureOrder,QVector<float> motionForEachAxis)
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();

    //Set new color for the component
    m_program->setUniformValue(m_colorLoc, m_color);

    //    structureOrder<<"BASE"<<"Y"<<"X"<<"A"<<"Z"<<"B"<<"SPINDLE";


    for(int i = 0;i<structureOrder.length();++i){
        if(structureOrder.at(i)=="BASE") //Base
        {
            m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
            m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, m_geometry.totalCount_BASE());
            startNubmer=m_geometry.totalCount_BASE();
        }

        if(structureOrder.at(i)=="X") //X
        {
            if(structureOrder.at(i-1)!="X")
            {
                if(structureOrder.at(i-1)=="BASE")
                    m_X_Translate = m_world;

                if(structureOrder.at(i-1)=="Y")
                    m_X_Translate = m_Y_Translate;


                m_X_Translate.translate(motionForEachAxis.at(0),0,0);
                normalMatrix = m_X_Translate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_X_Translate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_X());

                startNubmer+=m_geometry.totalCount_X();
            }
        }

        if(structureOrder.at(i)=="Y") //Y
        {
            if(structureOrder.at(i-1)!="Y")
            {
                if(structureOrder.at(i-1)=="BASE")
                    m_Y_Translate = m_world;

                if(structureOrder.at(i-1)=="X")
                    m_Y_Translate = m_X_Translate;

                m_Y_Translate.translate(0,motionForEachAxis.at(1),0);
                normalMatrix = m_Y_Translate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_Y_Translate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_Y());

                if(structureOrder.at(i-1)!="Y")
                    startNubmer+=m_geometry.totalCount_Y();
            }
        }

        if(structureOrder.at(i)=="Z") //Z
        {
            if(structureOrder.at(i-1)!="Z")
            {
                m_Z_Translate.translate(0,0,motionForEachAxis.at(2));
                normalMatrix = m_Z_Translate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_Z_Translate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_Z());

                if(structureOrder.at(i-1)!="Z")
                    startNubmer+=m_geometry.totalCount_Z();
            }
        }

        if(structureOrder.at(i)=="A") //A
        {
            if(structureOrder.at(i-1)!="A")
            {
                if(structureOrder.at(i-1)=="X")
                    m_A_Rotate=m_X_Translate;

                if(structureOrder.at(i-1)=="Y")
                    m_A_Rotate=m_Y_Translate;

                if(structureOrder.at(i-1)=="Z")
                    m_A_Rotate=m_Z_Translate;

                if(structureOrder.at(i-1)=="B")
                    m_A_Rotate=m_B_Rotate;

                if(structureOrder.at(i-1)=="C")
                    m_A_Rotate=m_C_Rotate;


                m_A_Rotate.translate(0.0f,0.0f,0.0f);  //translate the coordinate center to the circle center
                m_A_Rotate.rotate(motionForEachAxis.at(3), 1, 0, 0);
                m_A_Rotate.translate(0.0f,0.0f,-0.0f);  //translate back to the original center

                normalMatrix = m_A_Rotate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_A_Rotate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_A());
                startNubmer+=m_geometry.totalCount_A();
            }
        }

        if(structureOrder.at(i)=="B") //B
        {
            if(structureOrder.at(i-1)!="B")
            {
                if(structureOrder.at(i-1)=="X")
                    m_B_Rotate=m_X_Translate;

                if(structureOrder.at(i-1)=="Y")
                    m_B_Rotate=m_Y_Translate;

                if(structureOrder.at(i-1)=="Z")
                    m_B_Rotate=m_Z_Translate;

                if(structureOrder.at(i-1)=="A")
                    m_B_Rotate=m_A_Rotate;

                if(structureOrder.at(i-1)=="C")
                    m_B_Rotate=m_C_Rotate;

                m_B_Rotate.translate(0.0f,0.0f,0.0f);  //translate the coordinate center to the circle center
                m_B_Rotate.rotate(motionForEachAxis.at(4), 0, 1, 0);
                m_B_Rotate.translate(0.0f,0.0f,0.0f);  //translate back to the original center

                normalMatrix = m_B_Rotate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc,m_camera * m_B_Rotate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_B());
                startNubmer+=m_geometry.totalCount_B();
            }
        }

        if(structureOrder.at(i)=="C") //C
        {
            if(structureOrder.at(i-1)!="C")
            {
                if(structureOrder.at(i-1)=="X")
                    m_C_Rotate=m_X_Translate;

                if(structureOrder.at(i-1)=="Y")
                    m_C_Rotate=m_Y_Translate;

                if(structureOrder.at(i-1)=="Z")
                    m_A_Rotate=m_Z_Translate;

                if(structureOrder.at(i-1)=="A")
                    m_C_Rotate=m_A_Rotate;

                if(structureOrder.at(i-1)=="B")
                    m_C_Rotate=m_B_Rotate;

                m_C_Rotate.translate(0.0f,0.0f,0.0f);  //translate the coordinate center to the circle center
                m_C_Rotate.rotate(motionForEachAxis.at(5), 0, 0, 1);
                m_C_Rotate.translate(0.0f,0.0f,0.0f);  //translate back to the original center

                normalMatrix = m_C_Rotate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc,m_camera * m_C_Rotate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_C());
                startNubmer+=m_geometry.totalCount_C();
            }
        }

        if(structureOrder.at(i)=="SPINDLE") //SPINDLE
        {
            if(structureOrder.at(i-1)!="SPINDLE")
            {
                if(structureOrder.at(i-1)=="A")
                    m_SPINDLE_Translate=m_A_Rotate;

                if(structureOrder.at(i-1)=="B")
                    m_SPINDLE_Translate=m_B_Rotate;

                if(structureOrder.at(i-1)=="Z")
                    m_SPINDLE_Translate=m_Z_Translate;


                normalMatrix = m_SPINDLE_Translate.normalMatrix();
                m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
                m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_SPINDLE_Translate);

                glDrawArrays(GL_TRIANGLES, startNubmer, m_geometry.totalCount_SPINDLE());
            }
        }
    }
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(fov, GLfloat(w) / h, 1000.0f, -1000.0f);

    //void perspective(float verticalAngle(angle of view), float
    //aspectRatio, float nearPlane, float farPlane);
}

void MyOpenGLWidget::moveComponent(const QVector<GLfloat> data, int count)
{
    //    qDebug()<<"count_nc_line"<<count_nc_line;
    //    qDebug()<<"count"<<count;
    if(count_nc_line<count){
        float x = data[count_nc_line + 0];
        if (x>moveX)
        {
            moveX+=1.0f;
        }
        else if(x<moveX){
            moveX-=1.0f;
        }else if (x==moveX){
            moveX = x;
        }

        float y = data[count_nc_line + 1];
        if (y>moveY)
        {
            moveY+=1.0f;
        }
        else if(y<moveY){
            moveY-=1.0f;
        }else if (y==moveY){
            moveY = y;
        }

        float z = data[count_nc_line + 2];
        if (z>moveZ)
        {
            moveZ+=1.0f;
        }
        else if(z<moveZ){
            moveZ-=1.0f;
        }else if (z==moveZ){
            moveZ = z;
        }

        float a = data[count_nc_line + 3];
        if (a>moveA)
        {
            moveA+=1.0f;
        }
        else if(a<moveA){
            moveA-=1.0f;
        }else if (a==moveA){
            moveA = a;
        }

        float c = data[count_nc_line + 4];
        if (c>moveC)
        {
            moveC+=5.0f;
        }
        else if(c<moveC){
            moveC-=5.0f;
        }else if (c==moveC){
            moveC = c;
        }
        update();
        //        qDebug()<<"x = "<<moveX<<"y = "<<moveY<<"z = "<<moveZ<<"a = "<<moveA<<"c = "<<moveC;
    }

    if(moveX == data[count_nc_line + 0] & moveY == data[count_nc_line + 1] &
            moveZ == data[count_nc_line + 2] & moveA == data[count_nc_line + 3] & moveC == data[count_nc_line + 4]
            & count_nc_line<count-6)
    {
        count_nc_line = count_nc_line + 6;
    }

}

void MyOpenGLWidget::timerEvent(QTimerEvent *e)
{
    moveComponent(m_gcode.constData(), m_gcode.totalCount());
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    else if (event->buttons() & Qt::RightButton) {
        setXTranslation(m_xTran + 8 * dx);
        setYTranslation(m_yTran - 8 * dy);     //STIL HAVE PROBLEM HERE!!!!!
    }
    m_lastPos = event->pos();
}

void MyOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    setZoom(event->delta()/30);

}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){

    case Qt::Key_Up:
        moveY+=5;
        motionForEachAxis[1]=moveY;
        update();
        break;
    case Qt::Key_Down:
        moveY-=5;
        motionForEachAxis[1]=moveY;
        update();
        break;
    case Qt::Key_Left:
        moveX-=5;
        motionForEachAxis[0]=moveX;
        update();
        break;
    case Qt::Key_Right:
        moveX+=5;
        motionForEachAxis[0]=moveX;
        update();
        break;

    case Qt::Key_W:
        moveA+=5;
        motionForEachAxis[3]=moveA;
        update();
        break;
    case Qt::Key_S:
        moveA-=5;
        motionForEachAxis[3]=moveA;
        update();
        break;
    case Qt::Key_A:
        moveC-=5;
        motionForEachAxis[5]=moveC;
        moveB-=5;
        motionForEachAxis[4]=moveB;
        update();
        break;
    case Qt::Key_D:
        moveC+=5;
        motionForEachAxis[5]=moveC;
        moveB+=5;
        motionForEachAxis[4]=moveB;
        update();
        break;
    case Qt::Key_O:
        moveZ+=5;
        motionForEachAxis[2]=moveZ;
        update();
        break;
    case Qt::Key_L:
        moveZ-=5;
        motionForEachAxis[2]=moveZ;
        update();
        break;
    }
}

void MyOpenGLWidget::setupVertexAttribs()  //specified how OpenGL should interpret the vertex data
{
    m_geoemtryVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_geoemtryVbo.release();
}

