#include "GLMath.h"
#include <vector>
#include <iostream>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Program.h"
#include "IndexBuffer.h"
#include "Camera/Camera.h"

struct Vertex2 {
	Vec3 pos;
	Vec3 color;
	Vec3 normal;
};

class Cube {
private: 
	Vec3 pos = { 0,0,0 };
	Vec3 scale = { 1,1,1 };
	Vec3 rotation = { 0,0,0 };

	Matrix4x4 model;

	float* positions = nullptr;
	float* normals = nullptr;
	unsigned int* indices = nullptr;

	Program program;
	VertexArray va;
	VertexBuffer* vb = nullptr;

	std::vector<Vertex2> vertices;

public:
	Cube();
	Cube(Vec3 pos);
	Cube(Vec3 pos, Vec3 scale, Vec3 rotation);

	~Cube();

	void setPosition(Vec3 pos);
	Vec3 getPosition();

	void setScale(Vec3 scale);
	Vec3 getScale();

	void rotate(Vec3 rot);
	void setRotation(Vec3 rot);

	void draw(Camera& camera);

};