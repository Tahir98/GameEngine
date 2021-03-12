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
};
