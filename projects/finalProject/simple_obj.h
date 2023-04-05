#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



//define a vertex
struct Vertex {
	glm::vec3 _positionVec;
	glm::vec3 _normalVec;
	glm::vec2 _textureCoord;
};


//an "object" in the scene
class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex>_vertices, std::vector<unsigned int>_indices, std::string _name);//load mesh data
	~Mesh();

	void exportMesh(std::string _path, std::string _name);//export mmesh data, no texture/material contained.

	std::string getName();
	
	unsigned int getVao(void);
	unsigned int getVbo(void);
	unsigned int getEbo(void);

	unsigned int getVertexSize(void);
	unsigned int getIndexSize(void);

private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::string _name;
	unsigned int _vao = 0;
	unsigned int _vbo = 0;
	unsigned int _ebo = 0;

	void setup();//generate vao vbo ebo, et.al
};

//A model is what an .obj file contains. It may contain more than one mesh.
class Model
{
public:
	Model(std::string _path);//give it a .obj path, it will load that .obj
	~Model();
	//void exportObj(void);
	std::vector<Mesh*> getMeshes();
private:
	std::vector<Mesh*> _meshs;
	std::string _directory; //path to the file
	std::string _name;
	//variables to store an obj file(Although export the whole model to obj is just a meaningless "copy"...)
	std::vector<std::string> _meshNames;
	std::vector<glm::vec3> _positions;//w is its meshID
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texCoords;
	std::vector<glm::vec4> _indices;// about the ith drawn vertor: position, texCoord, normal, meshID

	void load(std::string _path);
	void generateAMesh(std::string _name, unsigned int _meshID);
};
