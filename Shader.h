#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	unsigned int id;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 trans) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
};

inline void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
inline void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
inline void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
inline void Shader::setMat4(const std::string& name, glm::mat4 trans) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}
inline void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
