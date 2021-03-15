#include "Plane.h"
#include "imgui.h"
#include <iostream>
#include <string>

static int objectCount = 0;

Plane::Plane(){
	init();
}

Plane::Plane(const char* texturePath) : texturePath(texturePath){
	init();
}

Plane::Plane(const Vec3 pos, const Vec3 size, const char* texturePath) : texturePath(texturePath){
	this->size = size;
	this->pos = pos;
	init();
}

Plane::Plane(const Vec3 pos, const Vec3 scale, const Vec3 rotation, const char* texturePath) : texturePath(texturePath){
	this->pos = pos;
	this->scale = scale;
	this->rotation = rotation;

	init();
}

Plane::Plane(const Vec3 size, const char* texturePath) : texturePath(texturePath){
	this->size = size;

	init();
}

Plane::~Plane(){
	delete vb;
	delete ib;
	delete ibl;
	delete program;

	delete texture;

	objectCount--;
}

void Plane::init(){
	m_objectCount = objectCount;

	material.ambient = { 0.3f,0.3f,0.3f };
	material.diffuse = { 0.7f,0.7f,0.7f };
	material.specular = { 0.9f,0.9f,0.9f };
	material.shininess = 150;

	program = new Program("Shaders/sphereWF.shader");

	vb = new VertexBuffer(nullptr, sizeof(Vec3) * 72, GL_STATIC_DRAW);
	ib = new IndexBuffer(nullptr, sizeof(unsigned int) * 36, GL_STATIC_DRAW);
	ibl = new IndexBuffer(nullptr, sizeof(unsigned int) * 40, GL_STATIC_DRAW);

	va.bind();
	vb->bind();

	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);

	if (texturePath)
		texture = new Texture(texturePath);

	setSize(size);
}

void Plane::draw(Camera& camera){
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
		if (sTemp.x != size.x || sTemp.z != size.z)
			setSize(sTemp);
	}
}

void Plane::draw(Camera& camera, Light light){
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

		program->setUniform3f("light.pos", light.pos.x, light.pos.y, light.pos.z);
		program->setUniform3f("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
		program->setUniform3f("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
		program->setUniform3f("light.specular", light.specular.x, light.specular.y, light.specular.z);
		program->setUniform1f("light.constant", light.constant);
		program->setUniform1f("light.linear", light.linear);
		program->setUniform1f("light.quadrantic", light.quadrantic);

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
		if (sTemp.x != size.x || sTemp.z != size.z)
			setSize(sTemp);
	}
}

void Plane::imGuiDraw(){
	ImGui::Begin(("Plane" + std::to_string(m_objectCount)).c_str());
	ImGui::SliderFloat3("Position", (float*)&pos, -20, 20);
	ImGui::SliderFloat3("Scale", (float*)&scale, 0, 10);
	ImGui::SliderFloat3("Rotation", (float*)&rotation, -PI, PI);
	ImGui::SliderFloat3("amibent", (float*)&material.ambient, 0, 1);
	ImGui::SliderFloat3("diffuse", (float*)&material.diffuse, 0, 1);
	ImGui::SliderFloat3("specular", (float*)&material.specular, 0, 1);
	ImGui::SliderFloat("shininess", &material.shininess, 1, 200);

	ImGui::SliderFloat3("Size", (float*)&sTemp, 0, 10);
	ImGui::Checkbox("Point", &point);
	ImGui::Checkbox("Line", &line);
	ImGui::Checkbox("Triangle", &triangle);
	ImGui::Checkbox("Draw Texture", &drawTexture);
	ImGui::End();
}

void Plane::setSize(const Vec3 size){
	if (sTemp.x != size.x || sTemp.z != size.z)
		sTemp = size;

	this->size = size;

	vertices.clear();
	vertices.resize(0);
	vertices.shrink_to_fit();

	indices.clear();
	indices.resize(0);
	indices.shrink_to_fit();

	lineIndices.clear();
	lineIndices.resize(0);
	lineIndices.shrink_to_fit();

	vertices = {
		{-0.5f,0, 0.5f} , {0,1,0} , {0,0,0}, // 0
		{-0.5f,0,-0.5f} , {0,1,0} , {0,1,0}, // 1
		{ 0.5f,0,-0.5f} , {0,1,0} , {1,1,0}, // 2
		{ 0.5f,0, 0.5f} , {0,1,0} , {1,0,0}, // 3
	};
	
	indices = {
		0,2,1,	0,3,2
	};

	lineIndices = {
		0,1,  1,2,  2,3,  3,0,  0,2
	};

	for (int i = 0; i < vertices.size(); i += 3) {
		vertices[i].x *= size.x;
		vertices[i].y *= size.y;
		vertices[i].z *= size.z;
	}

	//std::cout << "Vertices size: " << vertices.size() << std::endl;
	//std::cout << "Indices size: " << indices.size() << std::endl;
	//std::cout << "LineIndices size: " << lineIndices.size() << std::endl;

	vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
	ib->subData(0, indices.size() * sizeof(unsigned int), indices.data());
	ibl->subData(0, lineIndices.size() * sizeof(unsigned int), lineIndices.data());
}

Vec3 Plane::getSize(){
	return size;
}

void Plane::setMaterial(Material material){
	this->material = material;
}
