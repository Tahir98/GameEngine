#pragma once

class VertexBuffer {
private:
	unsigned int id;
public:
	VertexBuffer(const float* vertices, const unsigned int size, const unsigned int usage);
	~VertexBuffer();

	void subData(const unsigned int index,const unsigned int size, const float* vertices);

	void bind();
	void unBind();
};