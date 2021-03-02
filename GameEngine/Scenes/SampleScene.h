#pragma once
#include "Scene.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Program.h"
#include "GLMath.h"
#include "Core.h"
#include <vector>
#include <stdlib.h>
#include "Texture.h"


bool isW = false, isS = false, isA = false, isD = false, isSpace = false, isShift = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action,int mode){
	if (key == GLFW_KEY_W && action == GLFW_PRESS) isW = true;
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE) isW = false;

	if (key == GLFW_KEY_S && action == GLFW_PRESS) isS = true;
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE) isS = false;

	if (key == GLFW_KEY_A && action == GLFW_PRESS) isA = true;
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE) isA = false;

	if (key == GLFW_KEY_D && action == GLFW_PRESS) isD = true;
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE) isD = false;

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) isSpace = true;
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) isSpace = false;

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) isShift = true;
	else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) isShift = false;
	
}

Vec2 mousePrevPos = { Application::width/2.0f,Application::height/2.0f};
Vec2 mousePos = {0,0};
Vec2 offset;
float sensivity = 0.02f;
float yaw = -180, pitch = 0;
bool isFirst = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	mousePrevPos = mousePos;
	if (isFirst) {
		mousePrevPos.x = (float)xpos;
		mousePrevPos.y = Application::height - (float)ypos;
		isFirst = false;
	}

	mousePos.x = (float)xpos;
	mousePos.y = Application::height - (float)ypos;

	offset = mousePos - mousePrevPos;
	offset = offset * sensivity;

	yaw += offset.x;
	pitch += offset.y;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}


class SampleScene : public Scene {

private:
	VertexArray va;
	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;
	Program program;

	Vec3 camPos;
	Vec3 camDirection;
	Vec3 camUp;

	float counter = 0.0f;
	
	Matrix4x4 rot;
	Matrix4x4 model;
	Matrix4x4 view;
	Matrix4x4 projection;

	std::vector<float> buffer;

	Texture* texture;
public: 


	SampleScene(GLFWwindow* window) :Scene(window) ,program("Shaders/model.shader") {
		/*for (unsigned int i = 0; i < sizeof(indices)/sizeof(unsigned int);) {
			buffer.push_back((float)vertices[indices[i] * 3]);
			buffer.push_back((float)vertices[indices[i] * 3 + 1]);
			buffer.push_back((float)vertices[indices[i] * 3 + 2]);

			buffer.push_back((float)vertexNormals[indices[i + 2] * 3]);
			buffer.push_back((float)vertexNormals[indices[i + 2] * 3 + 1]);
			buffer.push_back((float)vertexNormals[indices[i + 2] * 3 + 2]);

			buffer.push_back((float)texCoords[indices[i + 1] * 3]);
			buffer.push_back((float)texCoords[indices[i + 1] * 3 + 1]);
			buffer.push_back((float)texCoords[indices[i + 1] * 3 + 2]);

			i += 3;
		}*/
		
		va.bind();
		vb = new VertexBuffer(buffer.data(),buffer.size() * sizeof(float),GL_STATIC_DRAW);
		vb->bind();

		va.addVertexAttribute(3, GL_FLOAT, false);
		va.addVertexAttribute(3, GL_FLOAT, false);
		va.addVertexAttribute(3, GL_FLOAT, false);


		camPos = {0,0,5};
		camDirection = {0,0,-1};
		camUp = { 0,1,0 };

		view = GLMath::lookat(camPos,camPos + camDirection,camUp);
		//projection = GLMath::ortho(16.0f/9.0f * -10.0f, 16.0f / 9.0f * 10.0f,-10,10,0.1f,100);
		projection = GLMath::perspective(9.0f/ 16.0f,70,1.5f,1000.0f);

		texture = new Texture("assets/diffuse.png");

		glfwSetKeyCallback(window,key_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);

	}
	
	~SampleScene() {
		if (vb != nullptr)
			delete vb;
		if (ib != nullptr)
			delete ib;
	}

	void update(float delta) override{
		input(delta);

		counter += delta;
		
		Matrix4x4 rot = GLMath::rotateX(counter * 0.0f) * GLMath::rotateY(counter * 0.0f);;
		model = GLMath::translate({ 0,0,-10 }) * rot;
		view = GLMath::lookat(camPos, camPos + camDirection, camUp);
		
		
		texture->bind();
		va.bind();
		program.bind();
	
		program.setUniformMatrix4fv("model", 1, false, model.m[0]);
		program.setUniformMatrix4fv("view", 1, false, view.m[0]);
		program.setUniformMatrix4fv("projection", 1, false, projection.m[0]);

		program.setUniform3f("viewPos",camPos.x,camPos.y,camPos.z);
		program.setUniform3f("lightPos",50,50,50);

		glDrawArrays(GL_TRIANGLES,0,buffer.size()/9);

	}

	void input(float delta) override {
		Vec3 direction;
		direction.x = cosf(pitch * PI / 180.0f) * cosf(yaw * PI / 180.0f);
		direction.y = sinf(pitch * PI / 180.0f);
		direction.z = cosf(pitch * PI / 180.0f) * sinf(yaw * PI / 180.0f);
		camDirection = GLMath::normalize(direction);
		

		float camSpeed = 0.05f;
		
		if (isW) 
			camPos = camPos + camDirection * camSpeed;
		if (isS) 
			camPos = camPos - camDirection * camSpeed;
		if (isA) 
			camPos =camPos - GLMath::normalize(GLMath::crossProduct(camDirection, camUp)) * camSpeed;
		if (isD)
			camPos = camPos + GLMath::normalize(GLMath::crossProduct(camDirection, camUp)) * camSpeed;
		
		
		if (isSpace) {
			Vec3 right = GLMath::normalize(GLMath::crossProduct(camDirection, camUp));
			Vec3 up = GLMath::normalize(GLMath::crossProduct(right,camDirection));
			camPos = camPos + up * camSpeed;
		}

		if (isShift) {
			Vec3 right = GLMath::normalize(GLMath::crossProduct(camDirection, camUp));
			Vec3 up = GLMath::normalize(GLMath::crossProduct(right, camDirection));
			camPos = camPos - up * camSpeed;
		}
	
	}
};