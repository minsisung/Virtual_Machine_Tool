#ifndef READURDF
#define READURDF

#include "joint.h"
#include "link.h"
#include <QVector>

class MachineTool
{

public:
    MachineTool();
    int readURDF(const char* filename);
    QVector<Link> LinkVector;                                            // create a vector for links
    QVector<Joint> JointVector;                                              // create a vector for joints
private:
    Link* find_link(std::string linkName, QVector<Link> &myVector);
};
#endif
