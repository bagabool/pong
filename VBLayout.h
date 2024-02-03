#pragma once
#include <vector>
#include <glad/glad.h>

struct VBElement {
	unsigned int count;
	unsigned int type;
	unsigned char normalize;

	static unsigned int SizeOf(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT:         return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}
};

class VBLayout
{
private:
	std::vector<VBElement> elements;
	unsigned int stride;
public:
	VBLayout() : stride(0) {};

	template<typename T>
	void Push(unsigned int count)
	{}

	inline const std::vector<VBElement> Elements() const
	{
		return elements;
	}

	inline unsigned int Stride() const
	{
		return stride;
	}
};

template<>
inline void VBLayout::Push<float>(unsigned int count)
{
	elements.push_back({ count, GL_FLOAT, GL_FALSE });
	stride += VBElement::SizeOf(GL_FLOAT) * count;
}

template<>
inline void VBLayout::Push<unsigned int>(unsigned int count)
{
	elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
	stride += VBElement::SizeOf(GL_UNSIGNED_INT) * count;
}

template<>
inline void VBLayout::Push<unsigned char>(unsigned int count)
{
	elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
	stride += VBElement::SizeOf(GL_UNSIGNED_BYTE) * count;
}

