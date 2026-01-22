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

Vec3 Vec3::operator+(const Vec3 &v) const {
  return {x + v.x, y + v.y, z + v.z};
}
Vec3 Vec3::operator-(const Vec3 &v) const {
  return {x - v.x, y - v.y, z - v.z};
}
Vec3 Vec3::operator*(float s) const { return {x * s, y * s, z * s}; }

Mat4::Mat4() {
  elements.fill(0.0f);
  elements[0] = 1.0f;
  elements[5] = 1.0f;
  elements[10] = 1.0f;
  elements[15] = 1.0f;
}

Mat4 Mat4::Scale(const Vec3 &v) {
  Mat4 res;
  res.elements[0] = v.x;
  res.elements[5] = v.y;
  res.elements[10] = v.z;
  return res;
}

Mat4 Mat4::RotateY(float angle) {
  Mat4 res;
  float c = std::cos(angle);
  float s = std::sin(angle);
  res.elements[0] = c;
  res.elements[2] = -s;
  res.elements[8] = s;
  res.elements[10] = c;
  return res;
}

Mat4 Mat4::operator*(const Mat4 &other) const {
  Mat4 res;
  res.elements.fill(0.0f);
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      for (int k = 0; k < 4; ++k) {
        res.elements[col * 4 + row] +=
            elements[k * 4 + row] * other.elements[col * 4 + k];
      }
    }
  }
  return res;
}
