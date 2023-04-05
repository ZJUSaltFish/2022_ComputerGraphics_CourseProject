#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//放在设置glfw库或glad库后
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../base/application.h"

class ImguiSettings
{
public:
	ImguiSettings();

	void initImgui(GLFWwindow* _window);
	void useImgui();

	float _conecolor[3] = { 0.5, 0.5, 0.5 };//调用imgui返回值设置颜色
	//shader
	int _shadermode = 0;
	int _show;

	//camera
	bool _useNextCamera;

	//material parameter
	float _diffColor[3] = { 0.5, 0.5, 0.5 };
	float _blend = 0.5;
	float _specular[3] = { 0.5f, 0.5f, 0.5f };
	float _gloss = 1.0f;

	//spot light
	float _spotLightintensity = 1.0;
	float _spotLightdegree = 1.0;
	float _spotLightColor[3] = {1.0f, 1.0f, 1.0f};
	float _spotLightattenuation = 1;
	bool _check;

	//mesh
	bool _exportMesh;
private:
	
};







