#pragma once
#include "Program.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include <vector>
#include "Texture.h"

class Skybox {
	
public:
	Skybox();
	~Skybox();

	void draw();

private:
	Texture* textures;
	std::vector<float> vertices;
	VertexArray va;
	VertexBuffer* vb;
	Program* shader;
};