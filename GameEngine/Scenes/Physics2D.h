#include "Scene.h"
#include "InputHandler.h"
#include "ShapeRenderer2D.h"
#include <vector>
#include "GLMath.h"
#include <stdio.h>
#include "Core.h"
#include "Text/TextRenderer.h"
#include "imgui.h"
#include <iostream>

struct Particle {
	float x,y;
	float radius;
	Vec2 velocity;
	float mass;
	Vec3 color;
};

class Physics : public Scene {
private:
	InputHandler handler;
	ShapeRenderer2D renderer;

	std::vector<Particle> particles;

	float gravityY = -0.6f;
	float gravityX =  0.0f;

	int segment = 40;

	TextRenderer text;

public:

	Physics(GLFWwindow* window) :Scene(window),handler(window),text("assets/arial.ttf",40) {
	
	}


	void update(const float delta) override {


		input(delta);

		for (unsigned int i = 0; i < particles.size(); i++) {
			Particle& particle = particles[i];

			particle.velocity.y += gravityY * delta * 60.0f;
			particle.y += particle.velocity.y * delta * 60.0f;

			particle.velocity.x += gravityX * delta * 60.0f;
			particle.x += particle.velocity.x * delta * 60.0f;

			//Collision width walls
			if (particle.y <= particle.radius) {
				particle.y = 2.0f * particle.radius - particle.y;
				particle.velocity.y *= -0.9f;
			}
			else if (particle.y + particle.radius >= Application::height) {
				particle.y = 2.0f * Application::height - 2.0f * particle.radius - particle.y;
				particle.velocity.y *= -0.9f;
			}

			if (particle.x <= particle.radius) {
				particle.x = 2.0f * particle.radius - particle.x;
				particle.velocity.x *= -0.9f;
			}
			else if (particle.x + particle.radius >= Application::width) {
				particle.x = 2.0f * Application::width - 2.0f * particle.radius - particle.x;
				particle.velocity.x *= -0.9f;
			}

			//Collision with each other
			for (unsigned int j = 0; j < particles.size(); j++) {
				if (j == i)
					continue;
				Particle& particle2 = particles[j];
				float disX = (particle2.x - particle.x);
				float disY = (particle2.y - particle.y);
				if (disX * disX + disY * disY <= (particle.radius + particle2.radius) * (particle.radius + particle2.radius)) {
					float distance = sqrtf(disX * disX + disY * disY);
					float overlap = 0.5f * (distance - particle.radius - particle2.radius);

					float totalMass = particle.mass + particle2.mass;
					particle.x -= overlap * (particle.x - particle2.x) / distance;
					particle.y -= overlap * (particle.y - particle2.y) / distance;

					particle2.x += overlap * (particle.x - particle2.x) / distance;
					particle2.y += overlap * (particle.y - particle2.y) / distance;


					Vec2 v1(particle.velocity), v2(particle2.velocity);
					v1 = v1 - v2;
					Vec2 temp(v1);
					disX = particle2.x - particle.x;
					disY = particle2.y - particle.y;
					distance = sqrtf(disX * disX + disY * disY);
					v1.x = temp.lenght();
				
				
				}				
			}
		}

		imguiDraw();

		renderer.begin(ShapeType::FILLED);
		
		for (const auto& particle : particles) {
			renderer.setColor(particle.color.x, particle.color.y, particle.color.z,1);
			renderer.circle(particle.x, particle.y, particle.radius, segment);
		}

		renderer.end();

		std::string size = std::to_string(particles.size());
		text.write(size, Application::width - 100, Application::height - 50, 1,{ 0,0,0 });
	}


	void input(const float delta) override {
		if (handler.getMouseButtonState(GLFW_MOUSE_BUTTON_2)) {
			double x, y;
			glfwGetCursorPos(window,&x,&y);

			y = Application::height - y;
			
			bool isEmpty = true;
			for (unsigned int i = 0; i < particles.size();i++) {
				Particle particle = particles[i];
				float diffX = x - particle.x;
				float diffY = y - particle.y;
				if (x + particle.radius > Application::width || x - particle.radius < 0) {
					isEmpty = false;
					break;
				}

				if (y + particle.radius > Application::height || y - particle.radius < 0) {
					isEmpty = false;
					break;
				}

				if (diffX * diffX + diffY * diffY <= 2 * particle.radius * 2 * particle.radius) {
					isEmpty = false;
					break;
				}
			}

			if (isEmpty) {
				for (unsigned int i = 0; i < 1; i++) {
					Particle particle;
					particle.x = (float)x;
					particle.y = (float)y;
					particle.radius = 20 + rand() % 60;
					particle.mass = particle.radius * particle.radius;
					particle.velocity = { (float)(rand() % 20 - 10),(float)(rand() % 20 - 10) };
					particle.color = { (float)(rand() % 256) / 256.0f,(float)(rand() % 256) / 256.0f,(float)(rand() % 256) / 256.0f };

			
					particles.push_back(particle);
				}
			}
		}

		if (handler.getKeyState(GLFW_KEY_C)) {
			particles.clear();
		}
	}

	void imguiDraw() {
		ImGui::Begin("Settings");
		ImGui::SliderFloat("gravityY", &gravityY, -5, 5);
		ImGui::SliderFloat("gravityX", &gravityX, -5, 5);
		ImGui::SliderInt("Segment",&segment,3,50);
		ImGui::End();
	}
};