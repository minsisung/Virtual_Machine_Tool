#include "creategemoetry.h"
#include <qfile.h>
#include<qtextstream.h>
#include<qdebug.h>
#include<assert.h>

CreateGemoetry::CreateGemoetry():
    m_totalCount(0),

    min_x(1000000),max_x(-1000000),
    min_y(1000000),max_y(-1000000),
    min_z(1000000),max_z(-1000000)

{
    myParts <<"BASE"<<"X"<<"Y"<<"Z"<<"A"<<"B"<<"C"<<"SPINDLE";
}

void CreateGemoetry::add(const QVector3D &v, const QVector3D &n)
{
    m_data.resize(m_totalCount+6);
    GLfloat *p = m_data.data() + m_totalCount;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_totalCount += 6;
}

void CreateGemoetry::readSTL(QString part, QString filepath)
{
    GLfloat x1{0};
    GLfloat y1{0};
    GLfloat z1{0};
    GLfloat x2{0};
    GLfloat y2{0};
    GLfloat z2{0};
    GLfloat x3{0};
    GLfloat y3{0};
    GLfloat z3{0};
    GLfloat nx{0};
    GLfloat ny{0};
    GLfloat nz{0};

    QString line;
    QString short_line;
    QStringList s;
    QString srch;

    QFile inputFile(filepath);
    inputFile.open(QIODevice::ReadOnly);
    assert (inputFile.isOpen());   //assert the file can not be opened
    QTextStream stream(&inputFile);


    do {
        line = stream.readLine();
        if( line.contains( "facet normal" ))
        {
            //            srch="facet normal";
            //            int startfrom = line.indexOf(srch)+srch.length()+1;
            short_line = line.mid(16);
            s = short_line.split(" ");
            nx=s.at(0).toFloat();            // not accurate
            ny=s.at(1).toFloat();
            nz=s.at(2).toFloat();
        }
        if ( line.contains( "vertex" ))
        {
            //srch="vertex";
            //int startfrom = line.indexOf(srch)+srch.length()+1;
            short_line = line.mid(16);                          // STILL HAS PROBLEM HERE
            s = short_line.split(" ");
            x1=s.at(0).toFloat();            // not accurate
            y1=s.at(1).toFloat();
            z1=s.at(2).toFloat();

            line = stream.readLine(); // go next line
            short_line = line.mid(16);
            s = short_line.split(" ");
            x2=s.at(0).toFloat();            // not accurate
            y2=s.at(1).toFloat();
            z2=s.at(2).toFloat();

            line = stream.readLine(); // go next line
            short_line = line.mid(16);
            s = short_line.split(" ");
            x3=s.at(0).toFloat();            // not accurate
            y3=s.at(1).toFloat();
            z3=s.at(2).toFloat();

            tri(x1,y1,z1,x2,y2,z2,x3,y3,z3,nx,ny,nz);

            switch(myParts.indexOf(part))
            {
            case 0: //Base
                m_totalCount_BASE+=3;
                break;

            case 1: //X
                m_totalCount_X+=3;
                break;

            case 2: //Y
                m_totalCount_Y+=3;
                break;

            case 3: //Z
                m_totalCount_Z+=3;
                break;

            case 4: //A
                m_totalCount_A+=3;
                break;

            case 5: //B
                m_totalCount_B+=3;
                break;

            case 6: //C
                m_totalCount_C+=3;
                break;

            case 7: //SPINDLE
                m_totalCount_SPINDLE+=3;
                break;
            }
            //qDebug()<<m_data.size();
            //qDebug()<<nx<<""<<ny<<""<<nz;
            //qDebug()<<x1<<""<<y1<<""<<z1;
            //qDebug()<<x2<<""<<y2<<""<<z2;
            //qDebug()<<x3<<""<<y3<<""<<z3;

        }
    } while(!line.isNull());
    inputFile.close();
}

void CreateGemoetry::tri(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat nx, GLfloat ny, GLfloat nz)
{
    QVector3D n=QVector3D(nx, ny, nz);

    add(QVector3D(x1, y1, z1), n);
    add(QVector3D(x2, y2, z2), n);
    add(QVector3D(x3, y3, z3), n);

    if(x1<min_x){ min_x=x1;}
    if(y1<min_y){ min_y=y1;}
    if(z1<min_z){ min_z=z1;}
    if(x1>max_x){ max_x=x1;}
    if(y1>max_y){ max_y=y1;}
    if(z1>max_z){ max_z=z1;}
}
