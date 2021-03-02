#include "VertexBuffer.h"
#include "GL/glew.h"
#include <iostream>


VertexBuffer::VertexBuffer(const float* vertices, const unsigned int size,const unsigned int usage){
	glGenBuffers(1,&id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1,&id);
}

void VertexBuffer::subData(const unsigned int index, const unsigned int size, const float* vertices){
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glBufferSubData(GL_ARRAY_BUFFER,index,size,vertices);
}

void VertexBuffer::bind(){
	glBindBuffer(GL_ARRAY_BUFFER, id);

}

void VertexBuffer::unBind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
