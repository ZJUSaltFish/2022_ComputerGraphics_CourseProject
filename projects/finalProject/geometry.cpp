#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "geometry.h"
//___________________________________________________父类______________________________________
Mesh* BasicGeometry::getMesh() {
	return _mesh;
}

//_____________________________________________________立方体____________________________________________________-
GeoBox::GeoBox(float _a) {
	float _r = _a / 2.0f;
	_vertices.push_back({ glm::vec3(_r, _r, _r), glm::vec3(1, 0, 0), glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r,  _r, -_r),glm::vec3(1, 0, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r, -_r, -_r),glm::vec3(1, 0, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r, -_r,  _r),glm::vec3(1, 0, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r,  _r,  _r),glm::vec3(-1, 0, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r,  _r, -_r),glm::vec3(-1, 0, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r, -_r, -_r),glm::vec3(-1, 0, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r, -_r,  _r),glm::vec3(-1, 0, 0),glm::vec2(0) });

	_indices.push_back(0); _indices.push_back(3); _indices.push_back(2);
	_indices.push_back(2); _indices.push_back(1); _indices.push_back(0);
	_indices.push_back(4); _indices.push_back(5); _indices.push_back(6);
	_indices.push_back(6); _indices.push_back(7); _indices.push_back(4);

	_vertices.push_back({ glm::vec3(_r, _r, _r),glm::vec3(0, 1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r,  _r, -_r),glm::vec3(0, 1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r, -_r, -_r),glm::vec3(0, -1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r, -_r,  _r),glm::vec3(0, -1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r,  _r,  _r),glm::vec3(0, 1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r,  _r, -_r),glm::vec3(0, 1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r, -_r, -_r),glm::vec3(0, -1, 0),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r, -_r, _r),glm::vec3(0, -1, 0),glm::vec2(0) });

	_indices.push_back(0 + 8); _indices.push_back(5 + 8); _indices.push_back(4 + 8);
	_indices.push_back(0 + 8); _indices.push_back(1 + 8); _indices.push_back(5 + 8);
	_indices.push_back(3 + 8); _indices.push_back(7 + 8); _indices.push_back(6 + 8);
	_indices.push_back(6 + 8); _indices.push_back(2 + 8); _indices.push_back(3 + 8);

	_vertices.push_back({ glm::vec3(_r,  _r,  _r),glm::vec3(0, 0, 1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r,  _r, -_r),glm::vec3(0, 0, -1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r, -_r, -_r),glm::vec3(0, 0, -1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(_r, -_r,  _r),glm::vec3(0, 0, 1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r,  _r,  _r),glm::vec3(0, 0, 1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r,  _r, -_r),glm::vec3(0, 0, -1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r, -_r, -_r),glm::vec3(0, 0, -1),glm::vec2(0) });
	_vertices.push_back({ glm::vec3(-_r, -_r, _r),glm::vec3(0, 0, 1),glm::vec2(0) });

	_indices.push_back(0 + 16); _indices.push_back(7 + 16); _indices.push_back(3 + 16);
	_indices.push_back(0 + 16); _indices.push_back(4 + 16); _indices.push_back(7 + 16);
	_indices.push_back(1 + 16); _indices.push_back(2 + 16); _indices.push_back(6 + 16);
	_indices.push_back(6 + 16); _indices.push_back(5 + 16); _indices.push_back(1 + 16);

	_edgeLength = _a;
	_mesh = new Mesh(_vertices, _indices, "Box_Geometry.obj");
}
GeoBox::~GeoBox() {

}
//____________________________________________________棱柱______________________________________________________________________________________
GeoColumn::GeoColumn(unsigned int _sides, float _height, float _radius) {
	//first create wall
	glm::vec3 _posVecB = glm::vec3(_radius, 0, 0.0f );//底部顶点
	glm::vec3 _posVecT = glm::vec3(_radius, _height, 0.0f);//顶部顶点
	glm::vec3 _normal1 = glm::vec3( glm::cos(glm::radians(180.0f / _sides)), 0.0f,- glm::sin(glm::radians(180.0f / _sides)));
	glm::mat4 _trans = glm::mat4(1.0f);
	_trans = glm::rotate(_trans, glm::radians(360.0f/_sides), glm::vec3(0.0, 1.0, 0.0));
	//WALL
	Vertex _tmpV = { _posVecT, _normal1, glm::vec2(0) };//侧面按面构造
	for (int i = 0; i < _sides; i++) {
		_vertices.push_back(_tmpV); 

		_tmpV._positionVec.y = 0;
		_vertices.push_back(_tmpV); 

		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
		_vertices.push_back(_tmpV); 

		_tmpV._positionVec.y = _height;
		_vertices.push_back(_tmpV); 

		_indices.push_back(0 + 4 * i); _indices.push_back(1 + 4 * i); _indices.push_back(2 + 4 * i);
		_indices.push_back(2 + 4 * i); _indices.push_back(3 + 4 * i); _indices.push_back(0 + 4 * i);

		_tmpV._normalVec = glm::vec3(_trans * glm::vec4(_tmpV._normalVec, 1.0f));
	}
	//TOP
	_tmpV._positionVec = _posVecT;//reset tmpV to make TOP
	_tmpV._normalVec = glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < _sides ; i++) {//顶面
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));	
	}
	for (int i = 1; i < _sides -1; i++) {//顶面序列
		_indices.push_back(4 * _sides); _indices.push_back(4 * _sides + i); _indices.push_back(4 * _sides + i +1);
	}
	//BOTTOM
	_tmpV._positionVec = _posVecB;//reset tmpV to make BOTTOM
	_tmpV._normalVec = glm::vec3(0.0f, -1.0f, 0.0f);
	for (int i = 0; i < _sides; i++) {
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
	}
	for (int i = 1; i < _sides - 1; i++) {
		_indices.push_back(5 * _sides + i +1); _indices.push_back(5 * _sides +i); _indices.push_back(5 * _sides);
	}

	this->_radius = _radius;
	this->_height = _height;
	this->_sides = _sides;
	_mesh = new Mesh(_vertices, _indices, "Column_Geometry.obj");

}
GeoColumn::~GeoColumn() {

}

//_________________________________________________________圆柱______________________________________-
GeoCylinder::GeoCylinder(unsigned int _sides, float _height, float _radius) {
	//calculate here
	glm::vec3 _posVecB = glm::vec3(_radius, 0, 0.0f);//底部顶点
	glm::vec3 _posVecT = glm::vec3(_radius, _height, 0.0f);//顶部顶点
	glm::vec3 _normal = glm::vec3(1, 0.0f, 0.0f);
	glm::mat4 _trans = glm::mat4(1.0f);
	_trans = glm::rotate(_trans, glm::radians(360.0f / _sides), glm::vec3(0.0, 1.0, 0.0));
	//WALL
	Vertex _tmpV = { _posVecT, _normal, glm::vec2(0) };//侧面按面构造
	for (int i = 0; i < _sides; i++) {
		_tmpV._positionVec.y = _height;
		_vertices.push_back(_tmpV);

		_tmpV._positionVec.y = 0;
		_vertices.push_back(_tmpV);

		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));		
		_tmpV._normalVec = glm::vec3(_trans * glm::vec4(_tmpV._normalVec, 1.0f));
	}
	for (int i = 0; i < _sides-1; i++) {
		_indices.push_back(0 + 2 * i); _indices.push_back(1 + 2 * i); _indices.push_back(2 + 2 * i);
		_indices.push_back(3 + 2 * i); _indices.push_back(2 + 2 * i); _indices.push_back(1 + 2 * i);
	}
	_indices.push_back(0+2 * (_sides - 1)); _indices.push_back(1 + 2 *(_sides - 1)); _indices.push_back(2 + 2 * (_sides - 1));
	_indices.push_back(1); _indices.push_back(0); _indices.push_back(1 + 2 * (_sides - 1));

	//TOP
	_tmpV._positionVec = _posVecT;//reset tmpV to make TOP
	_tmpV._normalVec = glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < _sides; i++) {//顶面
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
	}
	for (int i = 1; i < _sides - 1; i++) {//顶面序列
		_indices.push_back(2 * _sides); _indices.push_back(2 * _sides + i); _indices.push_back(2 * _sides + i + 1);
	}
	//BOTTOM
	_tmpV._positionVec = _posVecB;//reset tmpV to make BOTTOM
	_tmpV._normalVec = glm::vec3(0.0f, -1.0f, 0.0f);
	for (int i = 0; i < _sides;  i++) {
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
	}
	for (int i = 1; i < _sides - 1; i++) {
		_indices.push_back(3 * _sides + i + 1); _indices.push_back(3 * _sides + i); _indices.push_back(3 * _sides);
	}

	this->_radius = _radius;
	this->_height = _height;
	this->_sides = _sides;
	_mesh = new Mesh(_vertices, _indices, "Cylinder_Geometry.obj");
}
GeoCylinder::~GeoCylinder() {

}

//_____________________________________________________棱台_________________________________________-
GeoFrustum::GeoFrustum(unsigned int _sides, float _height, float _radiusT, float _radiusB) {
	//first create wall
	glm::vec3 _posVecB = glm::vec3(_radiusB, 0, 0.0f);//底部顶点
	glm::vec3 _posVecT = glm::vec3(_radiusT, _height, 0.0f);//顶部顶点
	glm::vec3 _normal1 = glm::vec3(glm::cos(glm::radians(180.0f / _sides)), 0.0f, -glm::sin(glm::radians(180.0f / _sides)));
	glm::mat4 _trans = glm::mat4(1.0f);
	_trans = glm::rotate(_trans, glm::radians(360.0f / _sides), glm::vec3(0.0, 1.0, 0.0));
	//WALL
	Vertex _tmpV = { _posVecT, _normal1, glm::vec2(0) };//侧面按面构造
	Vertex _tmpVB = { _posVecB, _normal1, glm::vec2(0) };
	_tmpVB._positionVec.y = 0;
	for (int i = 0; i < _sides; i++) {
		_vertices.push_back(_tmpV);

		_vertices.push_back(_tmpVB);

		_tmpVB._positionVec = glm::vec3(_trans * glm::vec4(_tmpVB._positionVec, 1.0f));
		_vertices.push_back(_tmpVB);

		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
		_vertices.push_back(_tmpV);

		_indices.push_back(0 + 4 * i); _indices.push_back(1 + 4 * i); _indices.push_back(2 + 4 * i);
		_indices.push_back(2 + 4 * i); _indices.push_back(3 + 4 * i); _indices.push_back(0 + 4 * i);

		_tmpV._normalVec = glm::vec3(_trans * glm::vec4(_tmpV._normalVec, 1.0f));

		_tmpVB._normalVec = glm::vec3(_trans * glm::vec4(_tmpVB._normalVec, 1.0f));

	}
	//TOP
	_tmpV._positionVec = _posVecT;//reset tmpV to make TOP
	_tmpV._normalVec = glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < _sides; i++) {//顶面
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
	}
	for (int i = 1; i < _sides - 1; i++) {//顶面序列
		_indices.push_back(4 * _sides); _indices.push_back(4 * _sides + i); _indices.push_back(4 * _sides + i + 1);
	}
	//BOTTOM
	_tmpVB._positionVec = _posVecB;//reset tmpV to make BOTTOM
	_tmpVB._normalVec = glm::vec3(0.0f, -1.0f, 0.0f);
	for (int i = 0; i < _sides; i++) {
		_vertices.push_back(_tmpVB);
		_tmpVB._positionVec = glm::vec3(_trans * glm::vec4(_tmpVB._positionVec, 1.0f));
	}
	for (int i = 1; i < _sides - 1; i++) {
		_indices.push_back(5 * _sides + i + 1); _indices.push_back(5 * _sides + i); _indices.push_back(5 * _sides);
	}



	this->_radiusTop = _radiusT;
	this->_radiusBottom = _radiusB;
	this->_height = _height;
	this->_sides = _sides;
	_mesh = new Mesh(_vertices, _indices, "FrustumOfPyramid_Geometry.obj");
}
GeoFrustum::~GeoFrustum() {

}


//___________________________________________圆锥_____________________________________________________
GeoCone::GeoCone(unsigned int _sides, float _height, float _radius) {
	//calculate here

	glm::vec3 _posVecB = glm::vec3(_radius, 0, 0.0f);//底部顶点
	glm::vec3 _normal = glm::vec3(_height / (_radius * _radius + _height * _height), _radius / (_radius * _radius + _height * _height), 0.0f);
	glm::mat4 _trans = glm::mat4(1.0f);
	_trans = glm::rotate(_trans, glm::radians(360.0f / _sides), glm::vec3(0.0, 1.0, 0.0));
	//Top
	Vertex _top = { glm::vec3(0.0f, _height, 0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(0)};
	_vertices.push_back(_top);
	//WALL
	Vertex _tmpV = { _posVecB, _normal, glm::vec2(0) };//侧面按面构造
	for (int i = 0; i < _sides; i++) {
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
		_indices.push_back(0); _indices.push_back(1 + i); _indices.push_back(2 + i);
		_tmpV._normalVec = glm::vec3(_trans * glm::vec4(_tmpV._normalVec, 1.0f));
	}
	//BOTTOM
	_tmpV._positionVec = _posVecB;//reset tmpV to make BOTTOM
	_tmpV._normalVec = glm::vec3(0.0f, -1.0f, 0.0f);
	for (int i = 0; i < _sides; i++) {
		_vertices.push_back(_tmpV);
		_tmpV._positionVec = glm::vec3(_trans * glm::vec4(_tmpV._positionVec, 1.0f));
	}
	for (int i = 1; i < _sides - 1; i++) {
		_indices.push_back(0); _indices.push_back(2 * _sides + i); _indices.push_back(2 * _sides + i + 1);
	}

	this->_radius = _radius;
	this->_height = _height;
	this->_sides = _sides;
	_mesh = new Mesh(_vertices, _indices, "Cone_Geometry.obj");
}
GeoCone::~GeoCone() {

}

/*


	
	//立方体顶点绘制顺序


	//多面棱台顶点绘制顺序

	Vertex _vertices4[202]; //圆柱
	Vertex _vertices5[102]; //圆锥

	float _PI = acos(-1.0f);
	glm::vec3 _cylinder[202];
	glm::vec3 _cone[102];
	for (int i = 0; i < 100; i++)
	{
		_cylinder[i] = glm::vec3(0.1f * cos(i * _PI / 50.0f),0.75f + 0.1f * sin(i * _PI / 50.0f) ,0.1f);
		_vertices4[i] = { _cylinder[i],glm::vec3(0.0f, 1.0f, 1.0f) };
	}
	for (int i = 0; i < 100; i++)
	{
		_cylinder[i+100] = glm::vec3(0.1f * cos(i * _PI / 50.0f), 0.75f + _rate * 0.1f * sin(i * _PI / 50.0f), -0.1f);
		_vertices4[i+100] = { _cylinder[i+100],glm::vec3(0.0f, 1.0f, 1.0f) };
		_cone[i] = glm::vec3(0.1f * cos(i * _PI / 50.0f), -0.75f + _rate * 0.1f * sin(i * _PI / 50.0f), -0.1f);
		_vertices5[i] = { _cone[i],glm::vec3(1.0f, 1.0f, 0.0f) };
	}

	_cylinder[200] = glm::vec3(0.0f, 0.75f, 1.0f);
	_cylinder[201] = glm::vec3(0.0f, 0.75f, -1.0f);
	_vertices4[200] = { _cylinder[200],glm::vec3(0.0f, 1.0f, 1.0f) };
	_vertices4[201] = { _cylinder[201],glm::vec3(0.0f, 1.0f, 1.0f) };
	_cone[100] = glm::vec3(0.0f, -0.75f, 1.0f);
	_cone[101] = glm::vec3(0.0f, -0.75f, -1.0f);
	_vertices5[100] = { _cone[100],glm::vec3(1.0f, 1.0f, 0.0f) };
	_vertices5[101] = { _cone[101],glm::vec3(1.0f, 1.0f, 0.0f) };


	//计算圆柱、圆锥上的点

	int _cylinderindex[1200];
	int _coneindex[600];

	for (int i = 0; i < 99; i++)
	{
		_cylinderindex[i * 3] = i;
		_cylinderindex[i * 3 + 1] = i + 1;
		_cylinderindex[i * 3 + 2] = 200;
		_cylinderindex[i * 3 + 300] = i + 100;
		_cylinderindex[i * 3 + 301] = i + 101;
		_cylinderindex[i * 3 + 302] = 201;
		_cylinderindex[i * 3 + 600] = i;
		_cylinderindex[i * 3 + 601] = i + 100;
		_cylinderindex[i * 3 + 602] = i + 101;
		_cylinderindex[i * 3 + 901] = i;
		_cylinderindex[i * 3 + 900] = i + 1;
		_cylinderindex[i * 3 + 902] = i + 101;
	}
	_cylinderindex[297] = 99;
	_cylinderindex[298] = 0;
	_cylinderindex[299] = 200;
	_cylinderindex[597] = 199;
	_cylinderindex[598] = 100;
	_cylinderindex[599] = 201;
	_cylinderindex[897] = 99;
	_cylinderindex[898] = 199;
	_cylinderindex[899] = 100;
	_cylinderindex[1197] = 99;
	_cylinderindex[1198] = 0;
	_cylinderindex[1199] = 100;

	//绘制圆柱使用的三角形顶点顺序


	for (int i = 0; i < 99; i++)
	{
		_coneindex[i * 3] = i;
		_coneindex[i * 3 + 1] = i + 1;
		_coneindex[i * 3 + 2] = 100;
		_coneindex[i * 3 + 300] = i;
		_coneindex[i * 3 + 301] = i + 1;
		_coneindex[i * 3 + 302] = 101;
	}
	_coneindex[297] = 99;
	_coneindex[298] = 0;
	_coneindex[299] = 100;
	_coneindex[597] = 99;
	_coneindex[598] = 0;
	_coneindex[599] = 101;

	//绘制圆锥使用的三角形顶点顺序

Bodies::~Bodies() {


}

GeoBox::~GeoBox()
{
}
*/