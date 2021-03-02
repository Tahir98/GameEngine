#include "Scene.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "PerspectiveCamera.h"
#include "GLMath.h"
#include "InputHandler.h"
#include <vector>
#include "imgui.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Model/Model.h"



float pos[] = {
	0,0,0,	0,0,0,1, //0
	0,1,0,	0,1,0,1, //1
	1,1,0,	1,1,0,1, //2
	1,0,0,	1,0,0,1, //3
	1,0,1,	1,0,1,1, //4
	1,1,1,	1,1,1,1, //5
	0,1,1,	0,1,1,1, //6
	0,0,1,	0,0,1,1	 //7
};

unsigned int ind[] = {
	0,1,2,	0,2,3,
	3,2,5,	3,5,4,
	4,5,6,	4,6,7,
	7,6,1,	7,1,0,
	1,6,5,	1,5,2,
	7,0,3,	7,3,4
};


class LightingScene : public Scene {
private:
	Program* objectProgram = nullptr;
	VertexArray* objectVA = nullptr;
	VertexBuffer* objectVB = nullptr;
	
	Program* lightProgram = nullptr;
	VertexArray* lightVA = nullptr;
	VertexBuffer* lightVB = nullptr;
	IndexBuffer* lightIB = nullptr;

	Vec3 lightPos = {0,0,-3};
	Vec4 lightColor = { 1,1,1,1 };
	Matrix4x4 lightModel;
	Matrix4x4 view;

	Matrix4x4 objectModel;
	Vec3 objectPos = {0,0,0};

	PerspectiveCamera camera;
	InputHandler* inputHandler;

	bool first = true;
	float prevX, prevY;

	float sensivity = 0.03f;
	float camSpeed = 6.5f;

	float a = 16.0f/9.0f;

	float counter = 0;

	std::vector<float> buffer;

	ModelLoader loader;
	int modelSize = 0;;

	Texture* texture;
	SpriteBatch batch;

	Model* model;

public:
	
	LightingScene(GLFWwindow* window) : Scene(window),camera(50, 9.0f/16.0f,0.1f,100) {
		
		lightProgram = new Program("Shaders/basic.shader");
		lightVB = new VertexBuffer(pos,sizeof(pos),GL_STATIC_DRAW);
		lightVA = new VertexArray();
		lightIB = new IndexBuffer(ind,sizeof(ind),GL_STATIC_DRAW);

		lightVB->bind();
		lightVA->bind();

		lightVA->addVertexAttribute(3, GL_FLOAT, false);
		lightVA->addVertexAttribute(4, GL_FLOAT, false);

		loader.load("assets/plane/airplane.obj");
		//loader.load("C:/Users/Tahir/Downloads/cartoon.obj");
	
		
		std::vector<float>* tri = loader.getTriangulatedData();
		modelSize = (int)(tri->size());

		objectProgram = new Program("Shaders/example.shader");
		objectVB = new VertexBuffer(tri->data(), sizeof(float) * (int)(tri->size()),GL_STATIC_DRAW);
		objectVA = new VertexArray();

		objectVA->bind();
		objectVB->bind();

		objectVA->addVertexAttribute(3, GL_FLOAT, false);
		objectVA->addVertexAttribute(3, GL_FLOAT, false);
		objectVA->addVertexAttribute(2, GL_FLOAT, false);

		inputHandler = new InputHandler(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		texture = new Texture("assets/plane/diffuse.png");

		model = new Model("assets/cottage/cottage_obj.obj");
	}

	~LightingScene() {
		delete objectProgram;
		delete objectVA;
		delete objectVB;

		delete lightProgram;
		delete lightVA;
		delete lightVB;
		delete lightIB;

		delete inputHandler;

		delete texture;
	}


	void update(float delta){
		input(delta);
		imguiTest(delta);

		counter += 1.0f*delta;
		lightModel = GLMath::translate(lightPos) * GLMath::scale(0.03f, 0.03f, 0.03f);
		objectModel = GLMath::translate(objectPos) * GLMath::scale(0.9f, 0.9f, 0.9f);

		camera.update(delta);

		lightProgram->bind();
		lightVA->bind();
		lightIB->bind();

		lightProgram->setUniformMatrix4fv("model", 1, false, lightModel.m[0]);
		lightProgram->setUniformMatrix4fv("view", 1, false,camera.getViewMatrix());
		lightProgram->setUniformMatrix4fv("projection", 1, false, camera.combine());

		glDrawElements(GL_TRIANGLES,sizeof(ind)/sizeof(unsigned int),GL_UNSIGNED_INT,nullptr);

		objectProgram->bind();
		objectVA->bind();
		objectVB->bind();
		texture->bind(0);

		objectProgram->setUniformMatrix4fv("model", 1, false, objectModel.m[0]);
		objectProgram->setUniformMatrix4fv("view", 1, false, camera.getViewMatrix());
		objectProgram->setUniformMatrix4fv("projection", 1, false, camera.combine());

		objectProgram->setUniformi("tex",0);
		objectProgram->setUniform3f("lightPos",lightPos.x,lightPos.y,lightPos.z);
		objectProgram->setUniform3f("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		objectProgram->setUniform3f("lightColor",lightColor.x, lightColor.y, lightColor.z);

		glDrawArrays(GL_TRIANGLES, 0, modelSize/8);

		//batch.begin();
		//batch.draw(*texture, 0, 0, 300, 300);
		//batch.end();
	}

	void input(float delta) override {

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		y = Application::height - y;
		
		if (first) {
			prevX = (float)x;
			prevY = (float)y;
			first = false;
		}

		float xDiff = (float)x - prevX;
		float yDiff = (float)y - prevY;

		prevX = (float)x;
		prevY = (float)y;

		camera.rotX(yDiff * sensivity);
		camera.rotY(xDiff * sensivity);

		if (inputHandler->getKeyState(GLFW_KEY_A)) {
			camera.translate2(-camSpeed * delta,0,0);
		}
		else if (inputHandler->getKeyState(GLFW_KEY_D)) {
			camera.translate2(camSpeed * delta, 0, 0);
		}

		if (inputHandler->getKeyState(GLFW_KEY_W)) {
			camera.translate2(0, 0, camSpeed * delta);
		}
		else if (inputHandler->getKeyState(GLFW_KEY_S)) {
			camera.translate2(0, 0, -camSpeed * delta);
		}

		if (inputHandler->getKeyState(GLFW_KEY_SPACE)) {
			camera.translate2(0, camSpeed * delta, 0);
		}
		if (inputHandler->getKeyState(GLFW_KEY_LEFT_SHIFT)) {
			camera.translate2(0, -camSpeed * delta, 0);
		}


		if (inputHandler->getKeyState(GLFW_KEY_UP)) {
			lightPos.z += -2 * delta;
		}
		else if (inputHandler->getKeyState(GLFW_KEY_DOWN)) {
			lightPos.z += 2 * delta;
		}

		if (inputHandler->getKeyState(GLFW_KEY_LEFT)) {
			lightPos.x += -2 * delta;
		}
		else if (inputHandler->getKeyState(GLFW_KEY_RIGHT)) {
			lightPos.x += 2 * delta;
		}

		if (inputHandler->getKeyState(GLFW_KEY_RIGHT_CONTROL)) {
			lightPos.y += -2 * delta;
		}
		else if (inputHandler->getKeyState(GLFW_KEY_RIGHT_SHIFT)) {
			lightPos.y += 2 * delta;
		}
	}

	void imguiTest(float delta) {
		
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                         

		ImGui::Text("This is some useful text.");              

		ImGui::SliderFloat("x", &lightPos.x, -10.0f, 10.0f);
		ImGui::SliderFloat("y", &lightPos.y, -10.0f, 10.0f);
		ImGui::SliderFloat("z", &lightPos.z, -10.0f, 10.0f);

		ImGui::ColorEdit3("clear color", &lightColor.x); 

		if (ImGui::Button("Button")) 
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application fps %.3f",1.0f / delta );
		ImGui::End();

	
	}
};

