#ifndef LINK_H
#define LINK_H
#include <string>
#include <QVector3D>
#include <vector3.h>
#include <QMatrix4x4>


class Link
{
private:
      std::string m_name;
      Vector3 m_origin_xyz;
      Vector3 m_origin_rpy;
      std::string m_meshfile;

public:
    Link();             //constructor
    Link(std::string name);
    Link(std::string name, Vector3 origin_xyz,Vector3 origin_rpy, std::string meshfile);
    ~Link(){}

    Link *ParentLink = nullptr;
    Link *ChildLink = nullptr;

    Vector3 getOrigin_xyz() {return m_origin_xyz;}
    Vector3 getOrigin_rpy() {return m_origin_rpy;}
    std::string getMeshFile();
    std::string getName() {return m_name;}
    int numberOfVertex = 0;
    QMatrix4x4 m_TransformMatrix;
};

#endif // LINK_H



