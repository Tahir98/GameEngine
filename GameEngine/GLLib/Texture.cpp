#include "Texture.h"
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>


Texture::Texture(const char* path) : filePath(path),width(0),height(0),bpp(0),id(0){

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(filePath,&width,&height,&bpp,4);

	if (image == nullptr) {
		std::cout << "Image not found" << std::endl;
	}
	else {
		glGenTextures(1,&id);

		glBindTexture(GL_TEXTURE_2D,id);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		std::cout << path << " width :" << width << "  height: " << height << " bpp: " << bpp << std::endl;

		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
	}
}

Texture::~Texture(){
	glDeleteTextures(1,&id);
	std::cout << "Deleted: " << filePath << std::endl;
}

int Texture::getWidth(){
	return width;
}

int Texture::getHeight() {
	return height;
}

void Texture::bind(const unsigned int slot ){
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind2(const unsigned int slot){
	glBindTextureUnit(slot, id);
}

void Texture::unbind(){
	glBindTexture(GL_TEXTURE_2D,0);
}

unsigned int Texture::getId(){
	return id;
}
