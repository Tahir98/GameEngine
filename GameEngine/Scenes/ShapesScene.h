#pragma once

#include "Scene.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "PerspectiveCamera.h"
#include "GLMath.h"
#include <vector>
#include "imgui.h"
#include <iostream>

#include "Camera/PerspectiveCamera.h"
#include "Core.h"
#include "InputHandler.h"
#include "Model/Cube.h"
#include "Model/Sphere.h"
#include "Model/Capsule.h"
#include "Model/Cylinder.h"
#include "Model/Cone.h"
#include "Model/Plane.h"
#include "Model/Circle3D.h"
#include "Skybox/SpherecalSkybox.h"
#include "Utility.h"

class ShapesScene : public Scene{
private:
	PerspectiveCamera camera;
	InputHandler handler;
	float camSpeed = 10;
	bool first = true;
	float prevX, prevY;
	float sensivity = 0.025f;

	float counter = 0;

	Matrix4x4 modelMatrix;
	Matrix4x4 modelMatrix2;

	Cube cube;

	Sphere sphere;
	Sphere sphere2;
	Capsule capsule;
	Cone cone;
	Cylinder cylinder;
	Plane plane;
	Circle3D circle;

	int segment = 3;
	int sPrev = 3;

	bool wf = false;
	bool tri = false;
	bool point = true;

	float radius, height;

	SpherecalSkybox skybox;

	Light light;
public:
	ShapesScene(GLFWwindow* window) :Scene(window),
		 camera(60, Application::aspectRatio, 0.1f, 1000), handler(window)
		, skybox("assets/rooitou_park.jpg"),plane("assets/icon.png")
	{
		modelMatrix = GLMath::scale(1,1,1);
		
		camera.setPosition(0,0,5);
		
		camera.rotY(00);

		sphere.setPosition({0,0,0});
		cone.setPosition({-6,0,0});
		capsule.setPosition({-3,0,0});
		cylinder.setPosition({ 3,0,0 });
		sphere2.setPosition({0,0,3});
		cube.setPosition({6,0,0});
		plane.setPosition({3,0,3});
		circle.setPosition({0,0,-3});

		sphere.debugMode = true;
		sphere2.debugMode = true;
		cone.debugMode = true;
		capsule.debugMode = true;
		cylinder.debugMode = true;
		cube.debugMode = true;
		plane.debugMode = true;
		circle.debugMode = true;

		sphere.drawTexture = false;

		light.pos = { 0,-5,0 };
		light.ambient = { 0.5f,0.5f ,0.5f };
		light.diffuse = { 0.8f,0.8f ,0.8f };
		light.specular = { 1.0f,1.0f ,1.0f };
	}


	void update(const float delta) {
		input(delta);
		imguiRender(delta);

		counter += delta;
		glDisable(GL_CULL_FACE);

		modelMatrix = GLMath::translate({ 0,0,-0 }) *  GLMath::scale(1, 1, 1) * GLMath::rotateZ(counter) * GLMath::rotateY(counter * 0.8f);

		camera.update(delta);
		
		sphere.rotate({ 0.00f,PI / 20.0f * delta * 0,0 });
		
		for (unsigned int i = 0; i < 1; i++) {
			capsule.draw(camera,light);
			cube.draw(camera,light);
			sphere.draw(camera,light);
			sphere2.draw(camera);
			cylinder.draw(camera,light);
			cone.draw(camera,light);
			plane.draw(camera,light);
			circle.draw(camera,light);

		}
		
		skybox.draw(camera);
	
		light.pos = sphere2.getPosition();

	}
	
	void input(const float delta) {
		if (handler.getMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT)) {
			double x, y;

			glfwGetCursorPos(window, &x, &y);
			y = Application::height - y;

			if (first) {
				prevX = (float)x;
				prevY = (float)y;
				first = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			float xDiff = (float)x - prevX;
			float yDiff = (float)y - prevY;

			prevX = (float)x;
			prevY = (float)y;

			camera.rotX(yDiff * sensivity);
			camera.rotY(xDiff * sensivity);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			first = true;
		}
		

		if (handler.getKeyState(GLFW_KEY_A)) {
			camera.translate2(-camSpeed * delta, 0, 0);
		}
		else if (handler.getKeyState(GLFW_KEY_D)) {
			camera.translate2(camSpeed * delta, 0, 0);
		}

		if (handler.getKeyState(GLFW_KEY_W)) {
			camera.translate2(0, 0, camSpeed * delta);
		}
		else if (handler.getKeyState(GLFW_KEY_S)) {
			camera.translate2(0, 0, -camSpeed * delta);
		}

		if (handler.getKeyState(GLFW_KEY_SPACE)) {
			camera.translate(0, camSpeed * delta, 0);
		}
		if (handler.getKeyState(GLFW_KEY_LEFT_SHIFT)) {
			camera.translate(0, -camSpeed * delta, 0);
		}
	}

	void imguiRender(float delta) {
		ImGui::Begin("Settings");

		ImGui::Text("FPS : %03d , %.3f ms", (int)(1.0f/delta),delta * 1000.0f);
		
		ImGui::End();
	}
};