#pragma once
#include <GL/glew.h>
#include <Macros.h>

class VertexBuffer
{
private:
	GLuint m_RendererID;
	GLuint m_Size;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

