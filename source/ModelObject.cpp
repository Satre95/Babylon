#include "ModelObject.hpp"

ModelObject::~ModelObject() {
	for(auto aMesh: m_meshes)
		delete aMesh;
	m_meshes.clear();
}

void ModelObject::Load(std::string path) {

}

// TODO: Implement 
bool ModelObject::Intersect(const Ray &ray, Intersection &hit) {
	return true;
}