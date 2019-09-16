#ifndef CREATEGEMOETRY_H
#define CREATEGEMOETRY_H


#include <qopengl.h>
#include <QVector>
#include <QVector3D>

class CreateGemoetry
{
public:
    CreateGemoetry();

    const GLfloat *constData() const { return m_data.constData(); }
    int totalCount() const { return m_totalCount; }
    int totocalVertexCount() const { return m_totalCount / 6; }
    int totalCount_BASE() const { return m_totalCount_BASE; }
    int totalCount_X() const { return m_totalCount_X; }
    int totalCount_Y() const { return m_totalCount_Y; }
    int totalCount_Z() const { return m_totalCount_Z; }
    int totalCount_A() const { return m_totalCount_A; }
    int totalCount_B() const { return m_totalCount_B; }
    int totalCount_C() const { return m_totalCount_C; }
    int totalCount_SPINDLE() const { return m_totalCount_SPINDLE; }



    float getX_avg() const {return (max_x+min_x)/2;}
    float getY_avg() const {return (max_y+min_y)/2;}
    float getZ_avg() const {return (max_z+min_z)/2;}

    void readSTL(QString part, QString path);



private:
    void tri(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat nx, GLfloat ny, GLfloat nz);
    void add(const QVector3D &v, const QVector3D &n);

    QVector<GLfloat> m_data;
    int m_totalCount;
    int m_totalCount_BASE =0;
    int m_totalCount_X =0;
    int m_totalCount_Y =0;
    int m_totalCount_Z =0;
    int m_totalCount_A =0;
    int m_totalCount_B =0;
    int m_totalCount_C =0;
    int m_totalCount_SPINDLE =0;
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
    QStringList myParts;
};

#endif // CREATEGEMOETRY_H
