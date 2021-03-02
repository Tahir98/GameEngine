#pragma once
#include "Camera.h"
#include "Core.h"
#include <iostream>

class PerspectiveCamera : public Camera {
private:
	float fov = 70;
	float aspectRatio;
	float near;
	float far;

public:
	PerspectiveCamera(const float fov,float aspectRatio,float near,float far) {
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		this->near = near;
		this->far = far;
		
		projection = GLMath::perspective(aspectRatio,fov,near,far);
		view = GLMath::lookat(position, position + direction, { 0,1,0 });
	}

	void update(float delta) override{
		Camera::update(delta);
		view = GLMath::lookat(position, position + direction, {0,1,0});
	}

	void setAspectRatio(float ratio) {
		if (ratio <= 0) {
			std::cout << "Given value is not acceptable" << std::endl;
			return;
		}

		aspectRatio = ratio;
		projection = GLMath::perspective(aspectRatio,fov,near,far);
	}

	void setFieldOfView(float degree) {
		if (degree < 5 || degree > 150) {
			std::cout << "Given value is not acceptable" << std::endl;
			return;
		}

		fov = degree;
		projection = GLMath::perspective(aspectRatio, fov, near, far);
	}

	void setFrustum(const float near, const float far) {
		if (near < 0.0f || far - near <= 0.0f || far <= 0.0) {
			std::cout << "Values are not in range" << std::endl;
			return;
		}
		
		this->near = near;
		this->far = far;
		
		projection = GLMath::perspective(aspectRatio,fov,near,far);
	}

};

