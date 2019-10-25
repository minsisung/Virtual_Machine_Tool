#ifndef JOINT_H
#define JOINT_H
#include <QVector3D>
#include <link.h>

class Joint
{
private:
    std::string m_name;
    std::string m_type;
    Vector3 m_origin_xyz;
    Vector3 m_origin_rpy;
    Vector3 m_axis;
    Link *m_parent_link;
    Link *m_child_link;
    double m_lower_limit;
    double m_upper_limit;



public:
    Joint();            //constructor
    Joint(std::string name , std::string type);
    Joint(std::string name,std::string type, Vector3 origin_xyz,Vector3 origin_rpy, Vector3 axis,
        Link *parent_link,  Link *child_link);
    ~Joint(){}

    Vector3 getOrigin_xyz() {return m_origin_xyz;}
    Vector3 getOrigin_rpy() {return m_origin_rpy;}
    Vector3 getAxis() {return m_axis;}
    std::string getName() {return m_name;}
    std::string getType() {return m_type;}
    Link* getParentLink() {return m_parent_link;}
    Link* getChildLink() {return m_child_link;}

};

#endif // JOINT_H
