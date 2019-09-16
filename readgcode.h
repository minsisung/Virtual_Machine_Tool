#ifndef READGCODE_H
#define READGCODE_H

#include <QString>
#include <QFile>
#include<qtextstream.h>
#include <QVector>
#include <QVector3D>
#include <qopengl.h>
#include <QDebug>



class ReadGCode
{
public:
    ReadGCode();

    const QVector<GLfloat> constData() const { return m_data; }
    int totalCount() const { return m_totalCount; }
    void read(QString path);

private:
    void add(const QVector3D &T, const QVector3D &R_FR);
    QVector<GLfloat> m_data;
    int m_totalCount;
};

#endif // READGCODE_H
