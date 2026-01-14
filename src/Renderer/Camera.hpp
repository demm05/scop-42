#pragma once

#include "Math/Math.hpp"

/**
 * @class Camera
 * @brief Responsible for calculating the View and Projection matrices.
 * * The Camera defines how we view the 3D scene. It converts coordinates
 * from World Space to View Space (via the View matrix) and then to
 * Clip Space (via the Projection matrix).
 */
class Camera {
public:
  /**
   * @brief Constructs a perspective camera.
   * @param fov Field of View in degrees.
   * @param aspectRatio Width / Height of the window.
   * @param near Closest distance to render.
   * @param far Furthest distance to render.
   */
  Camera(float fov, float aspectRatio, float near, float far);

  /** @brief Updates the View matrix based on position and target. */
  void LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

  /** @brief Returns the combined View-Projection matrix for the Shader. */
  const Mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

  // Setters for movement
  void SetPosition(const Vec3 &position);
  const Vec3 &GetPosition() const { return m_Position; }

private:
  void UpdateViewMatrix();
  void UpdateProjectionMatrix();

private:
  Mat4 m_ProjectionMatrix;
  Mat4 m_ViewMatrix;
  Mat4 m_ViewProjectionMatrix;

  Vec3 m_Position = {0.0f, 0.0f, 0.0f};
  Vec3 m_Rotation = {0.0f, 0.0f, 0.0f}; // Euler angles

  float m_FOV, m_AspectRatio, m_Near, m_Far;
};
