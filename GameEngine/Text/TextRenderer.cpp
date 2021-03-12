#pragma once
#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include "Core.h"

TextRenderer::TextRenderer(const char* path,unsigned int pixelSize) : path(path){
	
	program = new Program("Shaders/freetype.shader");
	va = new VertexArray();
	va->bind();

	vb = new VertexBuffer(nullptr,10000,GL_DYNAMIC_DRAW);
	vb->bind();
	va->addVertexAttribute(4,GL_FLOAT,false);
	

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		isExist = false;
		return;
	}
		
	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		isExist = false;
		return;
	}
	
	FT_Set_Pixel_Sizes(face, 0, pixelSize);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	for (unsigned char c = 0; c < 128; c++) {
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		
		// Generate texture
		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,0,
			GL_RED,GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Now store character for later use
		_Character character = {texture,
		face->glyph->bitmap.width, face->glyph->bitmap.rows,
		face->glyph->bitmap_left, face->glyph->bitmap_top,
		face->glyph->advance.x
		};

		characters.insert(std::pair<char, _Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	projection = GLMath::ortho(0, Application::width, 0, Application::height, -5, 5);
	isExist = true;
}

TextRenderer::~TextRenderer(){
	delete program;
	delete va;
	delete vb;

	for (unsigned int i = 0; i < characters.size(); i++) {
		glDeleteTextures(1, &characters[i].TextureID);
	}
}

void TextRenderer::write(std::string text, float x, float y, float scale, Vec3 color){
	if (isExist) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		program->bind();
		va->bind();
		vb->bind();

		program->setUniform3f("textColor", color.x, color.y, color.z);
		program->setUniformMatrix4fv("projection", 1, false, projection.m[0]);
		program->setUniformi("text", 0);

		glActiveTexture(GL_TEXTURE0);

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			_Character ch = characters[*c];
			float xpos = x + ch.bearingX * scale;
			float ypos = y - (ch.sizeY - ch.bearingY) * scale;

			float w = ch.sizeX * scale;
			float h = ch.sizeY * scale;

			float vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			vb->subData(0, sizeof(vertices), vertices[0]);

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number

			x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in
		}
	}
	
}
