#pragma once
#include <buffers/VertexBuffer.h>
#include <buffers/VertexBufferLayout.h>

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};

