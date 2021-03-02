#include <vector>
#include "GLMath.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Program.h"
#include "Camera/Camera.h"
#include "Texture.h"

class Sphere {
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

	unsigned int vSize = 0;
	unsigned int iSize = 0;
	unsigned int ilSize = 0;

	unsigned int segment = 10;

	Matrix4x4 model;
	Vec3 pos = { 0,0,0 };
	Vec3 scale = { 1,1,1 };
	Vec3 rotation = { 0,0,0 };

	float radius = 1;

	Vec3 color = {1,1,1};

	DrawMode mode = DrawMode::TRIANGLE;

	Texture* texture = nullptr;
public:
	Sphere();
	Sphere(const Vec3 pos,const float radius);
	Sphere(const Vec3 pos,const Vec3 scale,const Vec3 rotation,const float radius);

	Sphere(const float radius,const unsigned int segment);
	~Sphere();

	void init();

	void draw(Camera& camera);
	void setDrawMode(DrawMode mode);
	
	void setSegment(const unsigned int segment);
	unsigned int getSegment();
	
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

