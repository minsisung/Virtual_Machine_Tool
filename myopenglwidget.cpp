#include "myopenglwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <shaders.h>
#include <QtOpenGL>
#include <qdebug.h>
#include "machineTool.h"
#include <iostream>

using namespace std;

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


    //create machine tool
    MT.readURDF("umc500_report3.urdf");
    qDebug()<<MT<<endl;

    //            cout<<"LINKS: "<<endl;
    //            for (QVector<Link>::iterator loop = MT.LinkVector.begin();loop != MT.LinkVector.end(); loop++)        //print out all links information
    //            {
    //                qDebug() <<"Name of the link: "<<QString::fromStdString(loop->getName())<<endl;
    //                qDebug() <<"xyz:  "<<loop->getOrigin_xyz().x<<" "<<loop->getOrigin_xyz().y<<" "<<loop->getOrigin_xyz().z<<endl;
    //                qDebug() <<"rpy:  "<<loop->getOrigin_rpy().x<<" "<<loop->getOrigin_rpy().y<<" "<<loop->getOrigin_rpy().z<<endl;
    //                qDebug() <<"STL file Name:"<<QString::fromStdString(loop->getMeshFile())<<endl;
    //                qDebug() <<"address of the link: "<<loop<<endl<<endl;
    //                qDebug() << "transformation matrix: "<<loop->m_TransformMatrix<<endl<<endl;
    //            }

    //        cout<<"JOINTS: "<<endl;
    //        for (QVector<Joint>::iterator loop = MT.JointVector.begin();loop != MT.JointVector.end(); loop++)        //print out all links information
    //        {
    //            cout <<"Name of the Joint: "<<loop->getName()<< "  " <<"Type: " <<loop->getType()<<endl;
    //            cout <<"xyz:  "<<loop->getOrigin_xyz().x<<" "<<loop->getOrigin_xyz().y<<" "<<loop->getOrigin_xyz().z<<endl;
    //            cout <<"rpy:  "<<loop->getOrigin_rpy().x<<" "<<loop->getOrigin_rpy().y<<" "<<loop->getOrigin_rpy().z<<endl;
    //            cout <<"axis: "<<loop->getAxis().x<<" "<<loop->getAxis().y<<" "<<loop->getAxis().z<<" "<<endl;
    //            cout <<"Parent Link Name: "<< loop->getParentLink()->getName()<<" "<< "Parent Link Address: "<<loop->getParentLink()<<endl;
    //            cout <<"Child Link Name: "<< loop->getChildLink()->getName()<<" "<<" Child Link Address: "<< loop->getChildLink()<<endl<<endl;
    //        }
    motionForEachAxis<<moveX<<moveY<<moveZ<<moveA<<moveB<<moveC;
    //    read stl for each link from URDF
    for (QVector<Link>::iterator loop = MT.LinkVector.begin();loop != MT.LinkVector.end(); loop++)//print out all links information
    {
        m_geometry.URDFreadSTL(QString::fromStdString(loop->getMeshFile()),loop);
        //        qDebug() <<"Name of the link: "<<QString::fromStdString(loop->getName());
        //        qDebug() << "number of the vertex: " << loop->numberOfVertex << endl;
    }

    //        //read NC Code
    //        m_gcode.read("C:/Files_For_QT/TMV-710/trial_Gcode.txt");

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

    //create object
    createURDFObject();

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

void MyOpenGLWidget::createURDFObject()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();

    //Set new color for the component
    m_program->setUniformValue(m_colorLoc, m_color);
    //draw each link
    for (QVector<Joint>::iterator loop = MT.JointVector.begin();loop != MT.JointVector.end(); loop++) //print out all links information
    {
        if(loop == MT.JointVector.begin())
        {
            Link *ParentLink = loop->getParentLink();
            ParentLink->m_TransformMatrix = m_world;

            m_program->setUniformValue(m_mvMatrixLoc, m_camera * ParentLink->m_TransformMatrix);
            m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

            glDrawArrays(GL_TRIANGLES, startNubmer, loop->getParentLink()->numberOfVertex);
            startNubmer += loop->getParentLink()->numberOfVertex;
            //            qDebug() << QString::fromStdString(loop->getParentLink()->getName());
            //            qDebug() << loop->getParentLink()->numberOfVertex;
            //            qDebug()<<startNubmer<<endl;

            Link *ChildLink = moveChildLink(loop);

            normalMatrix = (ChildLink->m_TransformMatrix).normalMatrix();
            m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
            m_program->setUniformValue(m_mvMatrixLoc, m_camera * (ChildLink->m_TransformMatrix));

            glDrawArrays(GL_TRIANGLES, startNubmer, loop->getChildLink()->numberOfVertex);
            startNubmer += loop->getChildLink()->numberOfVertex;
            //            qDebug() << QString::fromStdString(loop->getChildLink()->getName());
            //            qDebug() << loop->getChildLink()->numberOfVertex;
            //            qDebug()<<startNubmer<<endl;
            continue;
        }
        Link *ChildLink = moveChildLink(loop);

        normalMatrix = (ChildLink->m_TransformMatrix).normalMatrix();
        m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
        m_program->setUniformValue(m_mvMatrixLoc, m_camera * (ChildLink->m_TransformMatrix));

        glDrawArrays(GL_TRIANGLES, startNubmer, loop->getChildLink()->numberOfVertex);
        startNubmer += loop->getChildLink()->numberOfVertex;
        //        qDebug() << QString::fromStdString(loop->getChildLink()->getName());
        //        qDebug() << loop->getChildLink()->numberOfVertex;
        //        qDebug()<<startNubmer<<endl;
    }
    startNubmer=0;    //SET BACK TO ZERO
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(fov, GLfloat(w) / h, 1000.0f, -1000.0f);

    //void perspective(float verticalAngle(angle of view), float
    //aspectRatio, float nearPlane, float farPlane);
}

Link *MyOpenGLWidget::moveChildLink(Joint *joint)
{
    Link *ParentLink = joint->getParentLink();
    Link *ChildLink = joint->getChildLink();

    if (joint->getType() == "prismatic")
    {
        ChildLink->m_TransformMatrix = ParentLink->m_TransformMatrix;
        ChildLink->m_TransformMatrix.translate(joint->getOrigin_xyz().x,joint->getOrigin_xyz().y,joint->getOrigin_xyz().z);
        joint->getAxis();
        ChildLink->m_TransformMatrix.translate(joint->getAxis().x * joint->translational_motion
                                               ,joint->getAxis().y * joint->translational_motion,joint->getAxis().z * joint->translational_motion);
    }

    if (joint->getType() == "fixed")
    {
        ChildLink->m_TransformMatrix = ParentLink->m_TransformMatrix;
        ChildLink->m_TransformMatrix.translate(joint->getOrigin_xyz().x,joint->getOrigin_xyz().y,joint->getOrigin_xyz().z);

    }

    if (joint->getType() == "revolute")
    {
        ChildLink->m_TransformMatrix = ParentLink->m_TransformMatrix;
        ChildLink->m_TransformMatrix.translate(joint->getOrigin_xyz().x,joint->getOrigin_xyz().y,joint->getOrigin_xyz().z);
        joint->getAxis();
/*        ChildLink->m_TransformMatrix.rotate(motionForEachAxis.at(5), joint->getAxis().x
                                            ,joint->getAxis().y,joint->getAxis().z);  */    //?????????????
                ChildLink->m_TransformMatrix.rotate(joint->rotational_motion, joint->getAxis().x
                                                    ,joint->getAxis().y,joint->getAxis().z);

    }
    return ChildLink;
}

void MyOpenGLWidget::moveComponent(const QVector<GLfloat> data, int count)
{
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
        MT.JointVector[4].translational_motion = moveY;
        update();
        break;
    case Qt::Key_Down:
        moveY-=5;
        MT.JointVector[4].translational_motion = moveY;
        update();
        break;
    case Qt::Key_Left:
        moveX-=5;
        MT.JointVector[3].translational_motion = moveX;
        update();
        break;
    case Qt::Key_Right:
        moveX+=5;
        MT.JointVector[3].translational_motion = moveX;
        update();
        break;

    case Qt::Key_W:
        moveA+=5;
        MT.JointVector[0].rotational_motion = moveA;
        update();
        break;
    case Qt::Key_S:
        moveA-=5;
        MT.JointVector[0].rotational_motion = moveA;
        update();
        break;
    case Qt::Key_A:
        moveC-=5;
        MT.JointVector[1].rotational_motion = moveC;
        update();
        break;
    case Qt::Key_D:
        moveC+=5;
        MT.JointVector[1].rotational_motion = moveC;
        update();
        break;
    case Qt::Key_O:
        moveZ+=5;
        MT.JointVector[5].translational_motion = moveZ;
        update();
        break;
    case Qt::Key_L:
        moveZ-=5;
        MT.JointVector[5].translational_motion = moveZ;
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
