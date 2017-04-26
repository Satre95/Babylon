#pragma once

#include <glm/vec3.hpp>

#include "Object.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Triangle.hpp"
#include <array>

#define MAX_TRIANGLES_PER_BOX 10

class BoxTreeNode
{
public:
	BoxTreeNode();
	~BoxTreeNode();

	bool Intersect(const Ray & ray, Intersection & hit);
	void Construct(int count, Triangle ** tri);
	void CountTriangles(int & count);

	int depth = 0;
	static int splitCount;
private:

	///Tests againsts children in order of distance.
	bool IntersectChildren(const Ray & ray, Intersection & hit);

	///Tests against the triangles of this volume, if any.
	bool IntersectTriangles(const Ray & ray, Intersection & hit);

	///Tests against the bounding volume of this node only.
	bool IntersectVolume(const Ray & ray, Intersection & hit);

	glm::vec3 BoxMin, BoxMax;
	BoxTreeNode * child1, *child2;
	float splitPlaneLoc = std::numeric_limits<float>::min();
	int planeNormalAxis = 0;
	std::array<Triangle*, MAX_TRIANGLES_PER_BOX> Tri;
	int numTriangles = -1; //Used only if node is a leaf
	//Triangle * Tri[MAX_TRIANGLES_PER_BOX];
};