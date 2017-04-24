#include "BoxTreeObject.hpp"

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
	rootNode->Construct(mesh.GetNumTriangles(), &mesh.GetTriangles());
}

bool BoxTreeObject::Intersect(const Ray & ray, Intersection & hit) {
	return true;
}