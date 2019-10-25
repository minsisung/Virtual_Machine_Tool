#ifndef LINK_H
#define LINK_H
#include <string>
#include <QVector3D>
#include <vector3.h>

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

    Vector3 getOrigin_xyz() {return m_origin_xyz;}
    Vector3 getOrigin_rpy() {return m_origin_rpy;}
    std::string getMeshFile() {return m_meshfile;}
    std::string getName() {return m_name;}
};

#endif // LINK_H



