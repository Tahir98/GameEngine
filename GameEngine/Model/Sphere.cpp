#include "Sphere.h"
#include "GL/glew.h"
#include <iostream>

Sphere::Sphere(){
	init();
}

Sphere::Sphere(const Vec3 pos ,const float radius) : pos(pos){
	if (radius > 0)
		this->radius = radius;
	
	init();
}

Sphere::Sphere(const Vec3 pos,const Vec3 scale,const  Vec3 rotation,const float radius) : pos(pos),scale(scale),rotation(rotation){
	if (radius > 0)
		this->radius = radius;
	init();
}

Sphere::Sphere(const float radius, const unsigned int segment) {
	if (radius > 0)
		this->radius = radius;

	if (segment >= 2 && segment <= 50)
		this->segment = segment;

	init();
}

Sphere::~Sphere(){
	delete vb;
	delete ib;
	delete ibl;
	delete program;

	delete texture;
}

void Sphere::init(){
	program = new Program("Shaders/sphereWF.shader");

	vb = new VertexBuffer(nullptr, sizeof(Vec3) * 15453, GL_STATIC_DRAW);
	ib = new IndexBuffer(nullptr, sizeof(unsigned int) * 29400, GL_STATIC_DRAW);
	ibl = new IndexBuffer(nullptr, sizeof(unsigned int) * 29400, GL_STATIC_DRAW);

	va.bind();
	vb->bind();

	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);

	texture = new Texture("assets/map.png");

	setSegment(segment);
}

void Sphere::draw(Camera& camera){
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	model = GLMath::translate(pos) * GLMath::scale(scale) * GLMath::rotate(rotation);

	program->bind();
	program->setUniformi("tex", 0);

	program->setUniformMatrix4fv("model", 1, false, model.m[0]);
	program->setUniformMatrix4fv("view", 1, false, camera.getViewMatrix());
	program->setUniformMatrix4fv("projection", 1, false, camera.combine());

	va.bind();
	
	if (mode == DrawMode::POINT) {
		program->setUniform1f("chosen", 0);
		glDrawArrays(GL_POINTS, 0, vSize);
	}
	else if (mode == DrawMode::LINE) {
		ibl->bind();
		//glDisable(GL_DEPTH_TEST);
		program->setUniform1f("chosen", 1);

		glLineWidth(2);
		glDrawElements(GL_LINES, ilSize, GL_UNSIGNED_INT, 0);
	}
	else if (mode == DrawMode::TRIANGLE) {
		ib->bind();
		texture->bind(0);
		program->setUniform1f("chosen", 2);
		glDrawElements(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0);
	}

}

void Sphere::setDrawMode(DrawMode mode){
	this->mode = mode;
}

void Sphere::setSegment(const unsigned  int segment){
	if (segment >= 2 && segment <= 50) {
		this->segment = segment;
	}
	else {
		return;
	}

	std::vector<Vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;
	
	float degY = 0;
	float degX = 0;

	float x, y, z;

	for (unsigned int i = 0; i < 2 * segment + 1; i++) {
		degY = (180.0f / (float)segment) * i;

		for (unsigned int j = 0; j < segment + 1; j++) {
			degX = (180.0f / (float)segment) * j;

			y = cosf(GLMath::MtoRadian(degX));
			x = sinf(GLMath::MtoRadian(degY)) * sinf(GLMath::MtoRadian(degX));
			z = cosf(GLMath::MtoRadian(degY)) * sinf(GLMath::MtoRadian(degX));
			
			vertices.push_back({ x * radius, y * radius,z * radius });
			vertices.push_back({ color });
			vertices.push_back({ ((float)i) / (2.0f * (float)segment),((float)segment - (float)j) / (float)segment,0 });
		}
	}

	for (unsigned int i = 0; i < 2 * segment; i++) {
		for (unsigned int j = 0; j < segment; j++) {
			if (i < 2 * segment) {
				if (j == 0) {
					indices.push_back(i * (segment + 1) + j);
					indices.push_back(i * (segment + 1) + j + 1);
					indices.push_back((i + 1) * (segment + 1) + j + 1);
				}
				else if (j == segment - 1) {
					indices.push_back(i * (segment + 1) + j);
					indices.push_back(i * (segment + 1) + j + 1);
					indices.push_back((i + 1) * (segment + 1) + j);
				}
				else {
					indices.push_back(i * (segment + 1) + j);
					indices.push_back(i * (segment + 1) + j + 1);
					indices.push_back((i + 1) * (segment + 1) + j + 1);

					indices.push_back(i * (segment + 1) + j);
					indices.push_back((i + 1) * (segment + 1) + j + 1);
					indices.push_back((i + 1) * (segment + 1) + j);
				}
			}
		}
	}


	for (unsigned int i = 0; i < 2 * segment; i++) {
		for (unsigned int j = 0; j < segment; j++) {
			if (i < 2 * segment) {
				if (j == 0) {
					lineIndices.push_back(i * (segment + 1) + j);
					lineIndices.push_back(i * (segment + 1) + j + 1);
					
					lineIndices.push_back(i * (segment + 1) + j + 1);
					lineIndices.push_back((i + 1) * (segment + 1) + j + 1);
				}
				else if (j == segment - 1) {
					lineIndices.push_back(i * (segment + 1) + j);
					lineIndices.push_back(i * (segment + 1) + j + 1);
				}
				else {
					lineIndices.push_back(i * (segment + 1) + j);
					lineIndices.push_back(i * (segment + 1) + j + 1);
					lineIndices.push_back(i * (segment + 1) + j + 1);
					lineIndices.push_back((i + 1) * (segment + 1) + j + 1);
					lineIndices.push_back((i + 1) * (segment + 1) + j + 1);
					lineIndices.push_back(i * (segment + 1) + j);
				}
			}
		}
	}

	//std::cout << "Vertices size: " << vertices.size() << std::endl;
	//std::cout << "Indices size: " << indices.size() << std::endl;
	//std::cout << "LineIndices size: " << lineIndices.size() << std::endl;

	vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
	ib->subData(0, indices.size() * sizeof(unsigned int), indices.data());
	ibl->subData(0, lineIndices.size() * sizeof(unsigned int), lineIndices.data());

	iSize = indices.size();
	vSize = vertices.size() / 3;
	ilSize = lineIndices.size();
}

unsigned int Sphere::getSegment(){
	return segment;
}

void Sphere::setPosition(const Vec3 pos){
	this->pos = pos;
}

Vec3 Sphere::getPosition(){
	return pos;
}

void Sphere::setScale(const Vec3 scale){
	this->scale = scale;
}

Vec3 Sphere::getScale(){
	return scale;
}

void Sphere::rotate(Vec3 rot){
	rotation = rotation + rot;
}

void Sphere::setRotation(Vec3 rotation){
	this->rotation = rotation;
}

Vec3 Sphere::getRotation(){
	return rotation;
}

void Sphere::setColor(Vec3 color){
	this->color = color;
}

Vec3 Sphere::getColor(){
	return color;
}
