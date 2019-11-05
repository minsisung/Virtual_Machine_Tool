#ifndef READURDF
#define READURDF

#include "joint.h"
#include "link.h"
#include <QVector>
#include <iostream>
#include<QDebug>
#include<vectorRGBA.h>

class MachineTool
{

public:
    MachineTool();
    int readURDF(const char* filename);
    QVector<Link> LinkVector;                 // create a vector for links
    QVector<Joint> JointVector;               // create a vector for joints
    QString m_name;
    friend QDebug operator<<(QDebug stream, const MachineTool& MT);   //overloading operator <<

private:
    Link* find_link(std::string linkName, QVector<Link> &myVector);

};
#endif
