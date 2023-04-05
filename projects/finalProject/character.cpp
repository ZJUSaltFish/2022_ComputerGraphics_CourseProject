#include "character.h"
Character::Character(std::string _name,POCamera* _camera, std::string _animPath, unsigned int _fps, unsigned int _frames, unsigned int _materialID,  float _frontOffset, float _yOffset, float _scale)
{
	this->_camera = _camera;
	this->_materialID = _materialID;
	this->_frontOffsetM = _frontOffset;
	this->_yOffsetM = _yOffset;
	this->_fps = _fps;
	this->_frames = _frames;
	this->scale *= _scale;
	this->_haveLight = false;
	this->_spotLight = NULL;

	for (int i = 0; i < _frames; i++) {//please make sure .obj files are named as "_name_i.obj", and one .obj only contains one mesh.
		char _index[2] ={i};
		itoa(i, _index, 10);
		Model _modelBuf(_animPath + _name + "_" + _index + ".obj");
		_animFrames.push_back(_modelBuf.getMeshes()[0]);
	}
	_start = clock();
	updateCharacter();
}

Character::~Character()
{

}
void Character::setMaterial(unsigned int _matID)
{
	_materialID = _matID;
}

unsigned int Character::getMaterialID(void)
{
	return _materialID;
}

void Character::updateAnim(void)
{
	_end = clock();
	_currentFrame = _animFrames[(_fps * _end / 1000)% _frames];
}

void Character::updateTrans(void)
{
	this->position = _camera->position + _camera->getFront() * _frontOffsetM + _camera->getUp() * _yOffsetM;
	this->rotation = _camera->rotation;
}

void Character::updateLights(void)
{
	_spotLight->position = _camera->position + _camera->getFront() * _frontOffsetL + _camera->getUp() * _yOffsetL;
	_spotLight->rotation = _camera->rotation;
}

void Character::updateCharacter(void)
{
	updateAnim();
	updateTrans();
	if(_haveLight)updateLights();
}

void Character::bindSoptLight(SpotLight* _spotLight, float _frontOffset, float _yOffset)
{
	_frontOffsetL = _frontOffset;
	_yOffsetL = _yOffset;
	_spotLight->position = _camera->position + _camera->getFront() * _frontOffsetL + _camera->getUp() * _yOffsetL;
	_spotLight->rotation = _camera->rotation;

	this->_spotLight = _spotLight;
	_haveLight = true;
}

SpotLight* Character::getLight(void)
{
	return _spotLight;
}