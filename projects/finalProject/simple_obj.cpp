#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "simple_obj.h"
#include <fstream>

Mesh::Mesh()
{

}
Mesh::Mesh(std::vector<Vertex>_vertices, std::vector<unsigned int>_indices, std::string _name)
{
	this->_vertices = _vertices;
	this->_indices = _indices;
	this->_name = _name;
	setup();
}
Mesh::~Mesh()
{
	if (_vbo != 0) { glDeleteBuffers(1, &_vbo); _vbo = 0; }
	if (_vao != 0) { glDeleteBuffers(1, &_vao); _vao = 0; }
	if (_ebo != 0) { glDeleteBuffers(1, &_ebo); _ebo = 0; }
}
void Mesh::setup()
{	//generate
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	//bind&transfer data
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	//set position vector
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _positionVec));
	//set normal vector
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _normalVec));
	//set texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _textureCoord));

	glBindVertexArray(0);
}

std::string Mesh::getName()
{
	return _name;
}

void Mesh::exportMesh(std::string _path, std::string _name)
{
	//create file
	std::ofstream _newObj;
	_newObj.open(_path.append(_name), std::ios::out | std::ios::trunc);
	//export name
	std::string _mainName;
	for (int i = 0; _name[i] != '_'; i++)_mainName.insert(i, 1, _name[i]);
	_newObj << "# This is a created obj mesh file." << std::endl;
	_newObj << "o" << ' ' << _mainName << std::endl;
	//export data
	unsigned int _vs = _vertices.size(), _fs = _indices.size();
	for (int i = 0; i < _vs; i++) {//v
		_newObj << "v " << _vertices[i]._positionVec.x <<' '<< _vertices[i]._positionVec.y << ' ' << _vertices[i]._positionVec.z << std::endl;
	}
	for (int i = 0; i < _vs; i++) {//vt
		_newObj << "vt " << _vertices[i]._textureCoord.x << ' ' << _vertices[i]._textureCoord.y <<  std::endl;
	}
	for (int i = 0; i < _vs; i++) {//vn
		_newObj << "vn " << _vertices[i]._normalVec.x << ' ' << _vertices[i]._normalVec.y << ' ' << _vertices[i]._normalVec.z << std::endl;
	}
	for (int i = 0; i < _fs; i+=3) {//f
		_newObj << "f " << _indices[i]+1 << '/' << _indices[i]+1 << '/' << _indices[i]+1
			<< ' ' << _indices[i + 1]+1 << '/' << _indices[i + 1]+1 << '/' << _indices[i + 1]+1
			<< ' ' << _indices[i + 2]+1 << '/' << _indices[i + 2]+1 << '/' << _indices[i + 2]+1 << std::endl;
	}
	//close file 
	_newObj.close();
}


unsigned int Mesh::getVao(void)
{
	return _vao;
}

unsigned int Mesh::getVbo(void)
{
	return _vbo;
}

unsigned int Mesh::getEbo(void)
{
	return _ebo;
}

unsigned int Mesh::getVertexSize(void)
{
	return _vertices.size();
}

unsigned int Mesh::getIndexSize(void)
{
	return _indices.size();
}
//______________________MODEL FUNCS______________________________

Model::Model(std::string _path)
{
	load(_path);
}

Model::~Model()
{

}
//the function to load a obj and generate mesh
//can only import: vertex normal texCoord meshName(1 obj-> more than one meshes) 
//can only read triangle faces
void Model::load(std::string _path)
{
	std::ifstream _inObj(_path);//infile stream
	if (!_inObj) {//if cannot find the file
		std::cout << "cannot open file:" << _path << std::endl;
		return;
	}

	std::string _wordBuf;//a word in obj file
	int _currentMeshID = -1;//the ith mesh in the obj file

	bool _vnLoaded = false;
	bool _vtLoaded = false;

	while (_inObj) {//while the infile stream is not empty
		//read first word per line, to get the indicator(v,vt,vn...) and detect whether it is an empty line 
		_inObj >> _wordBuf;
		if (!_wordBuf.empty()) {
			//Vertex _tmpV = { glm::vec3(0), glm::vec3(0), glm::vec2(0) };
			if (_wordBuf == "o") {//set mesh name. If not mentioned, use dfault name
				_inObj >> _wordBuf;
				_meshNames.push_back(_wordBuf);
				++_currentMeshID;
			}
			else if(_wordBuf == "v") {//3D vertex position
				glm::vec3 _tmp = glm::vec3(0);
				_inObj >> _wordBuf;
				_tmp.x = atof(&_wordBuf[0]);
				_inObj >> _wordBuf;
				_tmp.y = atof(&_wordBuf[0]);
				_inObj >> _wordBuf;
				_tmp.z = atof(&_wordBuf[0]);

				_positions.push_back(_tmp);
			}
			else if (_wordBuf == "vt") {//2D texture coord
				glm::vec2 _tmp = glm::vec2(0);
				_inObj >> _wordBuf;
				_tmp.x = atof(&_wordBuf[0]);
				_inObj >> _wordBuf;
				_tmp.y = atof(&_wordBuf[0]);
				_texCoords.push_back(_tmp);
				_vtLoaded = true;
			}
			else if (_wordBuf == "vn") {//3D normal coord
				glm::vec3 _tmp = glm::vec3(0);
				_inObj >> _wordBuf;
				_tmp.x = atof(&_wordBuf[0]);
				_inObj >> _wordBuf;
				_tmp.y = atof(&_wordBuf[0]);
				_inObj >> _wordBuf;
				_tmp.z = atof(&_wordBuf[0]);
				_normals.push_back(_tmp);
				_vnLoaded = true;
			}
			else if (_wordBuf == "f") {//index
				glm::vec4 _tmp;	
				for (int i = 0; i < 3; i++) {//a triangle face has 3 vertices, do it 3 times
					short int _mark = 0;
					_inObj >> _wordBuf;
					_tmp.x = atoi(&_wordBuf[_mark]);//format: posIndex/texIndex/norIndex. this is posIndex

					_mark = _wordBuf.find_first_of('/');
					_tmp.y = atoi(&_wordBuf[++_mark]);//texIndex

					_mark = _wordBuf.find_last_of('/');
					_tmp.z = atoi(&_wordBuf[++_mark]);//norIndex

					if (_currentMeshID == -1)_tmp.w = 0;
					else _tmp.w = _currentMeshID;//the mesh that this vertex belongs to 
					_indices.push_back(_tmp);
					//std::cout << '|' << _indices.back().x << '|' << _indices.back().y << '|' << _indices.back().z << '|';
				}
				//putchar('\n');
			}//end if:switch_mode

		}//end if_not_empty
		
	}//end while
	//if normal is missing, error!
	if (!_vnLoaded) {
		std::cout << "ERROR: failed to load mesh:[" << _path << "]. No Normal Info." << std::endl;
		return;
	}
	//if texCoord is missing: auto generate them to 0
	if (!_vtLoaded) {
		std::cout << "WARNING: no texture coord infomation in mesh:[" << _path << "]." << std::endl;
		_texCoords.push_back(glm::vec2(0));
		for (int i= 0; i < _indices.size(); i++){
			_indices[i].y = 1;
		}

	}
	//finish loading, prepare to gneerate mesh
	std::cout << "read in file:[" << _path << "] finished!" << std::endl;
	std::string _namePostfix = &_path[_path.find_last_of('/')+1];//postfix represents the model file of a mesh
	if (_currentMeshID == -1)//if mesh name not defined
	{
		std::string _name = "default_";
		generateAMesh(_name.append(_namePostfix), 0);
	}else {
		for (int i = 0; i <= _currentMeshID; i++) {
			generateAMesh((_meshNames[i].append("_")).append(_namePostfix), i);
		}
	}
}

void Model::generateAMesh(std::string _name, unsigned int _meshID)
{
	//vairiables to generate a mesh
	std::vector<Vertex> _vertices;//all vertices of this mesh
	std::vector<unsigned int> _elementIndices;//obj indices of the ith drawn vertex.

	unsigned int _numOfV;//number of vertices
	unsigned int _numOfE = _indices.size();//number of vertices to draw
	unsigned int _start = 0;//from which vertex this mesh starts
	unsigned int _end = _numOfE;
	for (_start = 0; _indices[_start].w < _meshID; ++_start);
	for (_end = _start;( _end + 1)< _numOfE&& _indices[_end + 1].w == _meshID; ++_end);

	unsigned int _firstV = 2147483647, _lastV = 0;
	for (unsigned int i = _start; i <= _end; i++) {
		if (_indices[i].x > _lastV) {
			_lastV = _indices[i].x;
		}
		if (_indices[i].x < _firstV)
			_firstV = _indices[i].x;
	}


	//construct indices, onr index = one vertex in _vertices[]. _vertices.size()=_elementIndices.size(), but _positions.size() < _indices.size()
	Vertex _tmpV = { glm::vec3( 0), glm::vec3(0), glm::vec2(0) };
	for (unsigned int i = _start; i <= _end; i++)
	{
		_elementIndices.push_back(i - _start);//x:position ; y:texCoord ; z:noremal//-1:in obj,index stars from 1 but 0 in OpenGL
		_vertices.push_back(_tmpV);
	}

	for (unsigned int i = 0; i <= _end - _start; i++)
	{
		//putchar('!');
		_vertices[i]._positionVec = _positions[_indices[i + _start].x - 1];//-1:in obj,index stars from 1 but 0 in OpenGL
		_vertices[i]._textureCoord = _texCoords[_indices[i + _start].y - 1];
		_vertices[i]._normalVec = _normals[_indices[i + _start].z - 1];

	}

	Mesh* _newMesh = NULL;
	if( !(_newMesh = new Mesh(_vertices, _elementIndices, _name) ) )std::cout<<"ERROR: out of memory"<<std::endl;
	_meshs.push_back(_newMesh);
}

std::vector<Mesh*> Model::getMeshes()
{
	std::vector<Mesh*> _meshCopy;
	for (int i = 0; i < _meshs.size(); i++)
		_meshCopy.push_back(_meshs[i]);
	return _meshCopy;
}

/*	//construct empty vertices
	Vertex _tmpV;
	_tmpV._positionVec = glm::vec3(0); _tmpV._textureCoord = glm::vec2(0); _tmpV._normalVec = glm::vec3(0);
	unsigned int _firstV = 2147364847, _lastV = 0;
	for (unsigned int i = _start; i <= _end; i++) {
		if (_indices[i].x > _lastV) {
			_lastV = _indices[i].x;
		}
		if (_indices[i].x < _firstV)
			_firstV = _indices[i].x;
	}
	for (unsigned int i = _firstV; i <= _lastV; i++)_vertices.push_back(_tmpV);
		
	//construct indices
	for (unsigned int i = _start; i <= _end; i++)
	{
		_elementIndices.push_back(_indices[i].x - _firstV) ;//x:position ; y:texCoord ; z:noremal//-1:in obj,index stars from 1 but 0 in OpenGL
	}
	//construct vertices 
	for (unsigned int i = 0; i <= _end - _start; i++)
	{
		//putchar('!');
		if (_vertices[_elementIndices[i]]._positionVec == glm::vec3(0))_vertices[_elementIndices[i]]._positionVec = _positions[_indices[i + _start].x - 1];//-1:in obj,index stars from 1 but 0 in OpenGL
		if (_vertices[_elementIndices[i]]._textureCoord == glm::vec2(0) )_vertices[_elementIndices[i]]._textureCoord = _texCoords[_indices[i + _start].y -1];
		//cause repeated assign of
		if (_vertices[_elementIndices[i]]._normalVec == glm::vec3(0) )_vertices[_elementIndices[i]]._normalVec = _normals[_indices[i + _start].z -1];
		else {
			_vertices[_elementIndices[i]]._normalVec += _normals[_indices[i + _start].z - 1];
		}
		//HERE IS SOME BUGS: REPEATED ASSIGN OF _normalVec
	}
	for (unsigned int i = 0; i <= _end - _start; i++)_vertices[_elementIndices[i]]._normalVec = glm::normalize(_vertices[_elementIndices[i]]._normalVec);*/