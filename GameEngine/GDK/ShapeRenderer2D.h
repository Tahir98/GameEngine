#pragma once
#include "Program.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h" 
#include <vector>
#include "Color.h"
#include "GLMath.h"

enum ShapeType {
	FILLED,LINE
};

class ShapeRenderer2D {
private:
	Program program;
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	const unsigned int segments = 30;//Default circle segments

	Color color = { 1,1,1,1 };
	ShapeType shapeType;

	float toRadian(float degree);

	bool sBegin = false;

	inline void addColor();// it sets the class's Color to vertex
	inline void addColor(Color c1);// it sets the sended Color to vertex

public:

	ShapeRenderer2D();
	~ShapeRenderer2D();

	void begin(ShapeType shapeType);
	void end();

	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3,Color color1, Color color2, Color color3);
	void rect(float x, float y, float width, float height);
	void rect(float x, float y, float width, float height, Color color1, Color color2, Color color3, Color color4);
	void rect(float x, float y, float width, float height, float degree);
	void rect(float x, float y, float width, float height, float degree, Color color1, Color color2, Color color3, Color color4);

	void circle(float x,float y,float radius);
	void circle(float x, float y, float radius,const unsigned int segments);
	void circle(float x, float y, float radius,const unsigned int segments,Color center,Color edge);

	void ellipse(float x, float y, float a, float b);
	void ellipse(float x, float y, float a, float b, unsigned int segments);
	void ellipse(float x, float y, float a, float b, float degree);
	void ellipse(float x, float y, float a, float b, float degree, unsigned int segments);

	void arc(float x,float y,float radius,float begin,float degree);
	void arc(float x,float y,float radius,float begin,float degree,unsigned int segments);

	void polygon(float x, float y, float radius, const unsigned int edgeCount, float degree);
	void polygon(float x, float y, float radius, const unsigned int edgeCount, float degree, Color center, Color edge);

	void rectLine(float x1,float y1,float x2,float y2,float tickness);
	void line(float x1,float y1,float x2,float y2);
	void line(float* positions,const unsigned int size);
	
	bool isBegin();

	void setColor(Color color);
	void setColor(const float r, const float g, const float b, const float a);
};