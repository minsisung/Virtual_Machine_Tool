#include "machineTool.h"
#include "tinyxml2.h"
#include <iostream>
#include "joint.h"
#include "link.h"
#include <QVector>
#include <cassert>


using namespace tinyxml2;
using namespace std;

MachineTool::MachineTool(){}


Link* MachineTool::find_link(std::string linkName, QVector<Link> &myVector)
{               //The function to find the link for joint and return a pointer

    for (QVector<Link>::iterator loop = myVector.begin();loop != myVector.end(); loop++)
    {
        if(loop->getName()== linkName)
            return loop;

        if(loop == myVector.end() && myVector.end()->getName() != linkName)//assert  can not find the name
            assert (false && "Can not find this link in the link vector");
    }

    return nullptr;
}

int MachineTool::readURDF(const char* filename){
    XMLDocument doc;
    int res = doc.LoadFile(filename);

    if (res != 0)
    {
        cout << "load xml file failed" << endl;
        return res;
    }
    XMLElement *MT = doc.RootElement();
    const XMLAttribute *nameOfMT = MT->FirstAttribute();
    cout << "Machine Tool: "<< nameOfMT->Name() << ":" << nameOfMT->Value() << endl << endl;
    m_name = nameOfMT->Value();

    XMLElement *link_count = MT->FirstChildElement("link");

    while (link_count)
    {
        const XMLAttribute *nameOfLink = link_count->FirstAttribute();
        XMLElement *visual = link_count->FirstChildElement("visual");

        if (visual == nullptr)                                          //if there is no visual element
        {
            Link link_reading(nameOfLink->Value());
            LinkVector.push_back(link_reading);                         //push into the link vector

            link_count = link_count->NextSiblingElement("link");        //move to next link
            continue;
        }

        XMLElement *origin = visual->FirstChildElement("origin");
        const XMLAttribute *xyz = origin->FirstAttribute();
        const XMLAttribute *rpy = xyz->Next();

        Vector3 xyz_input;
        xyz_input.init(xyz->Value());

        Vector3 rpy_input;
        rpy_input.init(rpy->Value());

        XMLElement *geometry = visual->FirstChildElement("geometry");
        XMLElement *mesh = geometry->FirstChildElement("mesh");
        const XMLAttribute *fileName = mesh->FirstAttribute();

        XMLElement *material = visual->FirstChildElement("material");
        XMLElement *color = material->FirstChildElement("color");
        const XMLAttribute *rgba = color->FirstAttribute();

        VectorRGBA rgba_input;
        rgba_input.initRGBA(rgba->Value());

        Link link_reading(nameOfLink->Value(), xyz_input, rpy_input,
                          fileName->Value(), rgba_input);
        LinkVector.push_back(link_reading);                                    //push link into the vector

        link_count = link_count->NextSiblingElement("link");                  //move to next sibling element
    }

    XMLElement *joint = MT->FirstChildElement("joint");

    while (joint)
    {
        const XMLAttribute *nameOfJoint = joint->FirstAttribute();
        const XMLAttribute *typeOfJoint = joint->FirstAttribute()->Next();
        XMLElement *parent_link = joint->FirstChildElement("parent");  //parent link
        XMLElement *child_link = joint->FirstChildElement("child");    //child link
        const XMLAttribute *parent = parent_link->FirstAttribute();
        const XMLAttribute *child = child_link->FirstAttribute();

        XMLElement *origin = joint->FirstChildElement("origin");
        if (origin == nullptr)                                              //for some joint without origin element
        {
            Joint joint_reading(nameOfJoint->Value(), typeOfJoint->Value(),
                                find_link(parent->Value(),LinkVector),find_link(child->Value(),LinkVector));
            JointVector.push_back(joint_reading);                           //push into the joint vector

            joint = joint->NextSiblingElement("joint");
            continue;
        }
        const XMLAttribute *xyz = origin->FirstAttribute();
        const XMLAttribute *rpy = xyz->Next();

        Vector3 xyz_input;
        xyz_input.init(xyz->Value());

        xyz_input.x = 1000*xyz_input.x;     //meter to minimeter
        xyz_input.y = 1000*xyz_input.y;
        xyz_input.z = 1000*xyz_input.z;

        Vector3 rpy_input;
        rpy_input.init(rpy->Value());

        XMLElement *axis = joint->FirstChildElement("axis");           //axis
        const XMLAttribute *axisAttribute = axis->FirstAttribute();
        //        XMLElement *limit = joint->FirstChildElement("limit");
        //        const XMLAttribute *limit_lower = limit->FirstAttribute();   //lower limit  do it later
        //        const XMLAttribute *limit_upper = limit_lower->Next();       //upper limit


        Vector3 axis_input;
        axis_input.init(axisAttribute->Value());

        Joint joint_reading(nameOfJoint->Value(), typeOfJoint->Value(), xyz_input, rpy_input, axis_input,
                            find_link(parent->Value(),LinkVector),find_link(child->Value(),LinkVector));
        JointVector.push_back(joint_reading);                           //push into the joint vector

        joint_reading.getParentLink()->ChildLink = joint_reading.getChildLink();
        //assign child link to the parent link

        joint_reading.getChildLink()->ParentLink = joint_reading.getParentLink();
        //assign parent link to the child link

        joint = joint->NextSiblingElement("joint");  //move to next sibling element
    }
    return 0;
}

QDebug operator<<(QDebug stream, const MachineTool &MT)
{
    stream<< "Machine Tool: "<< MT.m_name<<endl<<endl;

    for (QVector<const Link>::iterator loop = MT.LinkVector.begin();loop != MT.LinkVector.end(); loop++)        //print out all links information
    {
        stream<<"Name of the link: "<<QString::fromStdString(loop->getName())<<endl;
        stream<<"xyz:  "<<loop->getOrigin_xyz().x<<" "<<loop->getOrigin_xyz().y<<" "<<loop->getOrigin_xyz().z<<endl;
        stream<<"rpy:  "<<loop->getOrigin_rpy().x<<" "<<loop->getOrigin_rpy().y<<" "<<loop->getOrigin_rpy().z<<endl;
        stream<<"STL file Name:"<<QString::fromStdString(loop->getMeshFile())<<endl<<endl;
    }
    return stream;
}





