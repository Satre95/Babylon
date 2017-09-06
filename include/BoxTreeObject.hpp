#pragma once

#include "Object.hpp"
#include "MeshObject.hpp"
#include "InstanceObject.hpp"
#include "BoxTreeNode.hpp"

class BoxTreeObject : public Object
{
public:
	BoxTreeObject();
	~BoxTreeObject();

	void Construct(MeshObject & mesh);
	bool Intersect(const Ray & ray, Intersection & hit);
	void CountTriangles(int & count);
private:
	BoxTreeNode * rootNode;
};