#include "Math.hpp"

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vec4::Vec4(const Vec3 &v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

float &Vec4::operator[](int i) {
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    return w;
  }
}

float Vec4::operator[](int i) const {
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    return w;
  }
}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
