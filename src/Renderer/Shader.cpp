#include "Shader.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include "Utils/MappedFile.hpp"

namespace {

/**
 * @brief Helper to compile a single shader stage.
 */
std::expected<GLuint, std::string> CompileShader(std::string_view source,
                                                 GLenum type) {
  GLuint shader = glCreateShader(type);
  const char *src = source.data();
  GLint length = static_cast<GLint>(source.length());

  glShaderSource(shader, 1, &src, &length);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::string infoLog(infoLogLength, '\0');
    glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());
    glDeleteShader(shader);
    return std::unexpected("Shader Compilation Error: " + infoLog);
  }
  return shader;
}

/**
 * @brief Helper to link vertex and fragment shaders into a program.
 */
std::expected<GLuint, std::string> LinkProgram(GLuint vShader, GLuint fShader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::string infoLog(infoLogLength, '\0');
    glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());
    glDeleteProgram(program);
    return std::unexpected("Shader Linking Error: " + infoLog);
  }

  // Once linked, we can detach the shaders
  glDetachShader(program, vShader);
  glDetachShader(program, fShader);

  return program;
}

} // namespace

Shader::~Shader() {
  if (m_RendererID != 0) {
    glDeleteProgram(m_RendererID);
  }
}

Shader::Shader(Shader &&other) noexcept : m_RendererID(other.m_RendererID) {
  other.m_RendererID = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    if (m_RendererID != 0) {
      glDeleteProgram(m_RendererID);
    }
    m_RendererID = other.m_RendererID;
    other.m_RendererID = 0;
  }
  return *this;
}

Shader::Result Shader::FromFile(const std::filesystem::path &vertexPath,
                                const std::filesystem::path &fragmentPath) {
  auto vertexFile = MappedFile::open(vertexPath);
  if (!vertexFile)
    return std::unexpected("Could not open vertex shader: " +
                           vertexPath.string());

  auto fragmentFile = MappedFile::open(fragmentPath);
  if (!fragmentFile)
    return std::unexpected("Could not open fragment shader: " +
                           fragmentPath.string());

  return FromSource(vertexFile->view(), fragmentFile->view());
}

Shader::Result Shader::FromSource(std::string_view vertexSrc,
                                  std::string_view fragmentSrc) {
  auto vShader = CompileShader(vertexSrc, GL_VERTEX_SHADER);
  if (!vShader)
    return std::unexpected(vShader.error());

  auto fShader = CompileShader(fragmentSrc, GL_FRAGMENT_SHADER);
  if (!fShader) {
    glDeleteShader(*vShader);
    return std::unexpected(fShader.error());
  }

  auto program = LinkProgram(*vShader, *fShader);

  glDeleteShader(*vShader);
  glDeleteShader(*fShader);

  if (!program)
    return std::unexpected(program.error());

  return Shader(*program);
}

void Shader::Bind() const { glUseProgram(m_RendererID); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetUniformMat4(const std::string &name, const Mat4 &matrix) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location != -1) {
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.elements.data());
  }
}
