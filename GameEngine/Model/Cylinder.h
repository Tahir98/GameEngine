#include <vector>
#include "GLMath.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Program.h"
#include "Camera/Camera.h"

class Cylinder {
public:
	enum DrawMode {
		POINT = 0, LINE = 1, TRIANGLE = 2
	};

private:
	VertexArray va;
	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;
	IndexBuffer* ibl = nullptr;

	Program* program = nullptr;

	std::vector<Vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;

	unsigned int segment = 100;

	Matrix4x4 model;
	Vec3 pos = { 0,0,0 };
	Vec3 scale = { 1,1,1 };
	Vec3 rotation = { 0,0,0 };

	float radius = 5;
	float height = 10;

	Vec3 color = { 1,0,0 };

	DrawMode mode = DrawMode::TRIANGLE;

public:
	Cylinder();
	Cylinder(const Vec3 pos, const float radius, const float height);
	Cylinder(const Vec3 pos, const Vec3 scale, const Vec3 rotation);
	Cylinder(const float radius, const float height, const unsigned int segment);

	~Cylinder();

	void init();

	void draw(Camera& camera);
	void setDrawMode(DrawMode mode);

	void setSegment(const unsigned int segment);
	unsigned int getSegment();

	void setRadius(const float radius);
	float getRadius();

	void setHeight(const float height);
	float getHeight();

	void setSize(const float radius, const float height);

	void setPosition(const Vec3 pos);
	Vec3 getPosition();

	void setScale(const Vec3 scale);
	Vec3 getScale();

	void rotate(Vec3 rot);
	void setRotation(Vec3 rotation);
	Vec3 getRotation();

	void setColor(Vec3 color);
	Vec3 getColor();
};