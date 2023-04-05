#pragma once

#include <memory> 

#include "../base/application.h"
#include "../base/glsl_program.h"
#include "scene.h"
#include "simple_obj.h"
#include "geometry.h"
#include "useImgui.h"

class TestScene : public Application {
public:
	TestScene(const Options& options);

	~TestScene();

private:
	std::vector<Scene*> _sceneList;
	std::vector<Model*> _modelList;

	unsigned int _currentScene;

	ImguiSettings imguiSettings;

	std::vector<unsigned int> loadModelToScene(unsigned int _modelID, unsigned int _sceneID);//load model to scene, return object ID of each mesh.Parameter:(modelID, SceneID)
	unsigned int loadGeometryToScene(BasicGeometry* _geomerty, unsigned int _sceneID);//load a geometry to scene, return object ID of that geometry.Parameter:(ptrToGeo, SceneID)

	void useScene(unsigned int _sceneID);

	virtual void handleInput();

	virtual void renderFrame();
};
