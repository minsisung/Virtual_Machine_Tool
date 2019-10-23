#include "link.h"

Link::Link():m_origin(0,0,0), m_meshfile(nullptr){}

void Link::setOrigin(const QVector3D origin)
{
 m_origin.setX( origin.x());
 m_origin.setY( origin.y());
 m_origin.setZ( origin.z());

}

void Link::setMeshfile(const char * meshfile)
{
m_meshfile = meshfile;
}

