#pragma once
#include "Sphere.h"
#include "GL/glew.h"
#include <iostream>
#include "imgui.h"

Sphere::Sphere(){
	init();
}

Sphere::Sphere(const char* texturePath) : texturePath(texturePath){
	init();
}

Sphere::Sphere(const Vec3 pos ,const float radius, const char* texturePath) : texturePath(texturePath){
	this->pos = pos;

	if (radius > 0)
		this->radius = rTemp = radius;
	
	init();
}

Sphere::Sphere(const Vec3 pos,const Vec3 scale,const  Vec3 rotation,const float radius, const char* texturePath) 
	: texturePath(texturePath){
	this->pos = pos;
	this->scale = scale;
	this->rotation = rotation;

	if (radius > 0)
		this->radius = rTemp =radius;
	init();
}

Sphere::Sphere(const float radius, const unsigned int segment, const char* texturePath) : texturePath(texturePath) {
	if (radius > 0)
		this->radius = rTemp = radius;

	if (segment >= 2 && segment <= 50)
		this->segment = sTemp = segment;

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
	material.ambient = { 0.3f,0.3f,0.3f };
	material.diffuse = { 0.7f,0.7f,0.7f };
	material.specular = { 0.9f,0.9f,0.9f };
	material.shininess = 150;

	program = new Program("Shaders/sphereWF.shader");

	vb = new VertexBuffer(nullptr, sizeof(Vec3) * 15453, GL_STATIC_DRAW);
	ib = new IndexBuffer(nullptr, sizeof(unsigned int) * 29400, GL_STATIC_DRAW);
	ibl = new IndexBuffer(nullptr, sizeof(unsigned int) * 29400, GL_STATIC_DRAW);

	va.bind();
	vb->bind();

	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);

	if(texturePath)
		texture = new Texture(texturePath);

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

	if ((debugMode && triangle) || (!debugMode && mode == DrawMode::TRIANGLE)) {
		ib->bind();
		
		program->setUniform1f("chosen", 2);
		if (drawTexture && texturePath != nullptr) {
			texture->bind(0);
			program->setUniformi("drawTexture", 1);
		}
		else
			program->setUniformi("drawTexture", 0);

		program->setUniform3f("material.ambient",material.ambient.x, material.ambient.y, material.ambient.z);
		program->setUniform3f("material.diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
		program->setUniform3f("material.specular", material.specular.x, material.specular.y, material.specular.z);
		program->setUniform1f("material.shininess", material.shininess);

		program->setUniform3f("light.specular", 0, 0, 0);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	
	if ((debugMode && point) || (!debugMode && mode == DrawMode::POINT)) {
		program->setUniform1f("chosen", 0);
		glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
	}
	
	if ((debugMode && line) || (!debugMode && mode == DrawMode::LINE)) {
		ibl->bind();
		program->setUniform1f("chosen", 1);
		glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, 0);
	}

	if (debugMode) {
		imGuiDraw();
		if (sTemp != segment)
			setSegment(sTemp);
		if (rTemp != radius)
			setRadius(rTemp);
	}
}

void Sphere::draw(Camera& camera, Light light){
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	model = GLMath::translate(pos) * GLMath::scale(scale) * GLMath::rotate(rotation);

	program->bind();
	program->setUniformi("tex", 0);

	program->setUniformMatrix4fv("model", 1, false, model.m[0]);
	program->setUniformMatrix4fv("view", 1, false, camera.getViewMatrix());
	program->setUniformMatrix4fv("projection", 1, false, camera.combine());

	va.bind();

	if ((debugMode && triangle) || (!debugMode && mode == DrawMode::TRIANGLE)) {
		ib->bind();

		program->setUniform1f("chosen", 2);
		if (drawTexture && texturePath != nullptr) {
			texture->bind(0);
			program->setUniformi("drawTexture", 1);
		}
		else
			program->setUniformi("drawTexture", 0);

		program->setUniform3f("material.ambient", material.ambient.x, material.ambient.y, material.ambient.z);
		program->setUniform3f("material.diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
		program->setUniform3f("material.specular", material.specular.x, material.specular.y, material.specular.z);
		program->setUniform1f("material.shininess", material.shininess);

		program->setUniform3f("light.pos",light.pos.x, light.pos.y, light.pos.z);
		program->setUniform3f("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
		program->setUniform3f("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
		program->setUniform3f("light.specular", light.specular.x, light.specular.y, light.specular.z);

		Vec3 viewPos = camera.getPosition();
		program->setUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	if ((debugMode && point) || (!debugMode && mode == DrawMode::POINT)) {
		program->setUniform1f("chosen", 0);
		glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
	}

	if ((debugMode && line) || (!debugMode && mode == DrawMode::LINE)) {
		ibl->bind();
		program->setUniform1f("chosen", 1);
		glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, 0);
	}

	if (debugMode) {
		imGuiDraw();
		if (sTemp != segment)
			setSegment(sTemp);
		if (rTemp != radius)
			setRadius(rTemp);
	}
}

void Sphere::imGuiDraw(){
	ImGui::Begin("Sphere");
	ImGui::SliderFloat3("Position", (float*)&pos, -20, 20);
	ImGui::SliderFloat3("Scale", (float*)&scale, 0, 10);
	ImGui::SliderFloat3("Rotation", (float*)&rotation, -PI, PI);
	ImGui::SliderFloat3("amibent",(float*)&material.ambient,0,1);
	ImGui::SliderFloat3("diffuse", (float*)&material.diffuse, 0, 1);
	ImGui::SliderFloat3("specular", (float*)&material.specular, 0, 1);
	ImGui::SliderFloat("shininess", &material.shininess, 1, 200);

	ImGui::SliderInt("Segment", (int*)&sTemp, 2, 50);
	ImGui::SliderFloat("Radius", &rTemp, 0, 10);
	ImGui::Checkbox("Point", &point);
	ImGui::Checkbox("Line", &line);
	ImGui::Checkbox("Triangle", &triangle);
	ImGui::Checkbox("Draw Texture",&drawTexture);
	ImGui::End();
}


void Sphere::setSegment(const unsigned  int segment){
	if (segment >= 2 && segment <= 50) {
		this->segment = segment;
		if (sTemp != segment)
			sTemp = segment;
	}
	else {
		return;
	}

	vertices.clear();
	vertices.resize(0);
	vertices.shrink_to_fit();

	indices.clear();
	indices.resize(0);
	indices.shrink_to_fit();

	lineIndices.clear();
	lineIndices.resize(0);
	lineIndices.shrink_to_fit();

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
			
			//std::cout << "x:" << x << " y:" << y << " z:" << z << std::endl;

			vertices.push_back({ x * radius, y * radius,z * radius });
			vertices.push_back({ x, y, z });
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
}

unsigned int Sphere::getSegment(){
	return segment;
}

void Sphere::setRadius(const float radius){
	if (radius == this->radius)
		return;
	if (rTemp != radius)
		rTemp = radius;

	if (radius > 0) {
		for (unsigned int i = 0; i < vertices.size(); i += 3) {
			vertices[i].x = vertices[i].x / this->radius * radius;
			vertices[i].y = vertices[i].y / this->radius * radius;
			vertices[i].z = vertices[i].z / this->radius * radius;
		}

		this->radius = radius;
		vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
	}
}

float Sphere::getRadius(){
	return radius;
}

void Sphere::setMaterial(Material material){
	this->material = material;
}




