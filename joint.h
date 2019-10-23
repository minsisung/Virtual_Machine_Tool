#ifndef JOINT_H
#define JOINT_H
#include <QVector3D>
#include <link.h>

class Joint
{
private:
    std::string name;
    std::string type;
    const QVector3D origin;
    const QVector3D axis;
    Link *parent_link;
    Link *child_link;
    void setOrigin();
    void setAxis();


public:
    Joint();
};

#endif // JOINT_H
