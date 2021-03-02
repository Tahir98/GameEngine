#include "Scene.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "PerspectiveCamera.h"
#include "GLMath.h"
#include <vector>
#include "imgui.h"
#include "Model/Model.h"
#include <iostream>

#include "Camera/PerspectiveCamera.h"
#include "Core.h"
#include "InputHandler.h"
#include "Model/Cube.h"
#include "Model/Sphere.h"
#include "Model/Capsule.h"
#include "Model/Cylinder.h"
#include "Model/Cone.h"

class ModelScene : public Scene{
private:
	Program program;
	Model model;
	Model model2;

	PerspectiveCamera camera;
	InputHandler handler;
	float camSpeed = 10;
	bool first = true;
	float prevX, prevY;
	float sensivity = 0.025f;

	Matrix4x4 modelMatrix;
	Matrix4x4 modelMatrix2;

	Cube cube;
	Vec3 cubePos = {0};
	Vec3 cubeScale = { 1,1,1 };
	float rotate = 0.001f;
	float counter = 0;

	Sphere sphere;
	Capsule capsule;
	Cone cone;
	Cylinder cylinder;

	int segment = 3;
	int sPrev = 3;

	bool wf = false;
	bool tri = false;
	bool point = true;

	float radius, height;
public:
	ModelScene(GLFWwindow* window) :Scene(window),program("Shaders/modelvnt.shader") ,model("assets/Su-25/Su-25.obj"),sphere(2,50),
		model2("assets/plane/airplane.obj"),camera(60, Application::aspectRatio, 0.1f, 1000), handler(window), cube(Vec3{ 5,0,0 })
	{
		modelMatrix = GLMath::scale(1,1,1);
		modelMatrix2 = GLMath::translate({ 0,0,10 }) * GLMath::scale(0.06f, 0.06f, 0.06f);
		camera.setPosition(0,0,-5);
		
		camera.rotY(90);

		cube.setScale({ 0.1f,0.1f,0.1f });


		sphere.setPosition({ 0,10,0 });
		sphere.setSegment(50);
		cone.setSize(5,10);
		cone.setSegment(100);
		cone.setPosition({ -30, 0, 0 });
		capsule.setSegment(100);
		capsule.setPosition({ 20,0,20 });

		cylinder.setPosition({0,0,-20});
		cylinder.setSegment(100);

		segment = capsule.getSegment();
		sPrev = capsule.getSegment();

		radius = capsule.getRadius();
		height = capsule.getHeight();
	}


	void update(const float delta) {
		input(delta);
		imguiRender(delta);

		//counter += delta;
		glDisable(GL_CULL_FACE);


		modelMatrix = GLMath::scale(1, 1, 1) * GLMath::rotateZ(counter) * GLMath::rotateY(counter * 0.8f);

		camera.update(delta);
		cube.setPosition(cubePos);
		cube.setScale(cubeScale);
		cube.rotate({ 0, rotate, 0 });

		program.bind();
		program.setUniformMatrix4fv("model",1,false,modelMatrix.m[0]);
		program.setUniformMatrix4fv("view",1,false,camera.getViewMatrix());
		program.setUniformMatrix4fv("projection",1,false,camera.combine());

		Vec3 pos = cube.getPosition();
		program.setUniform3f("light.position", pos.x, pos.y, pos.z);
		program.setUniform3f("light.ambient", 0.5f, 0.5f, 0.5f);
		program.setUniform3f("light.diffuse", 0.7f, 0.7f, 0.7f);
		program.setUniform3f("light.specular", 0.9f, 0.9f, 0.9f);

		for (unsigned int i = 0; i < 1; i++) {
			model.draw(program);

			program.setUniformMatrix4fv("model", 1, false, modelMatrix2.m[0]);
			model2.draw(program);
		}

		sphere.rotate({ 0.00f,PI / 25.0f * delta,0 });
		
		for (unsigned int i = 0; i < 1; i++) {
			cube.draw(camera);
			sphere.draw(camera);
			cylinder.draw(camera);
			cone.draw(camera);
		}
		

		if (segment != sPrev) {
			capsule.setSegment(segment);
			sPrev = segment;
		}

		if (radius != capsule.getRadius()) {
			capsule.setRadius(radius);
		}

		if (height != capsule.getHeight()) {
			capsule.setHeight(height);
		}

		if (point) {
			capsule.setDrawMode(Capsule::DrawMode::POINT);
			capsule.draw(camera);
		}

		if (tri) {
			capsule.setDrawMode(Capsule::DrawMode::TRIANGLE);
			capsule.draw(camera);
		}

		if (wf) {
			capsule.setDrawMode(Capsule::DrawMode::LINE);
			capsule.draw(camera);
		}
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
			camera.translate2(0, camSpeed * delta, 0);
		}
		if (handler.getKeyState(GLFW_KEY_LEFT_SHIFT)) {
			camera.translate2(0, -camSpeed * delta, 0);
		}
	}

	void imguiRender(float delta) {
		ImGui::Begin("Settings");

		ImGui::Text("FPS : %03d  Frame Time : %.3f", (int)(1.0f/delta),delta);
		ImGui::SliderFloat3("Cube Pos", ((float*)&cubePos), -20.0f, 20.0f);
		ImGui::SliderFloat3("Cube Scale", ((float*)&cubeScale), 0.1f, 20.0f);

		ImGui::SliderFloat("Rotation speed", &rotate, 0, 0.01f);

		ImGui::SliderInt("Segment", &segment, 2, 100);

		ImGui::Checkbox("Triangle", &tri);
		ImGui::Checkbox("Wire Frame", &wf);
		ImGui::Checkbox("point", &point);

		ImGui::SliderFloat("Radius", &radius, 0.1f, 10);
		ImGui::SliderFloat("Height", &height, 0.0f, 10);
		ImGui::End();
	}
};