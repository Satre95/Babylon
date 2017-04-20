#include "BoxTreeObject.hpp"

BoxTreeObject::BoxTreeObject()
{
}

BoxTreeObject::~BoxTreeObject()
{
	delete rootNode;
}

void BoxTreeObject::Construct(MeshObject & mesh) {
}

bool BoxTreeObject::Intersect(const Ray & ray, Intersection & hit) {
	return true;
}