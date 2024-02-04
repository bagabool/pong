#pragma once
#include <glad/glad.h>
#include "VBLayout.h"
#include "VBO.h"
#include "EBO.h"

class VAO
{
private:
	unsigned int id;
public:
	VAO();
	~VAO();

	void Append(const VBO& vbo, const VBLayout& layout);

	void Bind();
	void Unbind();
};

