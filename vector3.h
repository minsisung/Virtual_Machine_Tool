#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>
#include <sstream>
#include <QVector3D>


static inline double strToDouble(const char *in)
{
  std::stringstream ss;
  ss.imbue(std::locale::classic());

  ss << in;

  double out;
  ss >> out;

  if (ss.fail() || !ss.eof()) {
    throw std::runtime_error("Failed converting string to double");
  }

  return out;
}


class Vector3
{
public:
  Vector3(double _x,double _y, double _z) {this->x=_x;this->y=_y;this->z=_z;}
  Vector3() {this->clear();}
  double x;
  double y;
  double z;

  void clear() {this->x=this->y=this->z=0.0;}
  void init(const std::string &vector_str)
  {
    this->clear();
    std::vector<std::string> pieces;
    std::vector<double> xyz;
    split_string( pieces, vector_str, " ");
    for (unsigned int i = 0; i < pieces.size(); ++i){
      if (pieces[i] != ""){
          xyz.push_back(strToDouble(pieces[i].c_str()));
      }
    }
    this->x = xyz[0];
    this->y = xyz[1];
    this->z = xyz[2];
  }

  void split_string(std::vector<std::string> &result,
                    const std::string &input,
                    const std::string &isAnyOf)
  {
    std::string::size_type start = 0;
    std::string::size_type end = input.find_first_of(isAnyOf, start);
    while (end != std::string::npos)
    {
      result.push_back(input.substr(start, end-start));
      start = end + 1;
      end = input.find_first_of(isAnyOf, start);
    }
    if (start < input.length())
    {
      result.push_back(input.substr(start));
    }
  }
};
#endif // VECTOR3_H
