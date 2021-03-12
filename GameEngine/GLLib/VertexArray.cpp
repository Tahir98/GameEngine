#pragma once
#include "VertexArray.h"
#include "GL/glew.h"

VertexArray::VertexArray(){
	glCreateVertexArrays(1,&id);
}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1,&id);
}

void VertexArray::bind(){
	glBindVertexArray(id);
}

void VertexArray::unBind(){
	glBindVertexArray(0);
}

void VertexArray::addVertexAttribute(const unsigned int count, const unsigned int type, const bool normalized) {
	if (count > 0 && VertexElement::getTypeSize(type) != -1) {
		stride += count * VertexElement::getTypeSize(type);
		unsigned int offset = 0;
		elements.push_back({ count , type , normalized });
		for (unsigned int i = 0; i < elements.size(); i++) {
			bind();
			glVertexAttribPointer(i,elements[i].count, elements[i].type, elements[i].normalized,stride,(const void*)offset);
			glEnableVertexAttribArray(i);
			offset += elements[i].count * VertexElement::getTypeSize(elements[i].type);
		}
	}
}
