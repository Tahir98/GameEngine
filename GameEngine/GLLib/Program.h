#pragma once
#include <vector>
#include <utility>

class Program{
private:
	unsigned int id;
	unsigned int vsID, fsID;
	const char* filePath;
	std::vector<std::pair<const char*,const int>> uniforms;

public:
	Program(const char* path);
	~Program();
	void bind();
	void unBind();
	void deleteProgram();
	void recreate(const char* path, bool deletePrevious);

	//uniform functions pass values to uniform variales in shaders
	void setUniformi(const char* name,const int value);
	void setUniformiv(const char* name,const unsigned int count,const int* value);
	void setUniform1f(const char* name,const float value);
	void setUniform2f(const char* name,const float v1,const float v2);
	void setUniform3f(const char* name,const float v1,const float v2,const float v3);
	void setUniform4f(const char* name,const float v1,const float v2,const float v3,const float v4);
	void setUniformMatrix4fv(const char* name, const unsigned int count, const bool transpose, const float* pos);

private:
	void compileShader();
};

