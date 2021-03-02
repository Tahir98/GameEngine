#include "Cube.h"

Cube::Cube() :program("Shaders/cube.shader") {
	positions = new float[24]{
		0,0,0,
		0,1,0,
		1,1,0,
		1,0,0,
		1,0,1,
		1,1,1,
		0,1,1,
		0,0,1
	};

	for (unsigned int i = 0; i < 24; i++) {
		positions[i] -= 0.5f;
	}

	normals = new float[6*3]{
		0,0,1,
		1,0,0,
		0,0,-1,
		-1,0,0,
		0,1,0,
		0,-1,0
	};

	indices = new unsigned int[36]{
		0,1,2,	0,2,3,
		3,2,5,	3,5,4,
		4,5,6,	4,6,7,
		7,6,1,	7,1,0,
		1,6,5,	1,5,2,
		7,0,3,	7,3,4
	};
	
	for (unsigned int i = 0; i < 36; i++) {
		Vertex2 vertex;
		vertex.pos = { positions[indices[i] * 3],positions[indices[i] * 3 + 1] ,positions[indices[i] * 3 + 2] };
		vertex.color = vertex.pos - Vec3{ -0.5f,-0.5f,-0.5f };
		vertex.normal = { normals[(i / 6) * 3],normals[(i / 6) * 3 + 1] ,normals[(i / 6) * 3 + 6] };
		
		vertices.push_back(vertex);
	}

	va.bind();

	vb = new VertexBuffer((float*)vertices.data(), sizeof(Vertex2) * vertices.size(),GL_DYNAMIC_DRAW);

	vb->bind();

	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
}

Cube::Cube(Vec3 pos) : program("Shaders/cube.shader"),pos(pos){
	positions = new float[24]{
		0,0,0,
		0,1,0,
		1,1,0,
		1,0,0,
		1,0,1,
		1,1,1,
		0,1,1,
		0,0,1
	};

	for (unsigned int i = 0; i < 24; i++) {
		positions[i] -= 0.5f;
	}

	normals = new float[6 * 3]{
		0,0,1,
		1,0,0,
		0,0,-1,
		-1,0,0,
		0,1,0,
		0,-1,0
	};

	indices = new unsigned int[36]{
		0,1,2,	0,2,3,
		3,2,5,	3,5,4,
		4,5,6,	4,6,7,
		7,6,1,	7,1,0,
		1,6,5,	1,5,2,
		7,0,3,	7,3,4
	};

	for (unsigned int i = 0; i < 36; i++) {
		Vertex2 vertex;
		vertex.pos = { positions[indices[i] * 3],positions[indices[i] * 3 + 1] ,positions[indices[i] * 3 + 2] };
		vertex.color = vertex.pos - Vec3{ -0.5f,-0.5f,-0.5f };
		vertex.normal = { normals[(i / 6) * 3],normals[(i / 6) * 3 + 1] ,normals[(i / 6) * 3 + 6] };

		vertices.push_back(vertex);
	}

	va.bind();

	vb = new VertexBuffer((float*)vertices.data(), sizeof(Vertex2) * vertices.size(), GL_DYNAMIC_DRAW);

	vb->bind();

	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
}

Cube::Cube(Vec3 pos, Vec3 scale, Vec3 rotation) : program("Shaders/cube.shader"),pos(pos),scale(scale),rotation(rotation){
	positions = new float[24]{
		0,0,0,
		0,1,0,
		1,1,0,
		1,0,0,
		1,0,1,
		1,1,1,
		0,1,1,
		0,0,1
	};

	for (unsigned int i = 0; i < 24; i++) {
		positions[i] -= 0.5f;
	}

	normals = new float[6 * 3]{
		0,0,1,
		1,0,0,
		0,0,-1,
		-1,0,0,
		0,1,0,
		0,-1,0
	};

	indices = new unsigned int[36]{
		0,1,2,	0,2,3,
		3,2,5,	3,5,4,
		4,5,6,	4,6,7,
		7,6,1,	7,1,0,
		1,6,5,	1,5,2,
		7,0,3,	7,3,4
	};

	for (unsigned int i = 0; i < 36; i++) {
		Vertex2 vertex;
		vertex.pos = { positions[indices[i] * 3],positions[indices[i] * 3 + 1] ,positions[indices[i] * 3 + 2] };
		vertex.color = vertex.pos - Vec3{ -0.5f,-0.5f,-0.5f };
		vertex.normal = { normals[(i / 6) * 3],normals[(i / 6) * 3 + 1] ,normals[(i / 6) * 3 + 6] };

		vertices.push_back(vertex);
	}

	va.bind();

	vb = new VertexBuffer((float*)vertices.data(), sizeof(Vertex2) * vertices.size(), GL_DYNAMIC_DRAW);

	vb->bind();

	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
	va.addVertexAttribute(3, GL_FLOAT, false);
}

Cube::~Cube(){
	delete positions;
	delete normals;
	delete indices;

	delete vb;
}

void Cube::setPosition(Vec3 pos){
	this->pos = pos;
}

Vec3 Cube::getPosition(){
	return pos;
}

void Cube::setScale(Vec3 scale){
	this->scale = scale;
}

Vec3 Cube::getScale(){
	return scale;
}

void Cube::rotate(Vec3 rot){
	rotation = rotation + rot;
}

void Cube::setRotation(Vec3 rot) {
	this->rotation = rot;
}

void Cube::draw(Camera& camera){
	program.bind();
	va.bind();

	model = GLMath::translate(pos) * GLMath::rotate(rotation) * GLMath::scale(scale);

	program.setUniformMatrix4fv("model", 1, false, model.m[0]);
	program.setUniformMatrix4fv("view", 1, false, camera.getViewMatrix());
	program.setUniformMatrix4fv("projection", 1, false, camera.combine());

	glDrawArrays(GL_TRIANGLES,0,vertices.size());
}
