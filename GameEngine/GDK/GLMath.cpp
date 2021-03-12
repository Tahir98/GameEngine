#pragma once
#include "GLMath.h"


Vec2 GLMath::nornalize(Vec2 a) {
	float length = a.lenght();
	return { a.x / length, a.y / length };
}

Vec3 GLMath::normalize(Vec3 a) {
	float length = a.lenght();
	return { a.x / length, a.y / length, a.z / length };
}

Vec4 GLMath::nornalize(Vec4 a) {
	float length = a.lenght();
	return { a.x / length, a.y / length, a.z / length , a.w / length };
}

float GLMath::dotProduct(const Vec2 a1,const Vec2 a2){
	return a1.x * a2.x + a1.y * a2.y;
}

float GLMath::dotProduct(const Vec3 a1,const Vec3 a2){
	return a1.x * a2.x + a1.y * a2.y + a1.z * a2.z;
}

float GLMath::dotProduct(const Vec4 a1,const Vec4 a2){
	return a1.x * a2.x + a1.y * a2.y + a1.z * a2.z + a1.w * a2.w;
}

Vec3 GLMath::crossProduct(const Vec3 a1,const Vec3 a2) {
	float x = a1.y * a2.z - a1.z * a2.y;
	float y = a1.z * a2.x - a1.x * a2.z;
	float z = a1.x * a2.y - a1.y * a2.x;
	return { x,y,z };
}

Matrix4x4 GLMath::lookat(Vec3 eye,Vec3 at,Vec3 up) {
	
	Vec3 cameraDirection = normalize(eye - at);
	Vec3 cameraRight = normalize(crossProduct(up, cameraDirection));
	Vec3 cameraUp = normalize(crossProduct(cameraDirection,cameraRight));

	Matrix4x4 direction = {
		cameraRight.x,cameraRight.y,cameraRight.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z,0,
		cameraDirection.x,cameraDirection.y,cameraDirection.z,0,
		0,0,0,1
	};
	Matrix4x4 pos = {
		1,0,0, -eye.x,
		0,1,0, -eye.y,
		0,0,1, -eye.z,
		0,0,0,1
	};

	return direction * pos;
}

Matrix4x4 GLMath::perspective(const float aspectRatio, const float fov, const float near, const float far){
	float fovRad = 1.0f / tanf(fov * 0.5f * PI / 180.0f);

	Matrix4x4 projection;
	projection.m[0][0] = aspectRatio * fovRad;
	projection.m[1][1] = fovRad;
	projection.m[2][2] = -far/(far -near);
	projection.m[2][3] = -(far * near) / (far - near);
	projection.m[3][2] = -1.0f;

	return projection;
}

Matrix4x4 GLMath::ortho(const float left, const float right, const float bottom, const float top, const float near,const float far){
	Matrix4x4 projection;

	projection.m[0][0] = 2.0f / (right - left);
	projection.m[0][3] = -(right + left) / (right - left);

	projection.m[1][1] = 2.0f / (top - bottom);
	projection.m[1][3] = -(top + bottom) / (top - bottom);
	
	projection.m[2][2] = -2.0f / (far - near);
	projection.m[2][3] = -(far + near) / (far - near);

	projection.m[3][3] = 1.0f;

	return projection;
}

Matrix4x4 GLMath::translate(const Vec3 pos){
	Matrix4x4 translation = {
		1.0f,0.0f,0.0f,pos.x,
		0.0f,1.0f,0.0f,pos.y,
		0.0f,0.0f,1.0f,pos.z,
		0.0f,0.0f,0.0f,1.0f
	};
	return translation;
}

Matrix4x4 GLMath::rotateX(const float radian){
	Matrix4x4 rot = {
		1,0,0,0,
		0,cosf(radian),-sinf(radian),0,
		0,sinf(radian),cosf(radian),0,
		0,0,0,1
	};

	return rot;
}

Matrix4x4 GLMath::rotateY(const float radian){
	Matrix4x4 rot = {
		cosf(radian),0,sinf(radian),0,
		0,1,0,0,
		-sinf(radian),0,cosf(radian),0,
		0,0,0,1
	};

	return rot;
}

Matrix4x4 GLMath::rotateZ(const float radian){
	Matrix4x4 rot = {
		cosf(radian),-sinf(radian),0,0,
		sinf(radian),cosf(radian),0,0,
		0,0,1,0,
		0,0,0,1
	};

	return rot;
}

Matrix4x4 GLMath::rotate(const float x, const float y, const float z){
	return rotateX(x) * rotateY(y) * rotateZ(z);
}

Matrix4x4 GLMath::rotate(const Vec3 rot){
	return rotateX(rot.x) * rotateY(rot.y) * rotateZ(rot.z);
}

Matrix4x4 GLMath::scale(const Vec3 a){
	Matrix4x4 s = {
		a.x,0,0,0,
		0,a.y,0,0,
		0,0,a.z,0,
		0,0,0,1
	};

	return s;
}

Matrix4x4 GLMath::scale(const float x, const float y, const float z){
	Matrix4x4 s = {
		x,0,0,0,
		0,y,0,0,
		0,0,z,0,
		0,0,0,1
	};
	return s;
}

float GLMath::MtoRadian(float degree){
	return PI * degree / 180.0f;
}
