#include "urdfparser.h"
#include "tinyxml2.h"
#include <iostream>
#include "joint.h"
#include "link.h"
#include <QVector>
#include "assert.h"

using namespace tinyxml2;
using namespace std;

Link* find_link(std::string linkName, QVector<Link> &myVector){               //The function to find the link for joint and return a pointer

    for (QVector<Link>::iterator loop = myVector.begin();loop != myVector.end(); loop++)
    {
        if(loop->getName()== linkName)
            return loop;
    }
    assert (true);   //assert  can not find the name
    return nullptr;
}

int readURDF(const char* filename){
    XMLDocument doc;
    int res = doc.LoadFile(filename);

    if (res != 0)
    {
        cout << "load xml file failed" << endl;
        return res;
    }
    XMLElement *robot = doc.RootElement();
    const XMLAttribute *nameOfRobot = robot->FirstAttribute();
    cout << "robot "<< nameOfRobot->Name() << ":" << nameOfRobot->Value() << endl << endl;

    QVector<Link> LinkVector;                                            // create a vector for links

    XMLElement *link_count = robot->FirstChildElement("link");
    cout << "Link " << endl;
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


        Link link_reading(nameOfLink->Value(), xyz_input, rpy_input,fileName->Value());
        LinkVector.push_back(link_reading);                                    //push link into the vector

        link_count = link_count->NextSiblingElement("link");                  //move to next sibling element
    }

    for (QVector<Link>::iterator loop = LinkVector.begin();loop != LinkVector.end(); loop++)        //print out all links information
    {
        cout <<"Name of the link: "<<loop->getName()<<endl;
        cout <<"xyz:  "<<loop->getOrigin_xyz().x<<" "<<loop->getOrigin_xyz().y<<" "<<loop->getOrigin_xyz().z<<endl;
        cout <<"rpy:  "<<loop->getOrigin_rpy().x<<" "<<loop->getOrigin_rpy().y<<" "<<loop->getOrigin_rpy().z<<endl;
        cout <<"file path:"<<loop->getMeshFile()<<endl;
        cout <<"address of the link: "<<loop<<endl<<endl;
    }
    cout<<endl;

    XMLElement *joint = robot->FirstChildElement("joint");

    QVector<Joint> JointVector;                                              // create a vector for joints

    cout << "joint " << endl;
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
            cout << endl;
            continue;
        }

        const XMLAttribute *xyz = origin->FirstAttribute();
        const XMLAttribute *rpy = xyz->Next();

        Vector3 xyz_input;
        xyz_input.init(xyz->Value());

        Vector3 rpy_input;
        rpy_input.init(rpy->Value());


        XMLElement *axis = joint->FirstChildElement("axis");           //axis
        const XMLAttribute *axisAttribute = axis->FirstAttribute();
        //        XMLElement *limit = joint->FirstChildElement("limit");                    //limit   (do it later)
        //        const XMLAttribute *limit_lower = limit->FirstAttribute();   //lower limit
        //        const XMLAttribute *limit_upper = limit_lower->Next();       //upper limit


        Vector3 axis_input;
        axis_input.init(axisAttribute->Value());

        Joint joint_reading(nameOfJoint->Value(), typeOfJoint->Value(), xyz_input, rpy_input, axis_input,
                            find_link(parent->Value(),LinkVector),find_link(child->Value(),LinkVector));
        JointVector.push_back(joint_reading);                           //push into the joint vector

        //        cout << limit_lower->Name() << "_limit :" << limit_lower->Value() << endl;
        //        cout << limit_upper->Name() << "_limit:" << limit_upper->Value() << endl << endl;

        joint = joint->NextSiblingElement("joint");  //move to next sibling element
    }


    for (QVector<Joint>::iterator loop = JointVector.begin();loop != JointVector.end(); loop++)        //print out all links information
    {
        cout <<"Name of the Joint: "<<loop->getName()<< "  " <<"Type: " <<loop->getType()<<endl;
        cout <<"xyz:  "<<loop->getOrigin_xyz().x<<" "<<loop->getOrigin_xyz().y<<" "<<loop->getOrigin_xyz().z<<endl;
        cout <<"rpy:  "<<loop->getOrigin_rpy().x<<" "<<loop->getOrigin_rpy().y<<" "<<loop->getOrigin_rpy().z<<endl;
        cout <<"axis: "<<loop->getAxis().x<<" "<<loop->getAxis().y<<" "<<loop->getAxis().z<<" "<<endl;
        cout <<"Parent Link Name: "<< loop->getParentLink()->getName()<<" "<< "Parent Link Address: "<<loop->getParentLink()<<endl;
        cout <<"Child Link Name: "<< loop->getChildLink()->getName()<<" "<<" Child Link Address: "<< loop->getChildLink()<<endl<<endl;
    }

    return 0;
}


