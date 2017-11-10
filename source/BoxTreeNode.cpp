#include "BoxTreeNode.hpp"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>

int BoxTreeNode::splitCount = 0;
//Use an alias for the boost::simd namespace
namespace bs = boost::simd;
////////////////////////////////////////////////////////////////////////////////

BoxTreeNode::BoxTreeNode()
{
	child1 = child2 = nullptr;
	BoxMin = glm::vec3(1e15f);
	BoxMax = glm::vec3(-(1e15f));
	Tri.fill(nullptr);
}

////////////////////////////////////////////////////////////////////////////////

BoxTreeNode::~BoxTreeNode() {
	if (child1)
		delete child1;
	if (child2)
		delete child2;
}

////////////////////////////////////////////////////////////////////////////////

bool BoxTreeNode::Intersect(const Ray & ray, Intersection & hit, int & depth) {
	return IntersectChildren(ray, hit, depth);
}

////////////////////////////////////////////////////////////////////////////////

bool BoxTreeNode::IntersectVolume(const Ray & ray, Intersection & hit) {
	//test if ray intersects our bounding box.
	auto & a = BoxMin;
	auto & b = BoxMax;
	auto & p = ray.Origin;
	auto & d = ray.Direction;

	glm::vec3 t_1, t_2;
	t_1 = (a - p) / d;
	t_2 = (b - p) / d;

	float t_min = glm::max(glm::min(t_1.x, t_2.x), glm::min(t_1.y, t_2.y));
	t_min = glm::max(t_min, glm::min(t_1.z, t_2.z));

	float t_max = glm::min(glm::max(t_1.x, t_2.x), glm::max(t_1.y, t_2.y));
	t_max = glm::min(t_max, glm::max(t_1.z, t_2.z));

	//No intersection.
	if (t_min > t_max || t_max < 0) {
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool BoxTreeNode::IntersectChildren(const Ray & ray, Intersection & hit, int & depth) {
	//depth++;
	//if (depth == 55)
		//return true;
	//If leaf node, test against triangles
	if (child1 == nullptr && child2 == nullptr)
		return IntersectTriangles(ray, hit);

	bool success = false;
	//Test against child VOLUMES, then recurse inside.
	if (child1->IntersectVolume(ray, hit)) {
		if (child1->IntersectChildren(ray, hit, depth))
			success = true;
	}

	if (child2->IntersectVolume(ray, hit)) {
		if (child2->IntersectChildren(ray, hit, depth))
			success = true;
	}

	return success;
}

////////////////////////////////////////////////////////////////////////////////

bool BoxTreeNode::IntersectTriangles(const Ray & ray, Intersection & hit) {
	bool intersection = false;
	//If we are a leaf node, test against our triangles.
	for (int i = 0; i < numTriangles; i++) {
		intersection = intersection || Tri.at(i)->Intersect(ray, hit);
	}
	return intersection;
}

////////////////////////////////////////////////////////////////////////////////

void BoxTreeNode::Construct(int count, Triangle ** tri) {
	// Compute BoxMin & BoxMax to fit around all tri’s
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (tri[i]->GetVertex(j).Position.x < BoxMin.x)
				BoxMin.x = tri[i]->GetVertex(j).Position.x;

			if (tri[i]->GetVertex(j).Position.y < BoxMin.y)
				BoxMin.y = tri[i]->GetVertex(j).Position.y;

			if (tri[i]->GetVertex(j).Position.z < BoxMin.z)
				BoxMin.z = tri[i]->GetVertex(j).Position.z;

			if (tri[i]->GetVertex(j).Position.x > BoxMax.x)
				BoxMax.x = tri[i]->GetVertex(j).Position.x;

			if (tri[i]->GetVertex(j).Position.y > BoxMax.y)
				BoxMax.y = tri[i]->GetVertex(j).Position.y;

			if (tri[i]->GetVertex(j).Position.z > BoxMax.z)
				BoxMax.z = tri[i]->GetVertex(j).Position.z;
		}
	}

	// Check if this is a leaf node
	if (count <= MAX_TRIANGLES_PER_BOX) {
		// Copy triangles to BoxTreeNode’s Tri array
		std::copy(tri, tri + count, std::begin(Tri));
		numTriangles = count;
		return;
	}

	// Determine largest box dimension x, y, or z
	// Compute splitting plane halfway along largest dimension
	float dist = BoxMax.x - BoxMin.x;
	float splitPlaneLoc = (BoxMax.x + BoxMin.x) / 2.0f; //Default to X-axis
	int planeNormalAxis = 0;

	if (BoxMax.y - BoxMin.y > dist)
	{
		splitPlaneLoc = (BoxMax.y + BoxMin.y) / 2.0f;
		dist = BoxMax.y - BoxMin.y;
		planeNormalAxis = 1;
	}
	if (BoxMax.z - BoxMin.z > dist)
	{
		splitPlaneLoc = (BoxMax.z + BoxMin.z) / 2.0f;
		dist = BoxMax.z - BoxMin.z;
		planeNormalAxis = 2;
	}
	// Allocate two new temporary arrays
	Triangle ** leftGroup = new Triangle*[count];
	Triangle ** rightGroup = new Triangle*[count];
	int leftCount = 0, rightCount = 0;

	// Place triangles into group 1 or group 2
	for (int i = 0; i < count; i++) {
		// Compute center of triangle & determine which side of splitting plane
		glm::vec3 center = tri[i]->GetCenter();

		// Add to appropriate group
		if (center[planeNormalAxis] > splitPlaneLoc)
			leftGroup[leftCount++] = tri[i];
		else
			rightGroup[rightCount++] = tri[i];
	}
	// Check if either group is empty. If so, move (at least) 1 triangle into that group
	if (leftCount == 0) {
		leftGroup[0] = rightGroup[rightCount - 1];
		rightGroup[rightCount - 1] = nullptr;
		leftCount = 1;
		rightCount -= 1;
	}
	else if (rightCount == 0) {
		rightGroup[0] = leftGroup[leftCount - 1];
		leftGroup[leftCount - 1] = nullptr;
		rightCount = 1;
		leftCount -= 1;
	}

	// Recursively build sub-trees
	child1 = new BoxTreeNode; child2 = new BoxTreeNode;
	child1->Construct(leftCount, leftGroup);
	child2->Construct(rightCount, rightGroup);

	// Free up arrays
	delete[]leftGroup; delete[]rightGroup;
}

void BoxTreeNode::CountTriangles(int & count) {
	if (!child1 && !child2)
		count += numTriangles;
	else {
		child1->CountTriangles(count);
		child2->CountTriangles(count);
	}
}