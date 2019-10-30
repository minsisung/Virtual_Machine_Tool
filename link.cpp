#include "link.h"

Link::Link(){}
Link::Link(std::string name):m_name(name){}
     //constructor
Link::Link(std::string name, Vector3 origin_xyz,Vector3 origin_rpy, std::string meshfile):
    m_name(name),m_origin_xyz(origin_xyz), m_origin_rpy(origin_rpy),m_meshfile(meshfile) {}

std::string Link::getMeshFile() {

    std::size_t pos = m_meshfile.find("meshes/");
    std::string short_meshfile = m_meshfile.substr(pos+7);   //only return name of the stl file

    return short_meshfile;
}


Vector3 axis;
Link *m_parent_link;
Link *m_child_link;
