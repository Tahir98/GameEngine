#include "Scene.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <vector>
#include <stdlib.h>
#include "Core.h"
#include "ShapeRenderer2D.h"
#include <iostream>

static unsigned int elements;

class ParticalScene : public Scene {

	struct Partical {
		float x, y;
		float lifeDuration;
		float time;
		float size;
		float angle;
		float velocity;
	};

private:
	Program* program = nullptr;
	VertexBuffer* vb = nullptr;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;

	std::vector<Partical> particals;

	float density = 5;
	float duration = 10;
	float velocity = 6.2f;
	float size = 20.0f;

	float remainder = 0;

	ShapeRenderer2D renderer;

public:
	ParticalScene(GLFWwindow* window) : Scene(window) {
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		renderer.setColor({0,1,0,1});
	}
	
	void update(float delta) {
		input(delta);

		renderer.begin(ShapeType::FILLED);

		for (unsigned int i = 0; i < particals.size();) {
			particals[i].x += particals[i].velocity * cosf(particals[i].angle) * delta * 60.0f;
			particals[i].y += particals[i].velocity * sinf(particals[i].angle) * delta * 60.0f;

			particals[i].time += delta;
			if (particals[i].time >= particals[i].lifeDuration) {
				particals.erase(particals.begin() + i);
				continue;
			}
			renderer.rect(particals[i].x, particals[i].y, particals[i].size, particals[i].size);
			i++;
		}

		renderer.end();

		elements = particals.size();

	}
	
	void input(float delta) {
		if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			double xPos, yPos;
			glfwGetCursorPos(window,&xPos,&yPos);
			yPos = Application::height - yPos; 

			for (unsigned int i = 0; i < density * delta * 60.0f; i++) {
				particals.push_back({ (float)xPos,(float)yPos,duration,0.0f,size,(float)(rand() % 360),velocity });
			}

			remainder += (density * delta * 60.0f) - (int)(density * delta * 60.0f);
			
			if (remainder >= 1.0f) {
				particals.push_back({ (float)xPos,(float)yPos,duration,0.0f,size,(float)(rand() % 360),velocity });
				remainder -= 1.0f;
			}
		}
	}

};