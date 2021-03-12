#pragma once
#include "GL/glew.h"
#include "GLMath.h"
#include <string>
#include <vector>
#include"Program.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Utility.h"

struct Vertex {
	Vec3 position;
	Vec3 normal;
	Vec2 texCoord;
};

struct MeshTexture {
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh {
private:
	VertexArray* va = nullptr;
	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;

	std::vector<Vertex>* vertices = nullptr;
	std::vector<unsigned int>* indices = nullptr;
	std::vector<MeshTexture>* textures = nullptr;

	Material material;

public:
	Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<MeshTexture>* textures);
	Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<MeshTexture>* textures, Material material);
	~Mesh();

	void draw(Program& program);

};