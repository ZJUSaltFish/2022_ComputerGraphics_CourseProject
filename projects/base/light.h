#pragma once
#include "object3d.h"
class Light : public Object3D {
public:
	Light() = default;

	~Light() = default;

public:
	float intensity = 1.0f;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

class AmbientLight : public Light {
public:
	AmbientLight() = default;

	~AmbientLight() = default;
};

class DirectionalLight : public Light{
public:
	DirectionalLight() = default;

	~DirectionalLight() = default;
};

class SpotLight : public Light {
public:
	SpotLight() = default;

	~SpotLight() = default;

public:
	float _radian = glm::radians(60.0f);
	float _attenuation = 1.0f;
};