#include "readgcode.h"

ReadGCode::ReadGCode():
    m_totalCount(0)
{

}

void ReadGCode::read(QString filepath)
{
    GLfloat x{0};
    GLfloat y{0};
    GLfloat z{0};
    GLfloat a{0};
    GLfloat c{0};
    GLfloat f{0};
    QStringList s;
    QString line;


    QFile inputFile(filepath);
    inputFile.open(QIODevice::ReadOnly);
    assert (inputFile.isOpen());   //assert the file can not be opened

    QTextStream stream(&inputFile);
    line = stream.readLine();
    while (!line.isNull()){
        s = line.split(" ");
        x=s.at(0).toFloat();            // not accurate
        y=s.at(1).toFloat();
        z=s.at(2).toFloat();
        a=s.at(3).toFloat();
        c=s.at(4).toFloat();
        f=s.at(5).toFloat();
        add(QVector3D(x, y, z), QVector3D(a, c, f));
        line = stream.readLine();
    } ;
    inputFile.close();
}

void ReadGCode::add(const QVector3D &Tr, const QVector3D &Ro_Fr)
{
    m_data.resize(m_totalCount+6);
    GLfloat *p = m_data.data() + m_totalCount;
    *p++ = Tr.x();
    *p++ = Tr.y();
    *p++ = Tr.z();
    *p++ = Ro_Fr.x();
    *p++ = Ro_Fr.y();
    *p++ = Ro_Fr.z();
    m_totalCount += 6;
}
