#pragma once

#include "simple_obj.h"
#include "../base/object3d.h"
#include "../base/texture.h"
#include "../base/light.h"
#include "po_camera.h"

class Character : public Object3D
{
public:
	Character(std::string _name, POCamera* _camera, std::string _animPath, unsigned int _fps, unsigned int _frames, unsigned int _materialID,  float _frontOffset, float _yOffset, float _scale);
	~Character();

	void setMaterial(unsigned int _materialID);
	unsigned int getMaterialID(void);

	void bindSoptLight(SpotLight* _spotLight, float _frontOffset, float _yOffset);
	SpotLight* getLight(void);

	void updateCharacter(void);
	Mesh* _currentFrame;
private:
	//meshes
	std::vector<Mesh*> _animFrames;
	unsigned int _frames;
	unsigned int _fps;
	//material
	unsigned int _materialID;
	//3rd person camera
	POCamera* _camera;
	//spot light
	SpotLight* _spotLight;
	bool _haveLight = false;
	//transform
	float _frontOffsetM;
	float _yOffsetM;//mesh
	float _frontOffsetL;
	float _yOffsetL;//light

	void updateTrans(void);
	void updateAnim(void);
	void updateLights(void);

	//timer
	clock_t _start, _end;
};
