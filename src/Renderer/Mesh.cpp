#include "Renderer/Mesh.hpp"
#include "Core/Logger.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <cstddef>
#include <stdexcept>

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices) {
  if (vertices.empty()) {
    CORE_WARN("Creating a Mesh with 0 vertices.");
  }
  if (indices.empty()) {
    CORE_WARN("Creating a Mesh with 0 indices. Nothing will be drawn.");
  }

  m_IndexCount = static_cast<uint32_t>(indices.size());

  // Generate buffers
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  if (m_VAO == 0 || m_VBO == 0 || m_EBO == 0) {
    throw std::runtime_error("Failed to generate OpenGL buffers for Mesh");
  }

  glBindVertexArray(m_VAO);

  // Upload vertex data
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // Upload index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
               indices.data(), GL_STATIC_DRAW);

  // Define layout: Position (Attrib 0)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));

  // Define layout: Normal (Attrib 1)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));

  // Define layout: Texture Coordinates (Attrib 2)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw() const {
  if (m_IndexCount == 0) {
    return;
  }
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh() {
  if (m_VAO != 0)
    glDeleteVertexArrays(1, &m_VAO);
  if (m_VBO != 0)
    glDeleteBuffers(1, &m_VBO);
  if (m_EBO != 0)
    glDeleteBuffers(1, &m_EBO);
}

Mesh::Mesh(Mesh &&other) noexcept
    : m_VAO(other.m_VAO), m_VBO(other.m_VBO), m_EBO(other.m_EBO),
      m_IndexCount(other.m_IndexCount) {
  other.m_VAO = 0;
  other.m_VBO = 0;
  other.m_EBO = 0;
  other.m_IndexCount = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
  if (this != &other) {
    if (m_VAO != 0)
      glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0)
      glDeleteBuffers(1, &m_VBO);
    if (m_EBO != 0)
      glDeleteBuffers(1, &m_EBO);

    m_VAO = other.m_VAO;
    m_VBO = other.m_VBO;
    m_EBO = other.m_EBO;
    m_IndexCount = other.m_IndexCount;

    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_EBO = 0;
    other.m_IndexCount = 0;
  }
  return *this;
}
