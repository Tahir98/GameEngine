#pragma once
#include "GLFW/glfw3.h"

class Scene {
protected:
	GLFWwindow* window;

public:
	Scene(GLFWwindow* window):window(window){}


	virtual void update(const float delta){}
	virtual void input(const float delta){}
};