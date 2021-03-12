#pragma once
#include "GLMath.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"
#include "Camera.h"
#include <vector>
#include <iostream>
#include "stb_image.h"
#include "GL/glew.h"

class SpherecalSkybox {
private:
	VertexArray va;
	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;
	Program* program = nullptr;

	unsigned int size = 0;
	unsigned int textureID = 0;

public:
	SpherecalSkybox(const char* texturePath){
		std::vector<Vec3> vertices;
		std::vector<unsigned int> indices;

		float degY = 0;
		float degX = 0;

		float x, y, z;
		int segment = 40;
		float radius = 10;

		for (unsigned int i = 0; i < 2 * segment + 1; i++) {
			degY = (180.0f / (float)segment) * i;

			for (unsigned int j = 0; j < segment + 1; j++) {
				degX = (180.0f / (float)segment) * j;

				y = cosf(GLMath::MtoRadian(degX));
				x = sinf(GLMath::MtoRadian(degY)) * sinf(GLMath::MtoRadian(degX));
				z = cosf(GLMath::MtoRadian(degY)) * sinf(GLMath::MtoRadian(degX));

				vertices.push_back({ x * radius, y * radius,z * radius });
				vertices.push_back({1.0f - ((float)i) / (2.0f * (float)segment),((float)segment - (float)j) / (float)segment,0 });
			}
		}

		for (unsigned int i = 0; i < 2 * segment; i++) {
			for (unsigned int j = 0; j < segment; j++) {
				if (i < 2 * segment) {
					if (j == 0) {
						indices.push_back(i * (segment + 1) + j);
						indices.push_back((i + 1) * (segment + 1) + j + 1);
						indices.push_back(i * (segment + 1) + j + 1);
					}
					else if (j == segment - 1) {
						indices.push_back(i * (segment + 1) + j);
						indices.push_back((i + 1) * (segment + 1) + j);
						indices.push_back(i * (segment + 1) + j + 1);
					}
					else {
						indices.push_back(i * (segment + 1) + j);
						indices.push_back((i + 1) * (segment + 1) + j + 1);
						indices.push_back(i * (segment + 1) + j + 1);

						indices.push_back(i * (segment + 1) + j);
						indices.push_back((i + 1) * (segment + 1) + j);
						indices.push_back((i + 1) * (segment + 1) + j + 1);
					}
				}
			}
		}

		vb = new VertexBuffer((float*)vertices.data(),sizeof(Vec3) * vertices.size(),GL_STATIC_DRAW);
		ib = new IndexBuffer(indices.data(), sizeof(unsigned int) * indices.size(), GL_STATIC_DRAW);

		size = indices.size();

		va.bind();
		vb->bind();

		va.addVertexAttribute(3, GL_FLOAT, false);
		va.addVertexAttribute(3, GL_FLOAT, false);

		program = new Program("Shaders/skybox.shader");

		int width, height, bpp;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* image = stbi_load(texturePath, &width, &height, &bpp, 4);

		if (image == nullptr) {
			std::cout << "Image not found" << std::endl;
		}
		else {
			glGenTextures(1, &textureID);

			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			std::cout << texturePath << " width :" << width << "  height: " << height << " bpp: " << bpp << std::endl;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

			stbi_image_free(image);
		}
	}

	~SpherecalSkybox() {
		delete vb;
		delete ib;
		delete program;

		glDeleteTextures(1,&textureID);
	}


	void draw(Camera& camera) {
		glEnable(GL_DEPTH_TEST);

		program->bind();
		program->setUniformi("tex", 0);

		Matrix4x4 view;
		float* value = camera.getViewMatrix();
		for (unsigned int i = 0; i < 16; i++) {
			view.m[i / 4][i % 4] = value[i];
			if (i < 12 && i % 4 == 3)
				view.m[i / 4][i % 4] = 0;
		}

		program->setUniformMatrix4fv("view", 1, false, view.m[0]);
		program->setUniformMatrix4fv("projection", 1, false, camera.combine());

		va.bind();
		ib->bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,textureID);

		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);		
	}
};

