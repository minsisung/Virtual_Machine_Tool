#include "link.h"

Link::Link(){}
Link::Link(std::string name):m_name(name){}
     //constructor
Link::Link(std::string name, Vector3 origin_xyz,Vector3 origin_rpy, std::string meshfile):
    m_name(name),m_origin_xyz(origin_xyz), m_origin_rpy(origin_rpy),m_meshfile(meshfile) {}


Vector3 axis;
Link *m_parent_link;
Link *m_child_link;
