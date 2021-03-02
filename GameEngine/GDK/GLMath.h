#pragma once
#include <cmath>

#ifndef MY_MATH_H
#define MY_MATH_



struct Vec2 {
	float x, y;

	Vec2 operator+(Vec2 v) {
		v.x += this->x;
		v.y += this->y;
		return v;
	}

	Vec2 operator-(Vec2 v) {
		v.x = this->x - v.x;
		v.y = this->y - v.y;
		return v;
	}

	Vec2 operator*(float in) {
		Vec2 a;
		a.x = this->x * in;
		a.y = this->y * in;
		return a;
	}
	
	friend Vec2 operator*(float in,Vec2 a) {
		return a * in;
	}
	
	float lenght() {
		return (float)sqrt(x * x + y * y);
	}

	void normalize() {
		float l = lenght();
		x = x / l;
		y = y / l;
	}
};


struct Vec3 {
	float x, y, z;

	Vec3 operator+(Vec3 v) {
		v.x += this->x;
		v.y += this->y;
		v.z += this->z;
		return v;
	}

	Vec3 operator-(Vec3 v) {
		v.x = this->x - v.x;
		v.y = this->y - v.y;
		v.z = this->z - v.z;
		return v;
	}

	Vec3 operator*(float in) {
		Vec3 a;
		a.x = this->x * in;
		a.y = this->y * in;
		a.z = this->z * in;
		return a;
	}

	friend Vec3 operator*(float in,Vec3 a) {
		return a * in;
	}

	float lenght() {
		return (float)sqrt(x * x + y * y + z * z);
	}

	Vec3 cross(Vec3 in) {
		float _x = y * in.z - z * in.y;
		float _y = z * in.x - x * in.z;
		float _z = x * in.y - y * in.z;
		return { _x,_y,_z };
	}

	void normalize() {
		float l = lenght();
		x = x / l;
		y = y / l;
		z = z / l;
	}

};

struct Vec4 {
	float x, y, z, w;

	Vec4 operator+(Vec4 v) {
		v.x += this->x;
		v.y += this->y;
		v.z += this->z;
		v.w += this->w;
		return v;
	}

	Vec4 operator-(Vec4 v) {
		v.x = this->x - v.x;
		v.y = this->y - v.y;
		v.z = this->z - v.z;
		v.w = this->w - v.w;
		return v;
	}

	Vec4 operator*(float in) {
		Vec4 a;
		a.x = this->x * in;
		a.y = this->y * in;
		a.z = this->z * in;
		a.w = this->w * in;
		return a;
	}

	friend Vec4 operator*(float in,Vec4 a) {
		return a * in;
	}

	float lenght() {
		return (float)sqrt(x * x + y * y + z * z + w * w);
	}
	Vec4 normalize() {
		float l = lenght();
		x = x / l;
		y = y / l;
		z = z / l;
		w = w / l;
	}
};

struct Matrix4x4 {
	float m[4][4] = { 0.0f };

	Matrix4x4 operator+(float a) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] += a;
		return *this;
	}

	friend Matrix4x4 operator+(float a,Matrix4x4 b) {
		return b + a;
	}

	Matrix4x4 operator+(Matrix4x4 a) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a.m[i][j] += m[i][j];
		return a;
	}
	Matrix4x4 operator-(float a) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] -= a;
		return *this;
	}

	friend Matrix4x4 operator-(float a ,Matrix4x4 b){
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				b.m[i][j] = a - b.m[i][j];
		return b;
	}

	Matrix4x4 operator-(Matrix4x4 a) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a.m[i][j] = m[i][j] - a.m[i][j];
		return a;
	}

	Matrix4x4 operator*(float a) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] *= a;
		return *this;
	}

	friend Matrix4x4 operator*(float a,Matrix4x4 b) {
		return  b * a;
	}

	Matrix4x4 operator/(float a) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] /= a;
		return *this;
	}

	Matrix4x4 operator*(Matrix4x4 a) {
		Matrix4x4 temp;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					temp.m[i][j] += m[i][k] * a.m[k][j];
		return temp;
	}

	Vec4 operator*(Vec4 in) {
		float x = in.x * m[0][0] + in.y * m[1][0] + in.z * m[2][0] + in.w * m[3][0];
		float y = in.x * m[0][1] + in.y * m[1][1] + in.z * m[2][1] + in.w * m[3][1];
		float z = in.x * m[0][2] + in.y * m[1][2] + in.z * m[2][2] + in.w * m[3][2];
		float w = in.x * m[0][3] + in.y * m[1][3] + in.z * m[2][3] + in.w * m[3][3];
		return{ x,y,z,w };
	}

	friend Vec4 operator*(Vec4 a, Matrix4x4 b) {
		a.x = a.x * b.m[0][0] + a.y * b.m[1][0] + a.z * b.m[2][0] + a.w * b.m[3][0];
		a.y = a.x * b.m[0][1] + a.y * b.m[1][1] + a.z * b.m[2][1] + a.w * b.m[3][1];
		a.z = a.x * b.m[0][2] + a.y * b.m[1][2] + a.z * b.m[2][2] + a.w * b.m[3][2];
		a.w = a.x * b.m[0][3] + a.y * b.m[1][3] + a.z * b.m[2][3] + a.w * b.m[3][3];
		return a;
	}
};

struct Matrix3x3 {
	float m[3][3] = { 0.0f };

	Matrix3x3 operator+(float a) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] += a;
		return *this;
	}

	friend Matrix3x3 operator+(float a, Matrix3x3 b) {
		return b + a;
	}

	Matrix3x3 operator+(Matrix3x3 a) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				a.m[i][j] += m[i][j];
		return a;
	}
	Matrix3x3 operator-(float a) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] -= a;
		return *this;
	}

	friend Matrix3x3 operator-(float a, Matrix3x3 b) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				b.m[i][j] = a - b.m[i][j];
		return b;
	}

	Matrix3x3 operator-(Matrix3x3 a) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				a.m[i][j] = m[i][j] - a.m[i][j];
		return a;
	}

	Matrix3x3 operator*(float a) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] *= a;
		return *this;
	}

	friend Matrix3x3 operator*(float a, Matrix3x3 b) {
		return  b * a;
	}

	Matrix3x3 operator/(float a) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] /= a;
		return *this;
	}

	Matrix3x3 operator*(Matrix3x3 a) {
		Matrix3x3 temp;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					temp.m[i][j] += m[i][k] * a.m[k][j];
		return temp;
	}

	Vec3 operator*(Vec3 in) {
		float x = in.x * m[0][0] + in.y * m[1][0] + in.z * m[2][0];
		float y = in.x * m[0][1] + in.y * m[1][1] + in.z * m[2][1];
		float z = in.x * m[0][2] + in.y * m[1][2] + in.z * m[2][2];
		
		return{ x,y,z };
	}

	friend Vec3 operator*(Vec3 a, Matrix3x3 b) {
		a.x = a.x * b.m[0][0] + a.y * b.m[1][0] + a.z * b.m[2][0];
		a.y = a.x * b.m[0][1] + a.y * b.m[1][1] + a.z * b.m[2][1];
		a.z = a.x * b.m[0][2] + a.y * b.m[1][2] + a.z * b.m[2][2];
		return a;
	}
};

struct Matrix2x2{
	float m[2][2] = { 0.0f };

	Matrix2x2 operator+(float a) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				m[i][j] += a;
		return *this;
	}

	friend Matrix2x2 operator+(float a, Matrix2x2 b) {
		return b + a;
	}

	Matrix2x2 operator+(Matrix2x2 a) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				a.m[i][j] += m[i][j];
		return a;
	}
	Matrix2x2 operator-(float a) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				m[i][j] -= a;
		return *this;
	}

	friend Matrix2x2 operator-(float a, Matrix2x2 b) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				b.m[i][j] = a - b.m[i][j];
		return b;
	}

	Matrix2x2 operator-(Matrix2x2 a) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				a.m[i][j] = m[i][j] - a.m[i][j];
		return a;
	}

	Matrix2x2 operator*(float a) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				m[i][j] *= a;
		return *this;
	}

	friend Matrix2x2 operator*(float a, Matrix2x2 b) {
		return  b * a;
	}

	Matrix2x2 operator/(float a) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				m[i][j] /= a;
		return *this;
	}

	Matrix2x2 operator*(Matrix2x2 a) {
		Matrix2x2 temp;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					temp.m[i][j] += m[i][k] * a.m[k][j];
		return temp;
	}

	Vec2 operator*(Vec2 in) {
		float x = in.x * m[0][0] + in.y * m[1][0];
		float y = in.x * m[0][1] + in.y * m[1][1];
		
		return{ x,y };
	}

	friend Vec2 operator*(Vec2 a, Matrix2x2 b) {
		a.x = a.x * b.m[0][0] + a.y * b.m[1][0];
		a.y = a.x * b.m[0][1] + a.y * b.m[1][1];
		return a;
	}
};


namespace GLMath {
#define PI  3.14159f

	Vec2 nornalize(Vec2 a);
	Vec3 normalize(Vec3 a); 
	Vec4 nornalize(Vec4 a);

	float dotProduct(const Vec2 a1, Vec2 a2);
	float dotProduct(const Vec3 a1, Vec3 a2);
	float dotProduct(const Vec4 a1, Vec4 a2);
	Vec3 crossProduct(const Vec3 a1, Vec3 a2);
	Matrix4x4 lookat(Vec3 eye, Vec3 at, Vec3 up);
	Matrix4x4 perspective(const float aspectRatio,const float fov,const float near,const float far);
	Matrix4x4 ortho(const float left,const float right,const float bottom,const float top,const float near,const float far);

	Matrix4x4 translate(const Vec3 pos);
	Matrix4x4 rotateX(const float radian);
	Matrix4x4 rotateY(const float radian);
	Matrix4x4 rotateZ(const float radian);
	Matrix4x4 rotate(const float x, const float y, const float z);
	Matrix4x4 rotate(const Vec3 rot);
	Matrix4x4 scale(const Vec3 a);
	Matrix4x4 scale(const float x,const float y,const float z);

	float MtoRadian(float degree);
}

#endif // !__MY_MATH_H__
