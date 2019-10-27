#ifndef READURDF
#define READURDF

#include<link.h>
#include <vector3.h>

class MachineTool
{

public:
    MachineTool();
    int readURDF(const char* filename);
private:
    Link* find_link(std::string linkName, QVector<Link> &myVector);

};
#endif
