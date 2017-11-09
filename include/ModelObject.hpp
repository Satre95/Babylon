#pragma once

#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Object.hpp"
#include "MeshObject.hpp"

class ModelObject: public Object {
public:
	ModelObject() {}
	ModelObject(std::string path) { Load(path); }
	virtual ~ModelObject() override;
	virtual bool Intersect(const Ray &ray, Intersection &hit) override;
	void Load(std::string path);

private:
	void ProcessNode(aiNode *node, const aiScene *scene);
    MeshObject* ProcessMesh(aiMesh *mesh, const aiScene *scene);


	std::vector<MeshObject*> m_meshes;

};