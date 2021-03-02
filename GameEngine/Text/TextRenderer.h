#pragma once
#include "Program.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "GLMath.h"
#include <string>
#include <map>


class TextRenderer{
	struct _Character {
		uint32_t TextureID; // ID handle of the glyph texture
		uint32_t sizeX, sizeY; // Size of glyph
		uint32_t bearingX, bearingY; // Offset from baseline to left/top of glyph
		uint32_t Advance; // Offset to advance to next glyph
	};
	
private:
	const char* path;

	Program* program = nullptr; 
	VertexArray* va = nullptr;
	VertexBuffer* vb = nullptr;

	std::map<char, _Character> characters;

	Matrix4x4 projection;
	bool isExist = false;

public:
	TextRenderer(const char* path,unsigned int pixelSize);
	~TextRenderer();
	
	void write(std::string text, float x, float y, float scale, Vec3 color);
};

