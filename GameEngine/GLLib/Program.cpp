#include "GL/glew.h"
#include "Program.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

Program::Program(const char* path): filePath(path){
	id = glCreateProgram();
	compileShader();
}
Program::~Program() {
	glDeleteProgram(id);
}


void Program::bind(){
	glUseProgram(id);
}
void Program::unBind() {
	glUseProgram(0);
}

void Program::deleteProgram(){
	glDeleteProgram(id);
}

void Program::recreate(const char* path,bool deletePrevios){
	filePath = path;

	if (deletePrevios) {
		glDeleteProgram(id);
		id = glCreateProgram();
	} 
	else {
		glDetachShader(id, vsID);
		glDetachShader(id, fsID);
	}

	compileShader();
}

void Program::compileShader() {

	std::ifstream stream(filePath);
	if (!stream.is_open()) {
		std::cout << "File not found" << std::endl;
	}

	std::string line;
	int type = -1;

	std::string vs, fs;

	while (getline(stream, line)) {
		//std::cout << line << std::endl;

		if (line.find("#vertex shader") != std::string::npos)
			type = 0;
		else if (line.find("#fragment shader") != std::string::npos)
			type = 1;
		else {
			if (type == 0)
				vs += line + "\n";
			else if (type == 1)
				fs += line + "\n";
		}
	}

	const char* vsSource = vs.c_str();
	const char* fsSource = fs.c_str();

	vsID = glCreateShader(GL_VERTEX_SHADER);
	fsID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsID,1,&vsSource,nullptr);
	glShaderSource(fsID, 1, &fsSource, nullptr);

	glCompileShader(vsID);
	glCompileShader(fsID);

	int success;
	glGetShaderiv(vsID,GL_COMPILE_STATUS,&success);
	if (!success) {
		int length;
		glGetShaderiv(vsID,GL_INFO_LOG_LENGTH,&length);
		char* message = (char*)malloc(length);
		glGetShaderInfoLog(vsID,length,&length,message);

		std::cout << "Failed compile vertex shader: " << message << std::endl;

		free(message);
	}

	glGetShaderiv(fsID, GL_COMPILE_STATUS, &success);
	if (!success) {
		int length;
		glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length);
		glGetShaderInfoLog(fsID, length, &length, message);

		std::cout << "Failed compile fragment shader: " << message << std::endl;

		free(message);
	}

	glAttachShader(id,vsID);
	glAttachShader(id,fsID);

	glLinkProgram(id);

	glGetProgramiv(id,GL_LINK_STATUS,&success);
	if (!success) {
		int length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length);
		glGetProgramInfoLog(id, length, &length, message);

		std::cout << "Failed to link program: " << message << std::endl;
		free(message);
	}

	glValidateProgram(id);

	glGetProgramiv(id, GL_VALIDATE_STATUS, &success);
	if (!success) {
		int length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length);
		glGetProgramInfoLog(id, length, &length, message);

		std::cout << "Failed to validate program: " << message << std::endl;
		free(message);
	}
	
	glDeleteShader(vsID);
	glDeleteShader(fsID);
}


void Program::setUniformi(const char* name,const int value){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}
	
	if (isFound) {
		glUniform1i(location, value);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniform1i(location, value);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}	
	}
}

void Program::setUniformiv(const char* name, const unsigned int count, const int* value){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}

	if (isFound) {
		glUniform1iv(location, count, value);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniform1iv(location, count, value);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}
	}
}

void Program::setUniform1f(const char* name, float value){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}

	if (isFound) {
		glUniform1f(location, value);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniform1f(location, value);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}
	}
}

void Program::setUniform2f(const char* name, float v1, float v2){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}

	if (isFound) {
		glUniform2f(location, v1, v2);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniform2f(location, v1, v2);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}
	}
}

void Program::setUniform3f(const char* name, float v1, float v2, float v3){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}

	if (isFound) {
		glUniform3f(location, v1, v2, v3);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniform3f(location, v1, v2, v3);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}
	}
}

void Program::setUniform4f(const char* name, float v1, float v2, float v3, float v4){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}

	if (isFound) {
		glUniform4f(location, v1, v2, v3, v4);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniform4f(location, v1, v2, v3, v4);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}
	}
}

void Program::setUniformMatrix4fv(const char* name,const unsigned int count,const bool transpose,const float* pos){
	bind();
	bool isFound = false;
	int location;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		if (uniforms[i].first == name) {
			isFound = true;
			location = uniforms[i].second;
			break;
		}
	}

	if (isFound) {
		glUniformMatrix4fv(location, count, transpose, pos);
		return;
	}

	if (!isFound) {
		location = glGetUniformLocation(id, name);
		if (location != -1) {
			glUniformMatrix4fv(location, count, transpose, pos);
			uniforms.push_back(std::make_pair(name, location));
		}
		else {
			std::cout << name << " :uniform location is not found" << std::endl;
		}
	}
}












