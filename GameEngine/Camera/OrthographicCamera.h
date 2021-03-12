#pragma once
#include "Camera.h"
#include "Core.h"
#include <iostream>

class OrthographicCamera : public Camera {
private:
	float left, right;
	float bottom, top;
	float near, far;

public:
	OrthographicCamera(const float left, const float right, const float bottom, const float top, const float near, const float far) {
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
		this->near = near;
		this->far = far;

		projection = GLMath::ortho(left,right,bottom,top,near,far);
	}

	void update(float delta) override {
		Camera::update(delta);
		view = GLMath::lookat(position, position + direction, { 0,1,0 });
	}
	
};

