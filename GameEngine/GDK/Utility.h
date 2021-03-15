#pragma once
#include "GLMath.h"

struct Material {
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	float shininess;
};

struct Light {
	Vec3 pos;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;

	float constant = 1.0f;
	float linear = 0.007f;
	float quadrantic = 0.0002f;
};
