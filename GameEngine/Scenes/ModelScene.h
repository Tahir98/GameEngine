#pragma once

#include "Scene.h"
#include "Model/Model.h"
#include "Camera/PerspectiveCamera.h"
#include "Skybox/SpherecalSkybox.h"
#include "Program.h"
#include "GLMath.h"
#include "imgui.h"
#include "Model/Sphere.h"
#include "InputHandler.h"
#include "Utility.h"

class ModelScene : public Scene {
private:
	PerspectiveCamera camera;
	SpherecalSkybox skybox;

	InputHandler handler;

	Program program;
	Model plane;

	Matrix4x4 model;

	Sphere sphere;
	float xPrev = 0, yPrev = 0;
	bool first = true;
	float sensivity = 0.025f;

	Light light;

public:

	ModelScene(GLFWwindow* window) :Scene(window),plane("assets/Su-25/Su-25.obj"), camera(70,Application::aspectRatio,0.1f,1000)
	,skybox("assets/market.jpg"),program("Shaders/modelvnt.shader"),handler(window){
		sphere.debugMode = true;

		light.pos = { 0,-5,0 };
		light.ambient = { 0.5f,0.5f ,0.5f };
		light.diffuse = { 0.8f,0.8f ,0.8f };
		light.specular = { 1.0f,1.0f ,1.0f };
	}


	void update(const float delta) override {
		input(delta);

		model = GLMath::translate({ 0, 0, -10 }) * GLMath::scale({ 0.5f,0.5f,0.5f });

		camera.update(delta);

		skybox.draw(camera);

		sphere.draw(camera);

		program.bind();
		program.setUniformMatrix4fv("model",1,false,model.m[0]);
		program.setUniformMatrix4fv("view", 1, false, camera.getViewMatrix());
		program.setUniformMatrix4fv("projection", 1, false, camera.combine());


		
		program.setUniform3f("viewerPos",camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		program.setUniform3f("light.position", light.pos.x, light.pos.y, light.pos.z);
		program.setUniform3f("light.ambient",light.ambient.x, light.ambient.y, light.ambient.z);
		program.setUniform3f("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
		program.setUniform3f("light.specular", light.specular.x, light.specular.y, light.specular.z);

		program.setUniform1f("light.constant",light.constant);
		program.setUniform1f("light.m_linear", light.linear);
		program.setUniform1f("light.quadrantic", light.quadrantic);
		
		light.pos = sphere.getPosition();

		plane.draw(program);

		imguiDraw(delta);

	}


	void input(const float delta) override {
		if (handler.getMouseButtonState(GLFW_MOUSE_BUTTON_2)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			if (first) {
				xPrev = x;
				yPrev = y;
				first = false;
			}
			else {
				float diffX = x - xPrev;
				float diffY = y - yPrev;

				xPrev = x;
				yPrev = y;

				camera.rotY(diffX * sensivity);
				camera.rotX(-diffY * sensivity);
			}
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			first = true;
		}

		if (handler.getKeyState(GLFW_KEY_W)) {
			camera.translate2(0,0, 3.0f * delta);
		}
		else if (handler.getKeyState(GLFW_KEY_S)) {
			camera.translate2(0, 0, -3.0f * delta);
		}

		if (handler.getKeyState(GLFW_KEY_A)) {
			camera.translate2(-3.0f * delta, 0, 0);
		}
		else if (handler.getKeyState(GLFW_KEY_D)) {
			camera.translate2(3.0f * delta, 0, 0);
		}

		if (handler.getKeyState(GLFW_KEY_SPACE)) {
			camera.translate(0, 3.0f * delta, 0);
		}
		else if (handler.getKeyState(GLFW_KEY_LEFT_SHIFT)) {
			camera.translate(0, -3.0f * delta, 0);
		}
	}

	void imguiDraw(float delta) {
		ImGui::Begin("Settings");


		ImGui::End();
	}
};