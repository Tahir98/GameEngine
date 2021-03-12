#pragma once
#include "IndexBuffer.h"
#include "GL/glew.h"

IndexBuffer::IndexBuffer(const unsigned int* indices, const unsigned int size,const unsigned int usage){
	glGenBuffers(1,&id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,indices,usage);
}

IndexBuffer::~IndexBuffer(){
	glDeleteBuffers(1,&id);
}

void IndexBuffer::subData(const unsigned int index, const unsigned int size, const unsigned int* indices){
	bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index, size, indices);
}

void IndexBuffer::bind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
}

void IndexBuffer::unBind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
