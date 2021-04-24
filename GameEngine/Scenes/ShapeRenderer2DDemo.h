#pragma once

#include "Scene.h"
#include "ShapeRenderer2D.h"


class ShapeRenderer2DDemo : public Scene {
private:
	
	ShapeRenderer2D renderer;
public:

	ShapeRenderer2DDemo(GLFWwindow* window) :Scene(window) {

	}


	void update(const float delta) override {
		renderer.begin(ShapeType::FILLED);

		renderer.setColor(color::green);
		renderer.triangle(40,650,240,650,140,850);

		renderer.setColor(color::cyan);
		renderer.rect(280, 650, 240, 250);
		renderer.rect(560, 650, 240, 250, color::white, color::red, color::green, color::blue);

		renderer.setColor(color::red);
		renderer.circle(960,770,120);
		renderer.circle(1240,770,120,50,color::white,color::black);

		renderer.setColor(color::blue);
		renderer.ellipse(1520, 770, 120, 80, (unsigned int)50);

		renderer.setColor(color::yellow);
		renderer.arc(1800,770,120,30,300);
		
		renderer.end();


		renderer.begin(ShapeType::LINE);

	
		renderer.setColor(color::white);
		renderer.triangle(40, 250, 240, 250, 140, 450);

		renderer.rect(280, 250, 240, 250);

		renderer.circle(960, 375, 120);

		renderer.polygon(680, 375, 120,8,22.5f);

		renderer.ellipse(1520, 375, 120, 80, (unsigned int)50);
		
		float pos[] = {
			1120,375, 1220,375,1240,478,1360,300	
		};

		renderer.line(pos,8);

		renderer.arc(1800, 375, 120, 30, 300);
		renderer.end();
		
	}


	void input(const float delta) override {
		
	}
};