#pragma once

#include <array>
#include <cmath>
#include <cstdint>

/**
 * @struct Vec2
 * @brief 2-component vector for texture coordinates and 2D positions.
 */
struct Vec2 {
  float x, y;

  Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

  Vec2 operator+(const Vec2 &v) const { return {x + v.x, y + v.y}; }
  Vec2 operator-(const Vec2 &v) const { return {x - v.x, y - v.y}; }
  Vec2 operator*(float s) const { return {x * s, y * s}; }
};

/**
 * @struct Vec3
 * @brief 3-component vector for basic geometry and directions.
 */
struct Vec3 {
  float x, y, z;

  Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

  /** @brief Cross product: Find a vector perpendicular to two others (Essential
   * for Normals). */
  static Vec3 Cross(const Vec3 &a, const Vec3 &b);

public:
  Vec3 operator+(const Vec3 &v) const;
  Vec3 operator-(const Vec3 &v) const;
  Vec3 operator*(float s) const;
};

/**
 * @struct Vec4
 * @brief A 4-component vector used for positions (w=1) and directions (w=0).
 */
struct Vec4 {
  float x, y, z, w;

  Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

  Vec4(const Vec3 &v, float w);

  float Dot(const Vec4 &other) const;

public:
  float &operator[](int);
  float operator[](int) const;
};

/**
 * @struct Mat4
 * @brief A 4x4 matrix stored in column-major order for OpenGL compatibility.
 */
struct Mat4 {
  std::array<float, 16> elements;

  Mat4();

  /** @brief Translation: Moves an object by (x, y, z). */
  static Mat4 Translate(const Vec3 &v);

  /** @brief Scale: Resizes an object. */
  static Mat4 Scale(const Vec3 &v);

  /** @brief Rotate around the Y axis (radians). */
  static Mat4 RotateY(float angle);

  /** @brief Perspective Projection matrix logic would go here. */

public:
  float &operator()(int col, int row);
  const float &operator()(int col, int row) const;
  Mat4 operator*(const Mat4 &other) const;
};
