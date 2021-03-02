#include "Scene.h"
#include "imgui.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Program.h"
#include "Texture.h"
#include <vector>
#include "SpriteBatch.h"

class SpriteScene : public Scene {

private:

	std::vector<float> vertices = {
		-0.5f, -0.5f, 0,	1,0,0,1,	0,0,
		 0.5f, -0.5f, 0,	1,0,0,1,	1,0,
		 0.5f,  0.5f, 0,	1,0,0,1,	1,1,
					  		
		-0.5f, -0.5f, 0,	1,0,0,1,	0,0,
		 0.5f,  0.5f, 0,	1,0,0,1,	1,1,
		-0.5f,  0.5f, 0,	1,0,0,1,	0,1
	};

	VertexArray va;
	VertexBuffer vb;
	Texture texture;
	Texture texture2;
	Program program;

	float counter = 0;
	float fps = 0;
	float frametime = 0;

	SpriteBatch batch;

public:
	SpriteScene(GLFWwindow* window) : Scene(window), vb(vertices.data(), sizeof(float)* vertices.size(), GL_STATIC_DRAW),
		texture("assets/icon.png"),texture2("assets/city.png"),program("Shaders/sample.shader")
	{
		vb.bind();
		va.bind();

		va.addVertexAttribute(3, GL_FLOAT, false);
		va.addVertexAttribute(4, GL_FLOAT, false);
		va.addVertexAttribute(2, GL_FLOAT, false);

	}

	void update(float delta) {
		input(delta);
		guiRender(delta);
		
		batch.begin();

		batch.draw(texture, 50, 50, 200, 200);
		batch.draw(texture2, 500, 50, 200, 200);
		batch.draw(texture2, 700, 50, 200, 200);

		batch.end();

	}


	void input(float delta) {
		
	}

	void guiRender(float delta) {
		
		counter += delta;
		if (counter >= 0.5f) {
			fps = 1.0f / delta;
			frametime = delta;
			counter -= 0.5f;
		}

		ImGui::Begin("Deneme");

		ImGui::Text("FPS: %.3f  Frame time: %.5f" , fps,frametime);

		ImGui::End();
	}

};
