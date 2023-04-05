#include "po_camera.h"

glm::mat4 POCamera::getViewMatrix(){
	return glm::lookAt(position, position + getFront(), getUp());
}


POCamera::POCamera(float _whRatio, float _zNear, float _zFar, float _fovDegree, float _windowHeight){

	fovy = glm::radians(_fovDegree);
	aspect = _whRatio;
	znear = _zNear;
	zfar = _zFar;
	left = -0.5f * _windowHeight * _whRatio;
	right = 0.5f * _windowHeight * _whRatio;
	bottom = -0.5f * _windowHeight;
	top = 0.5f * _windowHeight;
}


glm::mat4 POCamera::getProjectionMatrixPersp(){
	return glm::perspective(fovy, aspect, znear, zfar);
}


glm::mat4 POCamera::getProjectionMatrixOrtho(){
	return glm::ortho(left, right, bottom, top, znear, zfar);
}

Frustum POCamera::getFrustumPersp() {
	Frustum frustum;
	// TODO: construct the frustum with the position and orientation of the camera
	// Note: this is for Bonus project 'Frustum Culling'
	// write your code here
	// ----------------------------------------------------------------------
	// ...
	// ----------------------------------------------------------------------

	return frustum;
}


Frustum POCamera::getFrustumOrtho(){
	Frustum frustum;
	const glm::vec3 fv = getFront();
	const glm::vec3 rv = getRight();
	const glm::vec3 uv = getUp();

	// all of the plane normal points inside the frustum, maybe it's a convention
	frustum.planes[Frustum::NearFace] = { position + znear * fv, fv };
	frustum.planes[Frustum::FarFace] = { position + zfar * fv, -fv };
	frustum.planes[Frustum::LeftFace] = { position - right * rv , rv };
	frustum.planes[Frustum::RightFace] = { position + right * rv , -rv };
	frustum.planes[Frustum::BottomFace] = { position - bottom * uv , uv };
	frustum.planes[Frustum::TopFace] = { position + top * uv , -uv };

	return frustum;
}