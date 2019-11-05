#ifndef VECTORRGBA_H
#define VECTORRGBA_H
#include<vector3.h>


class VectorRGBA: public Vector3   //inherit from Vector3
{
public:
    VectorRGBA(double _r,double _g, double _b, double _a) {this->r=_r;this->g=_g;this->b=_b
                ;this->a=_a;}
    VectorRGBA() {this->clear();}
    double r;
    double g;
    double b;
    double a;

    void clear() {this->r=this->g=this->b=this->a=0.0;}

    void initRGBA(const std::string &vector_str)
    {
      this->clear();
      std::vector<std::string> pieces;
      std::vector<double> rgba;
      split_string( pieces, vector_str, " ");
      for (unsigned int i = 0; i < pieces.size(); ++i){
        if (pieces[i] != ""){
            rgba.push_back(strToDouble(pieces[i].c_str()));
        }
      }
      this->r = rgba[0];
      this->g = rgba[1];
      this->b = rgba[2];
      this->a = rgba[3];
    }

};

#endif // VECTORRGBA_H
