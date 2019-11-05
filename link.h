#ifndef LINK_H
#define LINK_H
#include <string>
#include <QVector3D>
#include <vector3.h>
#include <vectorRGBA.h>
#include <QMatrix4x4>


class Link
{
private:
      std::string m_name;
      Vector3 m_origin_xyz;
      Vector3 m_origin_rpy;
      std::string m_meshfile;
      VectorRGBA m_rgba;

public:
    Link();             //constructor
    Link(std::string name);
    Link(std::string name, Vector3 origin_xyz,Vector3 origin_rpy, std::string meshfile
         ,VectorRGBA rgba);
    ~Link(){}

    Link *ParentLink = nullptr;
    Link *ChildLink = nullptr;

    Vector3 getOrigin_xyz() const{return m_origin_xyz;}
    Vector3 getOrigin_rpy() const{return m_origin_rpy;}
    VectorRGBA getRGBA() const{return m_rgba;}
    std::string getMeshFile()const;
    std::string getName() const{return m_name;}
    int numberOfVertex = 0;
    QMatrix4x4 m_TransformMatrix;
    Vector3 axis;
    Link *m_parent_link;
    Link *m_child_link;
};

#endif // LINK_H



