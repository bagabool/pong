#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
}

void VAO::Append(const VBO& vbo, const VBLayout& layout)
{
	Bind();
	vbo.Bind();
	int offset = 0;
	for (size_t i = 0; auto element : layout.Elements())
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalize, layout.Stride(), (void*)offset);
		offset += element.count * VBElement::SizeOf(element.type);
	}
}

void VAO::Bind()
{
	glBindVertexArray(id);
}
