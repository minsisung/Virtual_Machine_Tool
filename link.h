#ifndef LINK_H
#define LINK_H
#include <string>
#include <QVector3D>

class Link
{
private:
      std::string m_name;
      QVector3D m_origin;
      const char* m_meshfile;

public:
    Link();
    void setOrigin(const QVector3D);
    void setMeshfile(const char*);
    QVector3D getOrigin() {return m_origin;}
    char getMeshFile() {return *m_meshfile;}
};

#endif // LINK_H



