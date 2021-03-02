#pragma once

#include "GL/glew.h"
#include <iostream>

class EngineLog{
public:
	
	void glError(){
		int error;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "ERROR: " << error << std::endl;
		}
	}
};