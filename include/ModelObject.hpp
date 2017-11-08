#pragma once

#include <vector>
#include <string>
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
	std::vector<MeshObject*> m_meshes;

};