#include "VBO.h"

VBO::VBO(const void* data, int size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &id);
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
