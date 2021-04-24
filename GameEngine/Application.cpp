#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Core.h"
#include "Scenes/ShapesScene.h"
//#include "Scenes/Physics2D.h"
//#include "Scenes/ModelScene.h"
//#include "Scenes/FlappyBird.h"
//#include "Scenes/ParticalScene.h"
//#include "Scenes/ShapeRenderer2DDemo.h"
#include <string>
#include <iostream>
#include "GLMath.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "EngineLog.h"


float counter = 0;
float delta;

int main() {

	if (!glfwInit())
		return -1;

	

	GLFWwindow* window = glfwCreateWindow((int)Application::width,(int)Application::height,"Opengl FPS = 60",nullptr,nullptr);
	

	if (window != nullptr)
		glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_RENDERER) << std::endl;

	float prev = 0, now = (float)glfwGetTime();
	ShapesScene scene(window);
	//FlappyBird scene(window);
	//ParticalScene scene(window);
	//ShapeRenderer2DDemo scene(window);
	//ModelScene scene(window);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDepthFunc(GL_LEQUAL);

	glfwSwapInterval(0);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	//Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	EngineLog elog;

	while (!glfwWindowShouldClose(window)){
		//glClearColor(0.392156899f, 0.584313750f, 0.929411829f, 1);
		glClearColor(0.5f,0.5f,0.5f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		
		prev = now;
		now = (float)glfwGetTime();
		delta = now - prev;

		counter += delta;
		if(counter> 1.0f){
			unsigned int fps = (unsigned int)(1.0f / (delta));
			std::string Title = "OpenGL FPS = " + std::to_string(fps);
			glfwSetWindowTitle(window, Title.c_str());
			counter--;
		}

		
		scene.update(delta);
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);


		elog.glError();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}