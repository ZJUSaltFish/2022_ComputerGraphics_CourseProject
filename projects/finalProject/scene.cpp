#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../base/glsl_program.h"
#include "scene.h"

const std::vector<std::string> skyboxTexturePaths = {
	"./media/starfield/Right_Tex.jpg",
	"./media/starfield/Left_Tex.jpg",
	"./media/starfield/Up_Tex.jpg",
	"./media/starfield/Down_Tex.jpg",
	"./media/starfield/Front_Tex.jpg",
	"./media/starfield/Back_Tex.jpg"
};

Scene::Scene()
{
	_skyBox.reset(new SkyBox(skyboxTexturePaths));
	//defaul texture
	Texture2D* _defaultTexture = new Texture2D("./media/diffuse1.jpg");
	addTexture2D(_defaultTexture);
	
	//default material
	Material* _defaultMat = new Material();
	this->_materialList.push_back(_defaultMat);
	_materialList[0]->_textureID = 0;
	_materialList[0]->_textureBlend = 1.0f;

	//default lights
	newAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);
	newDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.6f, glm::vec3(0, -1, 1));

	//default camera
	unsigned int  _defaultCamera = addCamera(16 / 9.0f, 0.1f, 10000.0f, 50.0f, 40.0f);//set it by convinence
	setCameraPosition(_defaultCamera, glm::vec3(0, 0, 10));
	useCamera(MY_CAMERA_PERSP, _defaultCamera);

	//construct shaders
	constructDefaultShader();
	_numOfShaders = 0;

	constructDepthShader();
	constructShadowingShader();
	_numOfShaders ++;

	//use default shader
	useShader(MY_SHADER_DEFAULT);

}

Scene::~Scene()
{
}

//___________________________________MESH_AND_OBJECT________________________________________
unsigned int Scene::addPhysicalObject(Mesh* _meshPtr, unsigned int _matID)
{
	this->_meshList.push_back(_meshPtr);
	unsigned int _meshID = _meshList.size() - 1;
	PhysicalObject* _newObject = new PhysicalObject(_meshID, _matID);
	_objectList.push_back(_newObject);
	return _objectList.size() - 1;
}


void Scene::exportMesh(unsigned int _meshID)
{
	_meshList[_meshID]->exportMesh("./media/", _meshList[_meshID]->getName());
}

unsigned int Scene::getNumOfObjects(void)
{
	return _objectList.size();
}

void Scene::setObjectMaterial(unsigned int _objID, unsigned int _matID)
{
	_objectList[_objID]->setMaterialID(_matID);
}

void Scene::setObjectPosition(unsigned int _objID, glm::vec3 _positionVec)
{
	_objectList[_objID]->position = _positionVec;
}

void Scene::setObjectRotation(unsigned int _objID, glm::vec3 _axis, float _value)
{
	_objectList[_objID]->rotation = glm::quat(glm::cos(glm::radians(_value) / 2.0f),
		glm::sin(glm::radians(_value) / 2.0f) * glm::normalize(_axis)) * glm::quat(1,0,0,0);
}

void Scene::setObjectScale(unsigned int _objID, glm::vec3 _scale)
{
	_objectList[_objID]->scale = _scale;
}

void Scene::moveObject(unsigned int _objID, glm::vec3 _deltaPosition)
{
	_objectList[_objID]->position += _deltaPosition;
}

void Scene::rotateObject(unsigned int _objID, glm::vec3 _axis, float _angle)
{
	_objectList[_objID]->rotation = glm::quat(glm::cos(glm::radians(_angle) / 2.0f),
		glm::sin(glm::radians(_angle) / 2.0f) * glm::normalize(_axis)) * _objectList[_objID]->rotation;
}

void Scene::scaleObject(unsigned int _objID, glm::vec3 _deltaScale)
{
	_objectList[_objID]->scale += _deltaScale;
}
//____________________________________________________________________________________________

//____________________________________ABOUT_CHARACTER_________________________________________
unsigned int Scene::newCharacter(std::string _name, unsigned int _cameraID, std::string _animPath, unsigned int _fps, unsigned int _frames, unsigned int _materialID, float _frontOffset, float _yOffset, float _scale)
{
	POCamera* _camera = _cameraList[_cameraID];
	Character* _newCharacter;
	_newCharacter = new Character(_name, _camera, _animPath, _fps, _frames, _materialID, _frontOffset, _yOffset, _scale);
	_characterList.push_back(_newCharacter);
	_currentCharacter = _characterList.size() - 1;
	return _currentCharacter;
 }

void Scene::useCharacter(unsigned int _characterID) 
{
	if (_characterID < _characterList.size())
	{
		_currentCharacter = _characterID;
	}
}

unsigned int Scene::getCurrentCharacter(void)
{
	return _currentCharacter;
}
 
unsigned int Scene::getCharacterMaterial(unsigned int _characterID)
{
	return _characterList[_characterID]->getMaterialID();
}

void Scene::setCharacterMaterial(unsigned int _characterID, unsigned int _materialID)
{
	_characterList[_characterID]->setMaterial(_materialID);
}

void Scene::characterBindLight(unsigned int _characterID, unsigned int _spotLightID, float _frontOffset, float _yOffset)
{
	_characterList[_characterID]->bindSoptLight(_spotLightList[_spotLightID], _frontOffset, _yOffset);
}

SpotLight* Scene::getCharacterLight(unsigned int _characterID)
{
	return _characterList[_characterID]->getLight();
}
//_____________________________________________________________________________________________

//____________________________________MATERIAL_AND_TEXTURE______________________________________

unsigned int Scene::addTexture2D(Texture2D* _texture)
{
	_texture2DList.push_back(_texture);
	return _texture2DList.size() - 1;
}

unsigned int Scene::newMaterial(glm::vec3 _diffuse, glm::vec3 _specular, float _gloss, bool _ifTranseparent, float _textureBlend, unsigned int _textureID)
{
	Material* _newMat = new Material(_diffuse, _specular, _gloss, _ifTranseparent, _textureBlend, _textureID);
	_materialList.push_back(_newMat);
	return _materialList.size() - 1;
}

void Scene::setMaterialTexture(unsigned int _matID, unsigned int _texID)
{
	_materialList[_matID]->_textureID = _texID;
}

void Scene::setMaterialFloat(unsigned int _matID, unsigned int _mode, float _value)
{
	if (_matID >= _materialList.size()) {
		std::cout << "WARNING: setting non-exist material [" << _matID << "]" << std::endl;
		return;
	}
	if (_mode == MY_MAT_GLOSS) {
		_materialList[_matID]->_glossness = _value;
	}else if (_mode == MY_MAT_BLEND){
		_materialList[_matID]->_textureBlend = _value;
	}else {
		std::cout << "WARNING: setting material by non-exist parameter [" << _mode << "]" << std::endl;
	}
	
}

void Scene::setMaterialColor(unsigned int _matID, unsigned int _mode, glm::vec3 _color)
{
	if (_matID >= _materialList.size()) {
		std::cout << "WARNING: setting non-exist material [" << _matID << "]" << std::endl;
		return;
	}
	if (_mode == MY_MAT_DIFFUSE) {
		_materialList[_matID]->_diffuceColor = _color;
	}
	else if (_mode == MY_MAT_SPECULAR) {
		_materialList[_matID]->_specular = _color;
	}
	else {
		std::cout << "WARNING: setting material by non-exist parameter [" << _mode << "]" << std::endl;
	}

}
//______________________________________________________________________________________

//__________________________________ABOUT_LIGHT_________________________________________
unsigned int Scene::newAmbientLight(glm::vec3 _color, float _intensity)
{
	AmbientLight* _newAmbientLight = new AmbientLight;
	_newAmbientLight->color = _color;
	_newAmbientLight->intensity = _intensity;
	_ambientLightList.push_back(_newAmbientLight);
	return _ambientLightList.size() - 1;
}

unsigned int Scene::newDirectionalLight(glm::vec3 _color, float _intensity, glm::vec3 _direction)
{
	//generate light
	DirectionalLight* _newDirLight = new DirectionalLight;
	_newDirLight->color = _color;
	_newDirLight->intensity = _intensity;
	_newDirLight->rotation = glm::quatLookAt(glm::normalize(_direction), glm::vec3(0.0f, 1.0f, 0.0f));
	_directionalLightList.push_back(_newDirLight);
	//generate depthmap(for shadowing)
	DataTexture*  _depthMap = new DataTexture(GL_DEPTH_COMPONENT, MY_SHADOW_SIZE, MY_SHADOW_SIZE,GL_DEPTH_COMPONENT, GL_FLOAT);
	_dirDepthMapList.push_back(_depthMap);
	return _directionalLightList.size() - 1;
}

unsigned int Scene::newSpotLight(glm::vec3 _color, float _intensity, glm::vec3 _direction, float _degree, float _attenuation)
{
	SpotLight* _newSpotLight = new SpotLight;
	_newSpotLight->color = _color;
	_newSpotLight->intensity = _intensity;
	_newSpotLight->rotation = glm::quatLookAt(glm::normalize(_direction), glm::vec3(0.0f, 1.0f, 0.0f));
	_newSpotLight->_radian = glm::radians(_degree);
	_newSpotLight->_attenuation = _attenuation;
	_spotLightList.push_back(_newSpotLight);

	//generate depthmap(for shadowing)
	DataTexture* _depthMap = new DataTexture(GL_DEPTH_COMPONENT, MY_SHADOW_SIZE, MY_SHADOW_SIZE, GL_DEPTH_COMPONENT, GL_FLOAT);
	_spotDepthMapList.push_back(_depthMap);
	return _spotLightList.size() - 1;
}

void Scene::setLightFloat(unsigned int _lightType, unsigned int _lightID, unsigned int _attribName, float _value)
{
	if (_lightType == MY_LIGHT_AMBIENT) {
		if (_lightID >= _ambientLightList.size()) {
			std::cout << "WARNING: setting non-exist light ID [" << _lightID<< "]" << std::endl;
			return;
		}
		if (_attribName == MY_LIGHT_INTENSITY) {
			_ambientLightList[_lightID]->intensity = _value;
		}else{
			std::cout << "WARNING: setting non-exist attribute [" << _attribName << "]" << std::endl;
		}

	}else if (_lightType == MY_LIGHT_DIRECTIONAL) {
		if (_lightID >= _directionalLightList.size()) {
			std::cout << "WARNING: setting non-exist light ID [" << _lightID << "]" << std::endl;
			return;
		}
		if (_attribName == MY_LIGHT_INTENSITY) {
			_directionalLightList[_lightID]->intensity = _value;
		}else {
			std::cout << "WARNING: setting non-exist attribute [" << _attribName << "]" << std::endl;
		}

	}else if (_lightType == MY_LIGHT_SPOT) {
		if (_lightID >= _spotLightList.size()) {
			std::cout << "WARNING: setting non-exist light ID [" << _lightID << "]" << std::endl;
			return;
		}
		if (_attribName == MY_LIGHT_INTENSITY) {
			_spotLightList[_lightID]->intensity = _value;
		}else if (_attribName == MY_LIGHT_ANGLE) {
			_spotLightList[_lightID]->_radian = glm::radians(_value);
		}else if (_attribName == MY_LIGHT_ATTENUATION) {
			_spotLightList[_lightID]->_attenuation = _value;
		}else {
			std::cout << "WARNING: setting non-exist attribute [" << _attribName << "]" << std::endl;
		}
	}else {
		std::cout << "WARNING: setting non-exist light type [" << _lightType << "]" << std::endl;
	}
}
void Scene::setLightColor(unsigned int _lightType, unsigned int _lightID, glm::vec3 _value)
{
	if (_lightType == MY_LIGHT_AMBIENT) {
		if (_lightID >= _ambientLightList.size()) {
			std::cout << "WARNING: setting non-exist light ID [" << _lightID << "]" << std::endl;
			return;
		}
		_ambientLightList[_lightID]->color = _value;
	}
	else if (_lightType == MY_LIGHT_DIRECTIONAL) {
		if (_lightID >= _directionalLightList.size()) {
			std::cout << "WARNING: setting non-exist light ID [" << _lightID << "]" << std::endl;
			return;
		}
		_directionalLightList[_lightID]->color = _value;
	}
	else if (_lightType == MY_LIGHT_SPOT) {
		if (_lightID >= _spotLightList.size()) {
			std::cout << "WARNING: setting non-exist light ID [" << _lightID << "]" << std::endl;
			return;
		}
		_spotLightList[_lightID]->color = _value;
	}
	else {
		std::cout << "WARNING: setting non-exist light type [" << _lightType << "]" << std::endl;
	}
}
void Scene::setSpotLightPosition(unsigned int _spotLightID, glm::vec3 _positionVec)
{
	_spotLightList[_spotLightID]->position = _positionVec;
}
//_______________________________________________________________________________________ 

//__________________________________ABOUT_CAMERA________________________________________
void Scene::useCamera(unsigned int _mode, unsigned int _cameraID)
{
	if (_mode == MY_CAMERA_PERSP) {
		_currentCamera = _cameraID;
		_currentCameraMode = MY_CAMERA_PERSP;
	}
	else if (_mode == MY_CAMERA_ORTHO) {
		_currentCamera = _cameraID;
		_currentCameraMode = MY_CAMERA_ORTHO;
	}
	else {
		std::cout << "ERROR: using invaild camera mode" << std::endl;
	}
}

void Scene::switchCameraMode()
{
	if (_currentCameraMode == MY_CAMERA_PERSP)_currentCameraMode = MY_CAMERA_ORTHO;
	else _currentCameraMode = MY_CAMERA_PERSP;
}

void Scene::setCameraPosition(unsigned int _cameraID, glm::vec3 _position)
{
	_cameraList[_cameraID]->position = _position;
}
void Scene::setCameraRotationQuat(unsigned int _cameraID, glm::quat _rotation)
{
	_cameraList[_cameraID]->rotation = _rotation;
}
void Scene::setCameraRotation(unsigned int _cameraID, glm::vec3 _axis, float _angle)
{
	_cameraList[_cameraID]->rotation = glm::quat(glm::cos(glm::radians(_angle) / 2.0f),
		glm::sin(glm::radians(_angle) / 2.0f) * glm::normalize(_axis)) * glm::quat(1, 0, 0, 0);
}
void Scene::setCameraScale(unsigned int _cameraID, glm::vec3 _scale)
{
	_cameraList[_cameraID]->scale = _scale;
}

void Scene::fpsMove(unsigned int _direction, float _value)
{
	POCamera* _current = _cameraList[_currentCamera];
	if (_direction == MY_FPS_FRONT) {
		_current->position += _current->getFront() * _value;
	}
	else if (_direction == MY_FPS_RIGHT) {
		_current->position += _current->getRight() * _value;
	}
	else if (_direction == MY_FPS_UP) {
		_current->position += glm::vec3(0.0f, 1.0f, 0.0f) * _value;
	}
	else {
		std::cout << "WARNING: invalid moving direction" << std::endl;
	}
}
void Scene::fpsTurn(unsigned int _direction, float _value)
{
	POCamera* _current = _cameraList[_currentCamera];
	if (_direction == MY_FPS_RIGHT) {
		_current->rotation = glm::quat(glm::cos(glm::radians(-_value / 2.0f)),
			glm::sin(glm::radians(-_value) / 2.0f) * glm::vec3(0.0f, 1.0f, 0.0f)) * _current->rotation;
	}
	else if(_direction == MY_FPS_UP) {
		if(-0.9 < _current->getFront().y && _value > 0 || _current->getFront().y< 0.9 && _value < 0)
		_current->rotation = glm::quat(glm::cos(glm::radians(- _value / 2.0f)),
			glm::sin(glm::radians(- _value) / 2.0f) * glm::normalize(_current->getRight())) * _current->rotation;
	}
	else {
		std::cout << "WARNING: invalid turning direction" << std::endl;
	}
}

void Scene::zoom(float _value)
{
	if (0.1 < _cameraList[_currentCamera]->fovy && _value > 0 || _cameraList[_currentCamera]->fovy < glm::radians(175.0f) && _value < 0) {
		_cameraList[_currentCamera]->fovy -= _value * 0.05;
	}
	//std::cout << _cameraList[_currentCamera]->rotation.w << std::endl;
}

void Scene::cameraReFocus()
{
	_cameraList[_currentCamera]->fovy = glm::radians(50.0f);
	_cameraList[_currentCamera] -> rotation = glm::quat(1.0f, 0, 0, 0);
	//std::cout << _cameraList[_currentCamera]->getFront().z << std::endl;
}

unsigned int Scene::addCamera(float _whRatio, float _zNear, float _zFar, float _fovDegree, float _windowHeight)
{
	_cameraList.push_back(new POCamera( _whRatio, _zNear, _zFar, _fovDegree, _windowHeight));
	return _cameraList.size() - 1;
}

unsigned int Scene::getCurrentCamera(void)
{
	return _currentCamera;
}

unsigned int Scene::getNumOfCameras(void)
{
	return _cameraList.size();
}
//______________________________________________________________________________________

//________________________________________ABOUT_DRAWING__________________________________

void Scene::updateViewportSize(unsigned int _width, unsigned int _height) 
{
	_viewWidth = _width; _viewHeight = _height;
}

void Scene::drawObject(PhysicalObject* _object)
{
	if (_currentShader == MY_SHADER_DEFAULT) {
		_defaultShader->setMat4("model", _object->getModelMatrix());

		Mesh* _mesh = _meshList[_object->getMeshID()];
		Material* _material = _materialList[_object->getMaterialID()];
		//material
		_defaultShader->setVec3("material.diffuseColor", _material->_diffuceColor);
		_defaultShader->setVec3("material.specular", _material->_specular);
		_defaultShader->setFloat("material.glossness", _material->_glossness);
		_defaultShader->setFloat("material.textureBlend", _material->_textureBlend);
		if (_material->_textureID != -1) {//if this material has texture
			glActiveTexture(GL_TEXTURE0);
			_texture2DList[_material->_textureID]->bind();
		}

		glBindVertexArray(_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEbo());
		glDrawElements(GL_TRIANGLES, _mesh->getIndexSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
	else if (_currentShader == MY_SHADER_DEPTHING) {
		_depthShader->setMat4("model", _object->getModelMatrix());

		Mesh* _mesh = _meshList[_object->getMeshID()];
		glBindVertexArray(_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEbo());
		glDrawElements(GL_TRIANGLES, _mesh->getIndexSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else if (_currentShader == MY_SHADER_SHADOWING) {
		_shadowingShader->setMat4("model", _object->getModelMatrix());

		Mesh* _mesh = _meshList[_object->getMeshID()];
		Material* _material = _materialList[_object->getMaterialID()];
		//material
		_shadowingShader->setVec3("material.diffuseColor", _material->_diffuceColor);
		_shadowingShader->setVec3("material.specular", _material->_specular);
		_shadowingShader->setFloat("material.glossness", _material->_glossness);
		_shadowingShader->setFloat("material.textureBlend", _material->_textureBlend);
		if (_material->_textureID != -1) {//if this material has texture
			glActiveTexture(GL_TEXTURE6);//1-5 has been used for depthmap
			_texture2DList[_material->_textureID]->bind();
		}

		glBindVertexArray(_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEbo());
		glDrawElements(GL_TRIANGLES, _mesh->getIndexSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

}

void Scene::drawCharacter(Character* _character)
{
	if (_currentShader == MY_SHADER_DEFAULT) {
		_defaultShader->setMat4("model", _character->getModelMatrix());

		Mesh* _mesh = _character->_currentFrame;
		Material* _material = _materialList[_character->getMaterialID()];
		//material
		_defaultShader->setVec3("material.diffuseColor", _material->_diffuceColor);
		_defaultShader->setVec3("material.specular", _material->_specular);
		_defaultShader->setFloat("material.glossness", _material->_glossness);
		_defaultShader->setFloat("material.textureBlend", _material->_textureBlend);
		if (_material->_textureID != -1) {//if this material has texture
			glActiveTexture(GL_TEXTURE0);
			_texture2DList[_material->_textureID]->bind();
		}

		glBindVertexArray(_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEbo());
		glDrawElements(GL_TRIANGLES, _mesh->getIndexSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
	else if (_currentShader == MY_SHADER_DEPTHING) {
		_depthShader->setMat4("model", _character->getModelMatrix());

		Mesh* _mesh = _character->_currentFrame;
		glBindVertexArray(_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEbo());
		glDrawElements(GL_TRIANGLES, _mesh->getIndexSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else if (_currentShader == MY_SHADER_SHADOWING) {
		_shadowingShader->setMat4("model", _character->getModelMatrix());

		Mesh* _mesh = _character->_currentFrame;
		Material* _material = _materialList[_character->getMaterialID()];
		//material
		_shadowingShader->setVec3("material.diffuseColor", _material->_diffuceColor);
		_shadowingShader->setVec3("material.specular", _material->_specular);
		_shadowingShader->setFloat("material.glossness", _material->_glossness);
		_shadowingShader->setFloat("material.textureBlend", _material->_textureBlend);
		if (_material->_textureID != -1) {//if this material has texture
			glActiveTexture(GL_TEXTURE6);//1-5 has been used for depthmap
			_texture2DList[_material->_textureID]->bind();
		}

		glBindVertexArray(_mesh->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->getVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEbo());
		glDrawElements(GL_TRIANGLES, _mesh->getIndexSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}//end if
}

void Scene::drawScene(void)
{
	if (_currentShader == MY_SHADER_DEFAULT)//when using default shader
	{
		for(int i = 0; i< _characterList.size(); i++)
			_characterList[i]->updateCharacter();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//shader
		_defaultShader->use();

		//camera
		if(_currentCameraMode == MY_CAMERA_PERSP)
			_defaultShader->setMat4("projection", _cameraList[_currentCamera]->getProjectionMatrixPersp());
		else if(_currentCameraMode == MY_CAMERA_ORTHO)
			_defaultShader->setMat4("projection", _cameraList[_currentCamera]->getProjectionMatrixOrtho());

		_defaultShader->setMat4("view", _cameraList[_currentCamera]->getViewMatrix());
		_defaultShader->setVec3("viewPos", _cameraList[_currentCamera]->position);
		//light
		_defaultShader->setVec3("ambientLight.color", _ambientLightList[0]->color);
		_defaultShader->setFloat("ambientLight.intensity", _ambientLightList[0]->intensity);

		_defaultShader->setVec3("directionalLight.direction", _directionalLightList[0]->getFront());
		_defaultShader->setVec3("directionalLight.color", _directionalLightList[0]->color);
		_defaultShader->setFloat("directionalLight.intensity", _directionalLightList[0]->intensity);

		_defaultShader->setInt("spotNumber", _spotLightList.size());
		for (int i = 0; i < _spotLightList.size() && i < 5; i++) {//the shader only support 5 spotlights( you can change this by changing codes)
			char _index[2] = { i };
			itoa(i, _index, 10);
			std::string _name = "spotLight[";
			_defaultShader->setVec3(_name + _index + "].direction", _spotLightList[i]->getFront());
			_defaultShader->setVec3(_name + _index + "].color", _spotLightList[i]->color);
			_defaultShader->setFloat(_name + _index + "].intensity", _spotLightList[i]->intensity);
			_defaultShader->setFloat(_name + _index + "].radian", _spotLightList[i]->_radian);
			_defaultShader->setFloat(_name + _index + "].attenuation", _spotLightList[i]->_attenuation);
			_defaultShader->setVec3(_name + _index + "].position", _spotLightList[i]->position);
		}


		for (int i = 0; i < _objectList.size(); i++){
			drawObject(_objectList[i]);
		}

		for (int i = 0; i < _characterList.size(); i++) {
			drawCharacter(_characterList[i]);
		}
	}
	else if (_currentShader == MY_SHADER_SHADOWING) {

		for (int i = 0; i < _characterList.size(); i++)
			_characterList[i]->updateCharacter();

		//generate depth map
		
		glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFbo);


		glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE);

		glViewport(0, 0, MY_SHADOW_SIZE, MY_SHADOW_SIZE);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		_depthShader->use();
		_currentShader = MY_SHADER_DEPTHING;

		//directional depth map
		glActiveTexture(GL_TEXTURE0);
		_dirDepthMapList[0]->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _dirDepthMapList[0]->getHandle(), 0);
		glm::mat4 _lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 10000.0f);
		glm::mat4 _lightView = glm::lookAt(_directionalLightList[0]->position, _directionalLightList[0]->position + _directionalLightList[0]->getFront(), _directionalLightList[0]->getUp());
		glm::mat4 _dirLightSpaceMatrix = _lightProjection * _lightView;
		_depthShader->setMat4("lightSpaceMatrix", _dirLightSpaceMatrix);

		for (int i = 0; i < _objectList.size(); i++) {
			drawObject(_objectList[i]);
		}

		for (int i = 0; i < _characterList.size(); i++) {
			drawCharacter(_characterList[i]);
		}
		//spot light depth map
		glm::mat4 _spotLightSpaceMatrix[5];
		for (int j = 0; j < _spotLightList.size() && j<5; j++) {
			glActiveTexture(GL_TEXTURE0);
			_spotDepthMapList[j]->bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _spotDepthMapList[j]->getHandle(), 0);
			glm::mat4 _lightProjection = glm::perspective(_spotLightList[j]->_radian, 1.0f, 0.01f, 1000.0f);
			glm::mat4 _lightView = glm::lookAt(_spotLightList[j]->position, _spotLightList[j]->position + _spotLightList[j]->getFront(), _spotLightList[j]->getUp());
			_spotLightSpaceMatrix[j] = _lightProjection * _lightView;
			_depthShader->setMat4("lightSpaceMatrix", _spotLightSpaceMatrix[j]);

			for (int i = 0; i < _objectList.size(); i++) {
				drawObject(_objectList[i]);
			}

			for (int i = 0; i < _characterList.size(); i++) {
				drawCharacter(_characterList[i]);
			}
		}
		

		//finish
		_currentShader = MY_SHADER_SHADOWING;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//shadowing render
		glDrawBuffer(GL_BACK); glReadBuffer(GL_BACK);

		glViewport(0, 0, _viewWidth, _viewHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		
		//shader
		_shadowingShader->use();
	
		
		//camera
		if (_currentCameraMode == MY_CAMERA_PERSP)
			_shadowingShader->setMat4("projection", _cameraList[_currentCamera]->getProjectionMatrixPersp());
		else if (_currentCameraMode == MY_CAMERA_ORTHO)
			_shadowingShader->setMat4("projection", _cameraList[_currentCamera]->getProjectionMatrixOrtho());

		_shadowingShader->setMat4("view", _cameraList[_currentCamera]->getViewMatrix());
		_shadowingShader->setVec3("viewPos", _cameraList[_currentCamera]->position);

		//light
		_shadowingShader->setVec3("ambientLight.color", _ambientLightList[0]->color);
		_shadowingShader->setFloat("ambientLight.intensity", _ambientLightList[0]->intensity);

		_shadowingShader->setVec3("directionalLight.direction", _directionalLightList[0]->getFront());
		_shadowingShader->setVec3("directionalLight.color", _directionalLightList[0]->color);
		_shadowingShader->setFloat("directionalLight.intensity", _directionalLightList[0]->intensity);
		glActiveTexture(GL_TEXTURE0);
		_dirDepthMapList[0]->bind();
		_shadowingShader->setMat4("dirLightSpaceMatrix", _dirLightSpaceMatrix);

		_shadowingShader->setInt("spotNumber", _spotLightList.size());
		for (int i = 0; i < _spotLightList.size() && i < 5; i++) {//the shader only support 5 spotlights( you can change this by changing codes)
			glActiveTexture(GL_TEXTURE0 + i + 1);
			_spotDepthMapList[i]->bind();

			char _index[2] = { i };
			itoa(i, _index, 10);
			std::string _name = "spotLight[";
			_shadowingShader->setVec3(_name + _index + "].direction", _spotLightList[i]->getFront());
			_shadowingShader->setVec3(_name + _index + "].color", _spotLightList[i]->color);
			_shadowingShader->setFloat(_name + _index + "].intensity", _spotLightList[i]->intensity);
			_shadowingShader->setFloat(_name + _index + "].radian", _spotLightList[i]->_radian);
			_shadowingShader->setFloat(_name + _index + "].attenuation", _spotLightList[i]->_attenuation);
			_shadowingShader->setVec3(_name + _index + "].position", _spotLightList[i]->position);
			_name = "spotLightSpaceMatrix[";
			_shadowingShader->setMat4(_name + _index + "]", _spotLightSpaceMatrix[i]);

		}


		for (int i = 0; i < _objectList.size(); i++) {
			drawObject(_objectList[i]);
		}

		for (int i = 0; i < _characterList.size(); i++) {
			drawCharacter(_characterList[i]);
		}
		
	}




	_skyBox->draw(_cameraList[_currentCamera]->getProjectionMatrixPersp(), _cameraList[_currentCamera]->getViewMatrix());

}


//_____________________________________________________________________________________

//________________________________________ABOUT_SHADER__________________________________

void Scene::useShader(unsigned int _shaderID)
{
	if (_shaderID > _numOfShaders) {
		std::cout << "filed to use shader" << _shaderID << std::endl;
		return;
	}
	_currentShader = _shaderID;
}

void Scene::constructDefaultShader(void)
{
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPosition;\n"
		"out vec3 fNormal;\n"
		"out vec2 fTexCoord;\n"

		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	fTexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"} \n";

	const char* fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"in vec2 fTexCoord;\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 diffuseColor;\n"
		"	vec3 specular;\n"
		"	float glossness;\n"
		"	float textureBlend;\n"
		"};\n"

		"struct AmbientLight {\n"
		"	vec3 color;\n"
		"	float intensity;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"struct SpotLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"	float radian;\n"
		"	float attenuation;\n"
		"	vec3 position;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform SpotLight spotLight[5];\n"
		"uniform AmbientLight ambientLight;\n"
		"uniform vec3 viewPos;\n"
		"uniform sampler2D diffuseTexture;\n"
		"uniform int spotNumber;\n"

		"vec3 blendedDiffuse;\n"
		"vec3 normal;\n"
		
		//Directional light diffuse
		"vec3 calcDirLightDiff() {\n"
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 diffuse = directionalLight.color * max(dot(lightDir, normal), 0.0f) * blendedDiffuse;\n"
		"	return directionalLight.intensity * diffuse ;\n"
		"}\n"

		//Directional light specular (Blinn)
		"vec3 calcDirLightSpec() {\n"
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		//"	vec3 reflectDir = reflect(-lightDir, normal);\n"
		"	vec3 viewDir =  normalize(viewPos - fPosition);\n"
		"	vec3 halfV = normalize(lightDir + viewDir);\n"
		//"	vec3 specular = directionalLight.color * pow(max(dot(reflectDir, viewDir), 0.0), material.glossness) * material.specular;\n"
		"	vec3 specular = directionalLight.color * pow(max(dot(normal, halfV), 0.0), material.glossness) * material.specular;\n"
		"	return directionalLight.intensity * specular ;\n"
		"}\n"

		//Spot light diffuse
		"vec3 calcSpotLightDiff(int i) {\n"
		"	vec3 lightDir = normalize(spotLight[i].position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight[i].direction)));\n"
		"	if (theta > spotLight[i].radian) {\n"
		"		return vec3(0.0f, 0.0f, 0.0f);\n"
		"	}\n"
		"	vec3 diffuse = spotLight[i].color * max(dot(lightDir, normal), 0.0f) * blendedDiffuse;\n"
		"	float distance = length(spotLight[i].position - fPosition);\n"
		"	float weakness = 1.0f /( 1.0f + spotLight[i].attenuation * distance  / 100);\n"
		"	return spotLight[i].intensity * weakness * diffuse;\n"
		"}\n"

		//Spot light specular
		"vec3 calcSpotLightSpec(int i) {\n"
		"	vec3 lightDir = normalize(spotLight[i].position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight[i].direction)));\n"
		"	if (theta > spotLight[i].radian) {\n"
		"		return vec3(0.0f, 0.0f, 0.0f);\n"
		"	}\n"
		//"	vec3 reflectDir = reflect(-lightDir, normal);"
		"	vec3 viewDir =  normalize(viewPos - fPosition);\n"
		"	vec3 halfV = normalize(lightDir + viewDir);\n"
		//"	vec3 specular = spotLight[i].color * pow(max(dot(reflectDir, viewDir), 0.0), material.glossness) * material.specular;\n//this for phong
		"	vec3 specular = spotLight[i].color * pow(max(dot(normal, halfV), 0.0), material.glossness) * material.specular;\n"//this is used for blinn-phong
		"	float distance = length(spotLight[i].position - fPosition);\n"
		"	float weakness = 1.0f / (1.0f + spotLight[i].attenuation * distance  / 100);\n"
		"	return spotLight[i].intensity * weakness * specular;\n"
		"}\n"

		"void main() {\n"
		"	if(!gl_FrontFacing) discard;\n"
		"	blendedDiffuse = mix(material.diffuseColor, vec3(texture(diffuseTexture,fTexCoord) ),material.textureBlend);\n"
		"	normal = normalize(fNormal);\n"
		"	vec3 ambient = blendedDiffuse * ambientLight.color * ambientLight.intensity;\n"
		"	vec3 diffuse = calcDirLightDiff() ;\n"
		"	vec3 specular = calcDirLightSpec();\n"
		"	for(int i = 0; i< spotNumber && i < 5; i++){\n"
		"		diffuse += calcSpotLightDiff( i );\n"
		"		specular += calcSpotLightSpec( i );\n"
		"	}\n"

		"	color = vec4(ambient + diffuse + specular, 1.0f);\n"
		//"	color = texture(diffuseTexture,fTexCoord);\n"
		"}\n";

	_defaultShader.reset(new GLSLProgram);
	_defaultShader->attachVertexShader(vsCode);
	_defaultShader->attachFragmentShader(fsCode);
	_defaultShader->link();

}

//___________________________________________ABOUT_SHADOW_SHADING_____________________________________________
void Scene::constructDepthShader(void)//this shader is used to generate a depth map for directional light
{
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"

		"uniform mat4 lightSpaceMatrix;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	gl_Position = lightSpaceMatrix * model * vec4(aPosition, 1.0f);\n"
		"} \n";

	const char* fsCode =//do nothing, just write into depth buffer
		"#version 330 core\n"
		"void main(){\n"
		"}\n";

	_depthShader.reset(new GLSLProgram);
	_depthShader->attachVertexShader(vsCode);
	_depthShader->attachFragmentShader(fsCode);
	_depthShader->link();

	glGenBuffers(1, &_depthMapFbo);
}

void Scene::constructShadowingShader(void)
{
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"

		"out vec3 fPosition;\n"
		"out vec3 fNormal;\n"
		"out vec2 fTexCoord;\n"
		"out vec4 fDirLightSpace;\n"
		"out vec4 fSpotLightSpace[5];\n"

		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 dirLightSpaceMatrix;\n"
		"uniform mat4 spotLightSpaceMatrix[5];\n"
		"uniform int spotNumber;\n"

		"void main() {\n"
		"	fPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	fTexCoord = aTexCoord;\n"
		"	fDirLightSpace = dirLightSpaceMatrix * vec4(fPosition, 1.0f);\n"
		"	for(int i = 0; i< spotNumber && i < 5; i++)fSpotLightSpace[i] = spotLightSpaceMatrix[i] * vec4(fPosition, 1.0f);\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"} \n";

	const char* fsCode =
		"#version 330 core\n"
		"in vec3 fPosition;\n"
		"in vec3 fNormal;\n"
		"in vec2 fTexCoord;\n"
		"in vec4 fDirLightSpace;\n"
		"in vec4 fSpotLightSpace[5];\n"
		"out vec4 color;\n"

		"struct Material {\n"
		"	vec3 diffuseColor;\n"
		"	vec3 specular;\n"
		"	float glossness;\n"
		"	float textureBlend;\n"
		"};\n"

		"struct AmbientLight {\n"
		"	vec3 color;\n"
		"	float intensity;\n"
		"};\n"

		"struct DirectionalLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"};\n"

		"struct SpotLight {\n"
		"	vec3 direction;\n"
		"	float intensity;\n"
		"	vec3 color;\n"
		"	float radian;\n"
		"	float attenuation;\n"
		"	vec3 position;\n"
		"};\n"

		"uniform Material material;\n"
		"uniform DirectionalLight directionalLight;\n"
		"uniform SpotLight spotLight[5];\n"
		"uniform AmbientLight ambientLight;\n"
		"uniform vec3 viewPos;\n"
		"uniform sampler2D diffuseTexture;\n"
		"uniform sampler2D dirDepthMap;\n"
		"uniform sampler2D spotDepthMap[5];\n"
		"uniform int spotNumber;\n"

		"vec3 blendedDiffuse;\n"
		"vec3 normal;\n"

		//Directional light diffuse
		"vec3 calcDirLightDiff() {\n"
		//calculate shadow
		"	vec3 projCoords = (fDirLightSpace.xyz / fDirLightSpace.w) * 0.5 + 0.5;\n"
		"	float minDepth = texture(dirDepthMap, projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"
		"	float shadow = 0.0;\n"
		"	vec2 texelSize = 1.0 / textureSize(dirDepthMap, 0);\n"
		"	for (int x = -1; x <= 1; ++x){\n"
		"		for (int y = -1; y <= 1; ++y){\n"
		"			float pcfDepth = texture(dirDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
		"			if(currentDepth < (pcfDepth+0.001))shadow += 1;\n"
		"		}\n"
		"	}\n"
		"	shadow /= 9.0;\n"
		//calculate color
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 diffuse = directionalLight.color * max(dot(lightDir, normal), 0.0f) * blendedDiffuse;\n"
		"	return directionalLight.intensity * diffuse * shadow ;\n"
		"}\n"

		//Directional light specular (Blinn)
		"vec3 calcDirLightSpec() {\n"
		//calculate shadow
		"	vec3 projCoords = (fDirLightSpace.xyz / fDirLightSpace.w) * 0.5 + 0.5;\n"
		"	float minDepth = texture(dirDepthMap, projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"
		"	if( currentDepth > minDepth) return vec3(0.0f, 0.0f, 0.0f);\n"
		//calculate color
		"	vec3 lightDir = normalize(-directionalLight.direction);\n"
		"	vec3 viewDir =  normalize(viewPos - fPosition);\n"
		"	vec3 halfV = normalize(lightDir + viewDir);\n"
		"	vec3 specular = directionalLight.color * pow(max(dot(normal, halfV), 0.0), material.glossness) * material.specular;\n"
		"	return directionalLight.intensity * specular ;\n"
		"}\n"

		//Spot light diffuse
		"vec3 calcSpotLightDiff(int i) {\n"
		//calculate angle
		"	vec3 lightDir = normalize(spotLight[i].position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight[i].direction)));\n"
		"	if (theta > spotLight[i].radian) {\n"
		"		return vec3(0.0f, 0.0f, 0.0f);\n"
		"	}\n"
		//calculate shadow
		"	vec3 projCoords = (fSpotLightSpace[i].xyz / fSpotLightSpace[i].w) * 0.5 + 0.5;\n"
		"	float minDepth = texture(spotDepthMap[i], projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"
		"	if( currentDepth > minDepth) return vec3(0.0f, 0.0f, 0.0f);\n"
		//calculate color
		"	vec3 diffuse = spotLight[i].color * max(dot(lightDir, normal), 0.0f) * blendedDiffuse;\n"
		"	float distance = length(spotLight[i].position - fPosition);\n"
		"	float weakness = 1.0f /( 1.0f + spotLight[i].attenuation * distance * distance / 1000);\n"
		"	return spotLight[i].intensity * weakness * diffuse;\n"
		"}\n"

		//Spot light specular
		"vec3 calcSpotLightSpec(int i) {\n"
		//calculate angle
		"	vec3 lightDir = normalize(spotLight[i].position - fPosition);\n"
		"	float theta = acos(-dot(lightDir, normalize(spotLight[i].direction)));\n"
		"	if (theta > spotLight[i].radian) {\n"
		"		return vec3(0.0f, 0.0f, 0.0f);\n"
		"	}\n"
		//calculate shadow
		"	vec3 projCoords = (fSpotLightSpace[i].xyz / fSpotLightSpace[i].w) * 0.5 + 0.5;\n"
		"	float minDepth = texture(spotDepthMap[i], projCoords.xy).r;\n"
		"	float currentDepth = projCoords.z;\n"
		"	if( currentDepth > minDepth) return vec3(0.0f, 0.0f, 0.0f);\n"
		//calculate color
		//"	vec3 reflectDir = reflect(-lightDir, normal);"
		"	vec3 viewDir =  normalize(viewPos - fPosition);\n"
		"	vec3 halfV = normalize(lightDir + viewDir);\n"
		//"	vec3 specular = spotLight[i].color * pow(max(dot(reflectDir, viewDir), 0.0), material.glossness) * material.specular;\n//this for phong
		"	vec3 specular = spotLight[i].color * pow(max(dot(normal, halfV), 0.0), material.glossness) * material.specular;\n"//this is used for blinn-phong
		"	float distance = length(spotLight[i].position - fPosition);\n"
		"	float weakness = 1.0f / (1.0f + spotLight[i].attenuation * distance * distance / 1000);\n"
		"	return spotLight[i].intensity * weakness * specular;\n"
		"}\n"

		"void main() {\n"
		"	if(!gl_FrontFacing) discard;\n"
		"	blendedDiffuse = mix(material.diffuseColor, vec3(texture(diffuseTexture,fTexCoord) ),material.textureBlend);\n"
		"	normal = normalize(fNormal);\n"
		"	vec3 ambient = blendedDiffuse * ambientLight.color * ambientLight.intensity;\n"
		"	vec3 diffuse = calcDirLightDiff() ;\n"
		"	vec3 specular = calcDirLightSpec();\n"
		"	for(int i = 0; i< spotNumber && i < 5; i++){\n"
		"		diffuse += calcSpotLightDiff( i );\n"
		"		specular += calcSpotLightSpec( i );\n"
		"	}\n"

		"	color = vec4(ambient + diffuse + specular, 1.0f);\n"
		//"	color = texture(diffuseTexture,fTexCoord);\n"
		"}\n";

	_shadowingShader.reset(new GLSLProgram);
	_shadowingShader->attachVertexShader(vsCode);
	_shadowingShader->attachFragmentShader(fsCode);//some BUG here!!!!!!!!!
	_shadowingShader->link();
	//configure samplers(I set samplers for 1 directional light and 5 spot lights)
	_shadowingShader->use();
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "dirDepthMap"), 0);
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "spotDepthMap[0]"), 1);
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "spotDepthMap[1]"), 2);
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "spotDepthMap[2]"), 3);
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "spotDepthMap[3]"), 4);
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "spotDepthMap[4]"), 5);
	glUniform1i(glGetUniformLocation(_shadowingShader->_handle, "diffuseTexture"), 6);
}