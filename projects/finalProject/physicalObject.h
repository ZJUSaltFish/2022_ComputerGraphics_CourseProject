#pragma once
#include "../base/object3d.h"
#include "simple_obj.h"
#include "material.h"


class PhysicalObject : public Object3D
{
public:
	PhysicalObject() = default;
	PhysicalObject(unsigned int _meshID, unsigned int _matID);
	~PhysicalObject();
	
	unsigned int getMeshID(void);
	unsigned int getMaterialID(void);
	void setMaterialID(unsigned int _maID);

private:
	unsigned int _meshID;
	unsigned int _matID;
};