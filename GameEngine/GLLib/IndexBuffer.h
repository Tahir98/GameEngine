#pragma once

class IndexBuffer {
private:
	unsigned int id;


public:
	IndexBuffer(const unsigned int* indices,const unsigned int size,const unsigned int usage);
	~IndexBuffer();

	void subData(const unsigned int index, const unsigned int size, const unsigned int* indices);
	void bind();
	void unBind();
};