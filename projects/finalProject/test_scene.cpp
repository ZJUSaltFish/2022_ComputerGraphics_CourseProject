#include <iostream>
#include "test_scene.h"
#include "screenshot.h"

TestScene::TestScene(const Options& options): Application(options) {
	//_________________________________________BASIC_SCENE_____________________________________________
	_sceneList.push_back( new Scene());//default scene
	useScene(0);

	std::string _path = "./media/baseScene2.obj";
	_modelList.push_back(new Model(_path));
	loadModelToScene(0, 0);
	//load earth mat
	Texture2D* _textureBuf;
	unsigned int _textureIDBuf;
	unsigned int _matIDBuf;
	unsigned int _numOfObjs = _sceneList[0]->getNumOfObjects();

	_sceneList[0]->exportMesh(0);

	_textureBuf = new Texture2D("./media/earthmap.jpg");
	_textureIDBuf = _sceneList[_currentScene]->addTexture2D(_textureBuf);
	_matIDBuf = _sceneList[_currentScene]->newMaterial(
		glm::vec3(0.7,0.75, 1.0),//diffuse
		glm::vec3(0.0, 0.0, 0.0),//specular
		1.0f,//gloss
		false,//not transparent
		0.7,//textureBlend = 1.0 : always use texture as diffuse
		_textureIDBuf//use texture
	);
		_sceneList[_currentScene]->setObjectMaterial(0, _matIDBuf);
	//load Quom mat
	_textureBuf = new Texture2D("./media/planet_Quom1200.png");
	_textureIDBuf = _sceneList[_currentScene]->addTexture2D(_textureBuf);
	_matIDBuf = _sceneList[_currentScene]->newMaterial(
		glm::vec3(0.5, 0.5, 0.5),//diffuse
		glm::vec3(0.0, 0.0, 0.0),//specular
		1.0f,//gloss
		false,//not transparent
		1.0,//textureBlend = 1.0 : always use texture as diffuse
		_textureIDBuf//use texture
	);

	for (unsigned int i = 1; i < _numOfObjs; i++) {
		_sceneList[_currentScene]->setObjectMaterial(i, _matIDBuf);
	}

	//________________________________________________GEOMETRIES________________________________________________
	GeoBox _box(4.0f);
	unsigned int _IDBuf = loadGeometryToScene(&_box, _currentScene);
	_sceneList[_currentScene]->setObjectRotation(_IDBuf, glm::vec3(1, 1, 1), 45);

	GeoFrustum _frustum(8, 3.0f, 2.0f, 3.0f);
	_IDBuf = loadGeometryToScene(&_frustum, _currentScene);
	_sceneList[_currentScene]->setObjectPosition(_IDBuf, glm::vec3(0, 0, 10));
	
	GeoColumn _column(50, 4.0f, 2.0f);
	_IDBuf = loadGeometryToScene(&_column, _currentScene);
	_sceneList[_currentScene]->setObjectPosition(_IDBuf, glm::vec3(10, 0, 0));

	GeoCylinder _cylinder(50, 4.0f, 2.0f);
	_IDBuf = loadGeometryToScene(&_cylinder, _currentScene);
	_sceneList[_currentScene]->setObjectPosition(_IDBuf, glm::vec3(-10, 0, 0));

	GeoCone _cone(18, 3.0f, 3.0f);
	_IDBuf = loadGeometryToScene(&_cone, _currentScene);
	_sceneList[_currentScene]->setObjectPosition(_IDBuf, glm::vec3(0, 0, -10));
	//_________________________________________________CHARACTER_________________________________________________
	//craeate a new character
		//character material
	_textureBuf = new Texture2D("./media/animation/fish.png");
	_textureIDBuf = _sceneList[_currentScene]->addTexture2D(_textureBuf);
	_matIDBuf = _sceneList[_currentScene]->newMaterial(
		glm::vec3(0.5, 0.5, 0.5),//diffuse
		glm::vec3(0.0, 0.0, 0.0),//specular
		1.0f,//gloss
		false,//not transparent
		1.0,//textureBlend = 1.0 : always use texture as diffuse
		_textureIDBuf//use texture
	);
	//_sceneList[_currentScene]->setMaterialTexture(_matIDBuf, _textureIDBuf);
		//new character
	unsigned int _targetCha = _sceneList[_currentScene]->newCharacter(
		"fish",//name
		0,//cameraID
		"./media/animation/",//path
		20,//fps
		10,//frames
		_matIDBuf,//matID
		15.0f,//front offset
		-7.0f,//y offset
		0.35f//scale
	);

	unsigned int _targetLight = _sceneList[_currentScene]->newSpotLight(
		glm::vec3(0.5, 1.0, 0.5),//color
		1.0f,//intensity
		glm::vec3(0,0,1),//direction No Matter What You Fill In, it will be bind to the character and = character->getFront()
		30.0f,//angle in degree
		1.0f//attenuation
	);
	_sceneList[_currentScene]->characterBindLight(_targetCha, _targetLight, 20.0f, -4.0f);

	//other spot lights
	_targetLight = _sceneList[_currentScene]->newSpotLight(
		glm::vec3(0.5, 0.5, 1.0),//color
		1.0f,//intensity
		glm::vec3(0, -1, 0.01),//direction .can't be parrallel with y axis.
		30.0f,//angle in degree
		0.0f//attenuation
	);
	_sceneList[_currentScene]->setSpotLightPosition(_targetLight ,glm::vec3(0, 20, 0));

	//new cameras
	unsigned int _cameraBuf;
	_cameraBuf = _sceneList[_currentScene]->addCamera(16.0/9.0f, 0.01, -1000, 50, _windowHeight);
	_sceneList[_currentScene]->setCameraPosition(_cameraBuf, glm::vec3(100, 0, 0));
	_sceneList[_currentScene]->setCameraRotation(_cameraBuf, glm::vec3(0, 1, 0), -90);
	
	//initialize imgui
	imguiSettings.initImgui(_window);
	/*
	int imgH = 512;
	int imgW = 512;
	std::string name = "test";
	std::string path = "./media/";
	char* colorBuf = new char[imgW*imgH*3];
	for (int y = 0; y < imgH * imgW * 3; y++) {
		colorBuf[y] = 0;
	}
	exportBmp(path, name, colorBuf, imgW, imgH);*/
}

TestScene::~TestScene() {
	

	
}

void TestScene::handleInput() {
	constexpr float cameraMoveSpeed = 20.0f;
	constexpr float cameraRotateSpeed = 0.005f;
	//exit
	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		glfwSetWindowShouldClose(_window, true);
		return;
	}
	//some useful functions
	static float _storedx;
	static float _storedy;
	if (_keyboardInput.keyStates[GLFW_KEY_LEFT_ALT] == GLFW_RELEASE) {// hold LEFT_ALT to disable cusor(So you can move camera freely)
		//std::cout << "cursor disabled" << std::endl;
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_X] != GLFW_RELEASE) {//press X to do screeeeeeeenshot
		unsigned int _size = 512 * 512;
		RGBColor* _frame = new RGBColor[_size];
		char* _tmp = new char[_size * 3*sizeof(char)];
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glReadPixels(0, 0, 512, 512, GL_BGR, GL_UNSIGNED_BYTE, _tmp);
		for (int i = 0; i < _size; i++) {
			_frame[i].R = _tmp[3 * i];
			_frame[i].G = _tmp[3 * i + 1];
			_frame[i].B = _tmp[3 * i + 2];
		}
		std::string _shPath, _shName;
		_shPath = "./media/screenshot/"; _shName = "test";
		exportBmp(_shPath, _shName, _tmp, 512, 512);
		std::cout << "screenshot has been saved at["<<_shPath<<_shName<<".bmp]" << std::endl;
		_keyboardInput.keyStates[GLFW_KEY_X] = GLFW_RELEASE;
	}

	//camera operations
	if (_keyboardInput.keyStates[GLFW_KEY_C] == GLFW_PRESS) {
		//std::cout << "switch camera" << std::endl;
		// switch camera
		_sceneList[_currentScene]->switchCameraMode();
		_keyboardInput.keyStates[GLFW_KEY_C] = GLFW_RELEASE;
		return;
	}
	
	if (_keyboardInput.keyStates[GLFW_KEY_F] != GLFW_RELEASE) {
		//std::cout << "Focus" << std::endl;
		_sceneList[_currentScene]->cameraReFocus();//Press F to foucus camera on -z axis( Help you find the object!)
		_keyboardInput.keyStates[GLFW_KEY_SPACE] = GLFW_RELEASE;
		return;
	}

	if (1) {//Zoom: Scroll Up: IN; Scroll Down: OUT
		static float _scrollYOld;
		_sceneList[_currentScene]->zoom(_mouseInput.scroll.y - _scrollYOld);
		_scrollYOld = _mouseInput.scroll.y;
	}

	//move
	if ( _keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
		//std::cout << "W" << std::endl;
		_sceneList[_currentScene]->fpsMove(MY_FPS_FRONT, cameraMoveSpeed * _deltaTime);
	}

	if ( _keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		//std::cout << "A" << std::endl;
		_sceneList[_currentScene]->fpsMove(MY_FPS_RIGHT, - cameraMoveSpeed * _deltaTime);
	}

	if ( _keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		//std::cout << "S" << std::endl;
		_sceneList[_currentScene]->fpsMove(MY_FPS_FRONT, - cameraMoveSpeed * _deltaTime);
	}

	if ( _keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		//std::cout << "D" << std::endl;
		_sceneList[_currentScene]->fpsMove(MY_FPS_RIGHT, cameraMoveSpeed * _deltaTime);
	}

	if ( _keyboardInput.keyStates[GLFW_KEY_SPACE] != GLFW_RELEASE) {
		//std::cout << "SPACE" << std::endl;
		_sceneList[_currentScene]->fpsMove(MY_FPS_UP, cameraMoveSpeed * _deltaTime);
	}

	if ( _keyboardInput.keyStates[GLFW_KEY_LEFT_SHIFT] != GLFW_RELEASE) {
		//std::cout << "SHIFT" << std::endl;
		_sceneList[_currentScene]->fpsMove(MY_FPS_UP, - cameraMoveSpeed * _deltaTime);
	}
	//turn
	if ( _mouseInput.move.xCurrent != _mouseInput.move.xOld ) {
		//std::cout << "mouse move in x direction" << _mouseInput.move.xCurrent <<std::endl;
		//rotate the camera around world up: glm::vec3(0.0f, 1.0f, 0.0f)
		double _mouseMoveX = _mouseInput.move.xCurrent - _mouseInput.move.xOld;
		float _deltaYaw = _mouseMoveX * cameraRotateSpeed / _deltaTime;

		_sceneList[_currentScene]->fpsTurn(MY_FPS_RIGHT, _deltaYaw);

		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	}
	
	if ( _mouseInput.move.yCurrent != _mouseInput.move.yOld ) {
		//std::cout << "mouse move in y direction" << std::endl;
		// rotate the camera around its local right
		double _mouseMoveY = _mouseInput.move.yCurrent - _mouseInput.move.yOld;
		float _deltaPitch = _mouseMoveY * cameraRotateSpeed / _deltaTime;

		_sceneList[_currentScene]->fpsTurn(MY_FPS_UP, _deltaPitch);

		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
	}

}

void TestScene::renderFrame() {
	showFpsInWindowTitle();

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	_sceneList[_currentScene]->drawScene();

	imguiSettings.useImgui();
	//shader
	if (imguiSettings._shadermode == 0) {
		_sceneList[_currentScene]->useShader(MY_SHADER_DEFAULT);
	}
	else if (imguiSettings._shadermode == 1) {
		_sceneList[_currentScene]->useShader(MY_SHADER_SHADOWING);
	}

	unsigned int _targetCha = _sceneList[_currentScene]->getCurrentCharacter();
	unsigned int _targetMat = _sceneList[_currentScene]->getCharacterMaterial(_targetCha);
	SpotLight* _targetLight = _sceneList[_currentScene]->getCharacterLight(_targetCha);
	//material
	_sceneList[_currentScene]->setMaterialColor(_targetMat, MY_MAT_DIFFUSE, glm::vec3(imguiSettings._diffColor[0], imguiSettings._diffColor[1], imguiSettings._diffColor[2]));
	_sceneList[_currentScene]->setMaterialColor(_targetMat, MY_MAT_SPECULAR, glm::vec3(imguiSettings._specular[0], imguiSettings._specular[1], imguiSettings._specular[2]));
	_sceneList[_currentScene]->setMaterialFloat(_targetMat, MY_MAT_GLOSS, imguiSettings._gloss);
	_sceneList[_currentScene]->setMaterialFloat(_targetMat, MY_MAT_BLEND, imguiSettings._blend);
	//light
	_targetLight->color = glm::vec3(imguiSettings._spotLightColor[0], imguiSettings._spotLightColor[1], imguiSettings._spotLightColor[2]);
	_targetLight->intensity = imguiSettings._spotLightintensity;
	_targetLight->_radian = glm::radians(imguiSettings._spotLightdegree);
	_targetLight->_attenuation = imguiSettings._spotLightattenuation;
	//camera
	if (imguiSettings._useNextCamera) {
		_sceneList[_currentScene]->useCamera(MY_CAMERA_PERSP,
			(_sceneList[_currentScene]->getCurrentCamera() + 1) % _sceneList[_currentScene]->getNumOfCameras());
	}
	if (imguiSettings._exportMesh) {
		_sceneList[_currentScene]->exportMesh(0);
	}
}

void TestScene::useScene(unsigned int _sceneID)
{
	_currentScene = _sceneID;
	//this would tell the scene the size of viewport, for rendering. I should use size of viewport, but it seems they are same as the window size in application.cpp
	_sceneList[_currentScene]->updateViewportSize(_windowWidth, _windowHeight);
}

std::vector<unsigned int> TestScene::loadModelToScene(unsigned int _modelID, unsigned int _sceneID)
{
	std::vector<unsigned int> _objIDList;
	for (int i = 0; i < (_modelList[_modelID]->getMeshes()).size(); i++)
		_objIDList.push_back(_sceneList[_sceneID]->addPhysicalObject((_modelList[_modelID]->getMeshes())[i], 0));
	return _objIDList;
}

unsigned int TestScene::loadGeometryToScene(BasicGeometry* _geometry, unsigned int _sceneID)
{
	return _sceneList[_sceneID]->addPhysicalObject(_geometry->getMesh(), 0);
}