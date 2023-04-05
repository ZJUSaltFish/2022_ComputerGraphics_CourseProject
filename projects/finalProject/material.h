#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class  Material
{
public:
	Material();
	Material(glm::vec3 _diffuse, glm::vec3 _specular, float _gloss, bool _ifTranseparent, float _textureBlend, unsigned int _textureID);
	~Material();
	glm::vec3 _diffuceColor;
	glm::vec3 _specular;//RGB -> HSV. V = intensity
	float _glossness;
	bool _opacityMode;
	float _textureBlend;
	unsigned int _textureID;
	//glm::vec3 getHSV(glm::vec3 _RGB);
private:
	
};




