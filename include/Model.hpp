#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Object.hpp"
#include "MeshObject.hpp"

class Model{
public:
	Model() {}
	Model(std::string path) { Load(path); }
	~Model();
	void Load(std::string path);
	const std::vector<MeshObject *> & GetMeshes() { return m_meshes; }
	size_t NumMeshes() { return m_meshes.size(); }
	void PrintInfo(std::ostream & stream);

private:
	void ProcessNode(aiNode *node, const aiScene *scene);
    MeshObject* ProcessMesh(aiMesh *mesh, const aiScene *scene);


	std::vector<MeshObject*> m_meshes;

};