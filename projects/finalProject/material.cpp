#include "material.h"

Material::Material()
{
	this->_diffuceColor = glm::vec3(0.5);

	this->_specular = glm::vec3(0.5, 0.5, 1.0);
	this->_glossness = 20;

	this->_opacityMode = false;
	
	this->_textureBlend = 0;
	this->_textureID = -1;
}
Material::Material(glm::vec3 _diffuse, glm::vec3 _specular, float _gloss, bool _ifTranseparent, float _textureBlend, unsigned int _textureID)
{
	this->_diffuceColor = _diffuse;

	this->_specular = _specular;
	this->_glossness = _gloss;

	this->_opacityMode = _ifTranseparent;

	this->_textureBlend = _textureBlend;
	this->_textureID = _textureID;
}

Material::~Material()
{

	;
}


