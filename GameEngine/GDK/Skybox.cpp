#include "Skybox.h"
#include "GL/glew.h"
#include <stdlib.h>

float proj[4][4];

Skybox::Skybox(){

	textures = (Texture*)malloc(sizeof(Texture) * 6);

	textures[0] =  Texture("assets/skybox/miramar_ft.jpg");
	textures[1] =  Texture("assets/skybox/miramar_rt.jpg");
	textures[2] =  Texture("assets/skybox/miramar_bk.jpg");
	textures[3] =  Texture("assets/skybox/miramar_lf.jpg");
	textures[4] =  Texture("assets/skybox/miramar_up.jpg");
	textures[5] =  Texture("assets/skybox/miramar_dn.jpg");

	vertices = {
		-1,-1,-1,	0,1,
		-1,1,-1,	0,0,
		1,1,-1,		1,0,
		-1,-1,-1,	0,1,
		1,1,-1,		1,0,
		1,-1,-1,	1,1,

		1,-1,1,		0,1,
		1,1,1,		0,0,
		1,1,-1,		1,0,
		1,-1,1,		0,1,
		1,1,-1,		1,0,
		1,-1,-1,	1,1,

		-1,-1,1,	0,1,
		-1,1,1,		0,0,
		1,1,1,		1,0,
		-1,-1,1,	0,1,
		1,1,1,		1,0,
		1,-1,1,		1,1,

		-1,-1,-1,	0,1,
		-1,1,-1,	0,0,
		-1,1,1,		1,0,
		-1,-1,-1,	0,1,
		-1,1,1,		1,0,
		-1,-1,1,	1,1,

		-1,1,-1,	0,1,
		-1,1,1,		0,0,
		1,1,1,		0,1,
		-1,1,-1,	0,1,
		1,1,1,		0,1,
		1,1 -1,		1,1,

		-1,-1,-1,	0,1,
		-1,-1,1,	0,0,
		1,-1,1,		1,0,
		-1,-1,-1,	0,1,
		1,-1,1,		1,0,
		1,-1,-1,	1,1,
	};

	for(unsigned int i=0;i<vertices.size();i+=5){
		vertices[i] *= 300.0f;
		vertices[i + 1] *= 300.0f;
		vertices[i + 2] *= 300.0f;
	}


	float fRad = 1.0f / tanf(90.0 / 2.0f * 3.14159f / 180.0f);
	float far = 1000.0f, near = 0.1f;

	proj[0][0] = fRad * 9.0f / 16.0f;
	proj[1][1] = fRad;
	proj[2][2] = 2.0f / (far - near);
	proj[3][2] = (far + near) / (far - near);
	proj[2][3] = 1.0f;

	vb = new VertexBuffer(vertices.data(),sizeof(float) * vertices.size(), GL_STATIC_DRAW);
	va.bind();
	vb->bind();
	va.addVertexAttribute( 3, GL_FLOAT, false);
	va.addVertexAttribute( 2, GL_FLOAT, false);


	shader = new Program("Shaders/skybox.shader");
}

Skybox::~Skybox()
{
	delete vb;
	delete shader;

	free(textures);
}

void Skybox::draw(){
	va.bind();
	vb->bind();
	shader->bind();
	shader->setUniformMatrix4fv("projection",1,false,proj[0]);

	for (unsigned int i = 0; i < 6; i++) {
		textures[i].bind(0);
		glDrawArrays(GL_TRIANGLES,i*6,6);
	}
}
