#pragma once

class Texture {
private:
	unsigned int id;
	const char* filePath;
	int width, height;
	int bpp;

public:
	Texture(const char* path);
	~Texture();

	int getWidth();
	int getHeight();

	void bind(const unsigned int slot = 0);
	void bind2(const unsigned int slot = 0);
	void unbind();

	unsigned int getId();

};