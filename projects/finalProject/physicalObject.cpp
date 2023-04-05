#include "physicalObject.h"



PhysicalObject::PhysicalObject(unsigned int _meshID, unsigned int _matID)
{
	this->_meshID = _meshID;
	this->_matID = _matID;
}
PhysicalObject::~PhysicalObject()
{
	;
}

unsigned int PhysicalObject:: getMeshID(void)
{
	return this->_meshID;
}
unsigned int PhysicalObject::getMaterialID(void)
{
	return this->_matID;
}
void PhysicalObject::setMaterialID(unsigned int _matID)
{
	this->_matID = _matID;
}