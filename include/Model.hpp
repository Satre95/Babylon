#pragma once

#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Object.hpp"
#include "MeshObject.hpp"

class ModelObject{
public:
	ModelObject() {}
	ModelObject(std::string path) { Load(path); }
	~ModelObject();
	void Load(std::string path);
	const std::vector<MeshObject *> & GetMeshes() { return m_meshes; }

private:
	void ProcessNode(aiNode *node, const aiScene *scene);
    MeshObject* ProcessMesh(aiMesh *mesh, const aiScene *scene);


	std::vector<MeshObject*> m_meshes;

};