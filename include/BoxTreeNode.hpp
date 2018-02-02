#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <array>
#include <boost/simd/pack.hpp>
#include <boost/simd/function/store.hpp>
#include <boost/simd/function/load.hpp>
#include "Object.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Triangle.hpp"

#define MAX_TRIANGLES_PER_BOX 10
#define SIMD_TREE_TRAVERSAL

class BoxTreeNode
{
public:
	BoxTreeNode();
	~BoxTreeNode();

	bool Intersect(const Ray & ray, Intersection & hit, int & depth);
	void Construct(int count, Triangle ** tri);
	void CountTriangles(int & count);

	static int splitCount;
private:

	///Tests againsts children in order of distance.
	bool IntersectChildren(const Ray & ray, Intersection & hit, int & depth);

	///Tests against the triangles of this volume, if any.
	bool IntersectTriangles(const Ray & ray, Intersection & hit);

	///Tests against the bounding volume of this node only.
	bool IntersectVolume(const Ray & ray, Intersection & hit);

    // First member is min, second is max
    glm::vec3 BoxMin, BoxMax;
    using pack_t = boost::simd::pack<float, 8>;
    // Useful for SIMD processing.
    pack_t Box;
    
	BoxTreeNode * child1, *child2;
	std::array<Triangle*, MAX_TRIANGLES_PER_BOX> Tri;
	int numTriangles = -1; //Used only if node is a leaf
	//Triangle * Tri[MAX_TRIANGLES_PER_BOX];
};
