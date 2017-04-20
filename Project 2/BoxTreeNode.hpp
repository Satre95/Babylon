#pragma once

#include <glm/vec3.hpp>

#include "Object.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Triangle.hpp"

#define MAX_TRIANGLES_PER_BOX 8

class BoxTreeNode
{
public:
	BoxTreeNode();
	~BoxTreeNode();

	bool Intersect(const Ray & ray, Intersection & hit);
	void Construct(int count, Triangle ** tri);

private:
	glm::vec3 BoxMin, BoxMax;
	BoxTreeNode * child1, *child2;
	Triangle * Tri[MAX_TRIANGLES_PER_BOX];
};