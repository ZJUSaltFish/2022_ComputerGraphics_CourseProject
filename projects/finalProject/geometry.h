#pragma once

#include <memory> 
#include <vector>
#include "../base/application.h"
#include "../base/glsl_program.h"
#include "simple_obj.h"
#define MY_PI 3.14159265357

class BasicGeometry {
public:
	Mesh* getMesh();
protected:
	Mesh* _mesh;
	std::vector<unsigned int> _indices;
	std::vector<Vertex> _vertices;
};


class GeoBox : public BasicGeometry {
public:
	GeoBox(float _edgeLength);
	~GeoBox();
private:
	float _edgeLength = 0;
};


class GeoColumn : public BasicGeometry
{
public:
	GeoColumn(unsigned int _sides, float _height, float _radius);
	~GeoColumn();

private:
	unsigned int _sides;
	float _height;
	float _radius;
};

//cylinder is a smoothed column.(only different in normals)
class GeoCylinder : public BasicGeometry
{
public:
	GeoCylinder(unsigned int _sides, float _height, float _radius);
	~GeoCylinder();

private:
	unsigned int _sides;
	float _height;
	float _radius;
};


class GeoFrustum : public BasicGeometry
{
public:
	GeoFrustum(unsigned int _sides, float _height, float _radiusTop, float _radiusBottom);
	~GeoFrustum();
private:
	unsigned int _sides;
	float _height;
	float _radiusTop;
	float _radiusBottom;
};


class GeoPyramid : public BasicGeometry
{
public:
	GeoPyramid(unsigned int _sides, float _height, float _radius);
	~GeoPyramid();
private:
	unsigned int _sides;
	float _height;
	float _radius;
};

//cone is a smothed pyramid
class GeoCone : public BasicGeometry
{
public:
	GeoCone(unsigned int _sides, float _height, float _radius);
	~GeoCone();
private:
	unsigned int _sides;
	float _height;
	float _radius;
};


class GeoSphere : public BasicGeometry
{
public:
	GeoSphere(unsigned int _sides, float _radius);
	~GeoSphere();
private:
	unsigned int _sides;
	float _radius;
};