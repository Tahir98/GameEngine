#pragma once
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, 
	std::vector<MeshTexture>* textures): vertices(vertices),indices(indices),textures(textures){
	va = new VertexArray();
	vb = new VertexBuffer((float*)vertices->data(),sizeof(Vertex) * vertices->size(),GL_STATIC_DRAW);

	va->bind();
	vb->bind();

	va->addVertexAttribute(3, GL_FLOAT, false);
	va->addVertexAttribute(3, GL_FLOAT, false);
	va->addVertexAttribute(2, GL_FLOAT, false);

	ib = new IndexBuffer(indices->data(),sizeof(unsigned int) * indices->size(),GL_STATIC_DRAW);
}

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<MeshTexture>* textures, Material material) : vertices(vertices), indices(indices), textures(textures),material(material) {
	va = new VertexArray();
	vb = new VertexBuffer((float*)vertices->data(), sizeof(Vertex) * vertices->size(), GL_STATIC_DRAW);

	va->bind();
	vb->bind();

	va->addVertexAttribute(3, GL_FLOAT, false);
	va->addVertexAttribute(3, GL_FLOAT, false);
	va->addVertexAttribute(2, GL_FLOAT, false);

	ib = new IndexBuffer(indices->data(), sizeof(unsigned int) * indices->size(), GL_STATIC_DRAW);
}

Mesh::~Mesh(){
	delete va;
	delete vb;
	delete ib;

	delete vertices;
	delete indices;
	delete textures;
}

void Mesh::draw(Program& program){
	int s = 0, d = 0;
	program.bind();
	for (unsigned int i = 0; i < textures->size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, (*textures)[i].id);

		std::string name = (*textures)[i].type;

		if ((*textures)[i].type == "texture_diffuse")
			program.setUniformi((name + std::to_string(++d)).c_str(),i);
		else if ((*textures)[i].type == "texture_specular")
			program.setUniformi((name + std::to_string(++s)).c_str(), i);
	}

	if (material.shininess != 0.0f) {
		program.setUniform3f("material.ambient",material.ambient.x, material.ambient.y, material.ambient.z);
		program.setUniform3f("material.diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
		program.setUniform3f("material.specular", material.specular.x, material.specular.y, material.specular.z);
		program.setUniform1f("material.shininess",material.shininess);
	}

	va->bind();
	ib->bind();

	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
}


