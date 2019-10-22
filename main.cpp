#include "mainwindow.h"
#include <QApplication>
#include<QDebug>

#include "tinyxml2.h"
#include <iostream>
using namespace tinyxml2;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //QWidget widget;


    XMLDocument doc;
    int res = doc.LoadFile("testURDF.urdf");

    if (res != 0)
    {
        cout << "load xml file failed" << endl;
        return res;
    }
    XMLElement *robot = doc.RootElement();
    cout << robot->Name() << endl;
    XMLElement *link = robot->FirstChildElement("link");
    XMLElement *joint = robot->FirstChildElement("joint");
        while (link)
        {
            XMLElement *visual = link->FirstChildElement("visual");
            if (visual == nullptr)   //for some link without visual element
            {
                link = link->NextSiblingElement();
                continue;
            }

            cout << "Link " << endl;
            const XMLAttribute *nameOfLink = link->FirstAttribute();
            cout << nameOfLink->Name() << ":" << nameOfLink->Value() << endl;

            XMLElement *origin = visual->FirstChildElement("origin");
            const XMLAttribute *xyz = origin->FirstAttribute();
            const XMLAttribute *rpy = xyz->Next();
            cout  << xyz->Name() << ":" << xyz->Value() << endl;
            cout  << rpy->Name() << ":" << rpy->Value() << endl<<endl;

            link = link->NextSiblingElement();  //move to next sibling element
        }

        while (joint)
        {
            cout << "joint " << endl;
            const XMLAttribute *nameOfJoint = joint->FirstAttribute();
            const XMLAttribute *typeOfJoint = joint->FirstAttribute()->Next();
            cout << nameOfJoint->Name() << ":" << nameOfJoint->Value() << endl;
            cout << typeOfJoint->Name() << ":" << typeOfJoint->Value() << endl;


            XMLElement *origin = joint->FirstChildElement("origin");
            if (origin == nullptr)   //for some joint without origin element
            {
                joint = joint->NextSiblingElement();
                continue;
            }

            XMLElement *parent_link = joint->FirstChildElement("parent");
            XMLElement *child_link = joint->FirstChildElement("child");
            XMLElement *axis = joint->FirstChildElement("axis");

            const XMLAttribute *xyz = origin->FirstAttribute();
            const XMLAttribute *rpy = xyz->Next();

            cout << xyz->Name() << ":" << xyz->Value() << endl;
            cout << rpy->Name() << ":" << rpy->Value() << endl;

            const XMLAttribute *parent = parent_link->FirstAttribute();
            const XMLAttribute *child = child_link->FirstAttribute();
            const XMLAttribute *axisAttribute = axis->FirstAttribute();

            cout << "parent_"<< parent->Name() << ":" << parent->Value() << endl;
            cout << "child_"<< child->Name() << ":" << child->Value() << endl;
            cout << axisAttribute->Name() << ":" << axisAttribute->Value() << endl<<endl;

            joint = joint->NextSiblingElement();  //move to next sibling element
        }



    w.resize(1024,768);
    w.show();
    return a.exec();
}
