#pragma once
#include <glad/glad.h>

class VBO
{
private:
	unsigned int id;
public:
	VBO(const void* data, int size);
	VBO(const void* data, int size, int mode);
	~VBO();

	void Bind() const;
	void Unbind() const;
};

