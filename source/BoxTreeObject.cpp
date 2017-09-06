#include "BoxTreeObject.hpp"
#include <iostream>
BoxTreeObject::BoxTreeObject()
{
	rootNode = nullptr;
}

BoxTreeObject::~BoxTreeObject()
{
	delete rootNode;
}

void BoxTreeObject::Construct(MeshObject & mesh) {
	if (!rootNode)
		rootNode = new BoxTreeNode();
	rootNode->Construct(mesh.GetNumTriangles(), mesh.GetTrianglePtrs());
}

bool BoxTreeObject::Intersect(const Ray & ray, Intersection & hit) {
	if (!rootNode)
		return false;
	int depth = 0;
	return rootNode->Intersect(ray, hit, depth);
}
void BoxTreeObject::CountTriangles(int & count) {
	if (!rootNode) return;

	rootNode->CountTriangles(count);
}