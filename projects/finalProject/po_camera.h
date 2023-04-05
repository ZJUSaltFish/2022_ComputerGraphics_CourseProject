#pragma once
#pragma once

#include "../base/object3d.h"
#include "../base/frustum.h"

class POCamera : public Object3D {
public:
	POCamera(float _whRatio, float _zNear, float _zFar, float _fovDegree, float _windowHeight);

	glm::mat4 getViewMatrix();

	glm::mat4 getProjectionMatrixPersp();//perspective camera
	glm::mat4 getProjectionMatrixOrtho();//orthographic camera

	Frustum getFrustumPersp();
	Frustum getFrustumOrtho();

	float fovy;// in radian
	float aspect;// = width / height
	float znear;
	float zfar;

	float left;
	float right;
	float bottom;
	float top;

};
