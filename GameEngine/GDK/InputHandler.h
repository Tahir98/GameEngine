
class InputHandler {

private:
	
	void init();
public:
	InputHandler(void* window);
	~InputHandler();

	bool getKeyState(int key);
	bool getMouseButtonState(int button);

};