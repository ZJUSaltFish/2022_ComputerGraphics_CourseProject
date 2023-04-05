#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/integer.hpp>
#include "physicalObject.h"
#include "po_camera.h"
#include "../base/light.h"
#include "../base/texture.h"
#include "../base/skybox.h"
#include "character.h"

//defines:
#define MY_AXIS_X 0
#define MY_AXIS_Y 1
#define MY_AXIS_Z 2

#define MY_CAMERA_PERSP 0
#define MY_CAMERA_ORTHO 1
#define MY_FPS_FRONT 0
#define MY_FPS_RIGHT 1
#define MY_FPS_UP 2

#define MY_MAT_DIFFUSE 0
#define MY_MAT_SPECULAR 1
#define MY_MAT_GLOSS 2
#define MY_MAT_BLEND 3

#define MY_LIGHT_AMBIENT 0
#define MY_LIGHT_DIRECTIONAL 1
#define MY_LIGHT_SPOT 2
#define MY_LIGHT_COLOR 3
#define MY_LIGHT_INTENSITY 4
#define MY_LIGHT_ANGLE 5
#define MY_LIGHT_ATTENUATION 6


#define MY_SHADER_DEFAULT 0
#define MY_SHADER_SHADOWING 1
#define MY_SHADER_DEPTHING 2
#define MY_SHADOW_SIZE 1024 // size for depth map in shadowing

class Scene
{
public:
	Scene();
	~Scene();
	//object functions
	unsigned int addPhysicalObject(Mesh* _meshPtr, unsigned int matID);//generate a new object from mesh. return objID
	unsigned int getNumOfObjects(void);
	void setObjectMaterial(unsigned int _objID, unsigned int _matID);
	void setObjectPosition(unsigned int _objID, glm::vec3 _positionVec);
	void setObjectRotation(unsigned int _objID, glm::vec3 _axis, float _angle);//rotate an object around the axis, to the angle
	void setObjectScale(unsigned int _objID, glm::vec3 _scale);
	void moveObject(unsigned int _objID, glm::vec3 _deltaPosition);
	void rotateObject(unsigned int _objID, glm::vec3 _axis, float _angle);//rotate an object around the axis, by the degee
	void scaleObject(unsigned int _objID, glm::vec3 _deltaScale);

	//character functions
	unsigned int newCharacter(std::string _name, unsigned int _cameraID, std::string _animPath, unsigned int _fps, unsigned int _frames, unsigned int _materialID, float _frontOffset, float _yOffset, float _scale);
	void useCharacter(unsigned int _characterID);
	void characterBindLight(unsigned int _characterID, unsigned int _spotLightID, float _frontOffset, float _yOffset);
	SpotLight* getCharacterLight(unsigned int _characterID);
	unsigned int getCurrentCharacter(void);
	unsigned int getCharacterMaterial(unsigned int _characterID);
	void setCharacterMaterial(unsigned int _characterID, unsigned int _materialID);

	//texture functions
	unsigned int addTexture2D(Texture2D* _texture);//load a texture 2D to scene. return ID int textureList 

	//material functions
	unsigned int newMaterial(glm::vec3 _diffuse, glm::vec3 _specular, float _gloss, bool _ifTranseparent, float _textureBlend, unsigned int _textureID);
	void setMaterialTexture(unsigned int _materialID, unsigned int _textureID);
	void setMaterialFloat(unsigned int _materialID, unsigned int _attribName, float _value);
	void setMaterialColor(unsigned int _materialID, unsigned int _attribName, glm::vec3 _color);

	//light functions
	unsigned int newAmbientLight(glm::vec3 _color, float _intensity);
	unsigned int newDirectionalLight(glm::vec3 _color, float _intensity, glm::vec3 _direction);//Notice: direction can't be parrallel with y axis, and must be normalized.
	unsigned int newSpotLight(glm::vec3 _color, float _intensity, glm::vec3 _direction, float _degree, float _attenuation);//Notice: direction can't be parrallel with y axis, and must be normalized.
	void setLightFloat(unsigned int _lightType, unsigned int _lightID, unsigned int _attribName, float _value);//set float parameter of a light,
	void setLightColor(unsigned int _lightType, unsigned int _lightID, glm::vec3 _value);//set color of a light
	
	void setSpotLightPosition(unsigned int _spotLightID, glm::vec3 _positionVec);//"position" is only valid fot spot light

	//shader functions
	void useShader(unsigned int _shaderID);

	//camera functions
	void useCamera(unsigned int _mode, unsigned int _cameraID);
	void switchCameraMode();

	unsigned int addCamera(float _whRatio, float _zNear, float _zFar, float _fovDegree, float _windowHeight);
	unsigned int getCurrentCamera(void);
	unsigned int getNumOfCameras(void);

	void setCameraPosition(unsigned int _cameraID, glm::vec3 _position);
	void setCameraRotationQuat(unsigned int _cameraID, glm::quat _rotation);//use Quat to set.useful when getting rotation from Object3D
	void setCameraRotation(unsigned int __cameraID, glm::vec3 _axis, float _value);
	void setCameraScale(unsigned int _cameraID, glm::vec3 _scale);//Actually this function is useless because camera is not a mesh

	void fpsMove(unsigned int _direction, float _value);
	void fpsTurn(unsigned int _direction, float _value);

	void zoom(float _value);

	void cameraReFocus();
	//drawing
	void drawScene(void);
	
	void exportMesh(unsigned int _meshID);

	void updateViewportSize(unsigned int _viewWidth, unsigned int _viewHeight);

private:

	void drawObject(PhysicalObject* _object);
	void drawCharacter(Character* _character);

	void constructDefaultShader(void);

	void constructDepthShader(void);
	void constructShadowingShader(void);

	std::vector<Mesh*> _meshList;
	std::vector<PhysicalObject*> _objectList;
	std::vector<Character*> _characterList;
	std::vector<AmbientLight*> _ambientLightList;//0 -> default
	std::vector<DirectionalLight*> _directionalLightList;//0 -> default
	std::vector<SpotLight*> _spotLightList;
	std::vector<POCamera*> _cameraList;
	std::vector<Material*> _materialList;//material 0 = auto generated default mat
	std::vector<Texture2D*> _texture2DList;
	std::unique_ptr<SkyBox> _skyBox;
	//std::vector<unsigned int > _shadingModes;
	//character
	unsigned int _currentCharacter;
	//camera
	unsigned int _currentCamera;
	unsigned int _currentCameraMode;
	//shaders
	unsigned int _numOfShaders;
	unsigned int _currentShader;//0: auto generated blinn-phnog shader

	unsigned int _viewWidth;
	unsigned int _viewHeight;
	std::unique_ptr<GLSLProgram> _defaultShader;

	std::unique_ptr<GLSLProgram> _depthShader;
	std::unique_ptr<GLSLProgram> _shadowingShader;
	unsigned int _depthMapFbo;
	std::vector<DataTexture*> _dirDepthMapList;//when adding a new directional light to scene, push_back a new element.
	std::vector<DataTexture*> _spotDepthMapList;
};



