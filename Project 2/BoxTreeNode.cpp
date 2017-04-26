#include "BoxTreeNode.hpp"
#include <iterator>
#include <algorithm>
#include <iostream>

BoxTreeNode::BoxTreeNode()
{
	child1 = child2 = nullptr;
	BoxMin = glm::vec3(1e15f);
	BoxMax = glm::vec3(-1e15f);
	Tri.fill(nullptr);
}

BoxTreeNode::~BoxTreeNode() {
	if (child1)
		delete child1;
	if (child2)
		delete child2;
}

bool BoxTreeNode::Intersect(const Ray & ray, Intersection & hit) {
	if (IntersectVolume(ray, hit)) {
		return IntersectChildren(ray, hit);
	}

	return false;
}

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
	if (t_min > t_max || t_max < 0)
		return false;

	//Compute intersection point on this node's bounding volume.
	glm::vec3 intersection;
	if (t_min >= 0)
		intersection = p + t_min * d;
	else
		intersection = p + t_max * d;
	//Compare distance with already set value
	auto dist = glm::length(intersection);
	if (dist < hit.HitDistance) {
		hit.HitDistance = dist;
		hit.Position = intersection;
	}
	return true;
}

bool BoxTreeNode::IntersectChildren(const Ray & ray, Intersection & hit) {
	//If leaf node, test against children
	if (!child1 && !child2)
		return IntersectTriangles(ray, hit);

	//Test against child VOLUMES
	BoxTreeNode * children[2];
	children[0] = child1;
	children[1] = child2;
	Intersection volHit[2];
	for (int i = 0; i < 2; i++) {
		volHit[i].HitDistance = hit.HitDistance;
		children[i]->IntersectVolume(ray, volHit[i]);
	}

	if (volHit[0].HitDistance > volHit[1].HitDistance) {
		std::swap(volHit[0], volHit[1]);
		std::swap(children[0], children[1]);
	}

	bool success = false;
	for (int i = 0; i < 2; i++) {
		if (volHit[i].HitDistance < hit.HitDistance)
			if (children[i]->IntersectChildren(ray, hit))
				success = true;
	}

	return success;
}

bool BoxTreeNode::IntersectTriangles(const Ray & ray, Intersection & hit) {
	Intersection localHit;
	bool intersection = false;
	//If we are a leaf node, test against our triangles.
	for (int i = 0; i < numTriangles; i++) {
		Intersection tempHit;
		bool tempI = Tri.at(i)->Intersect(ray, tempHit);
		intersection = intersection || tempI;
		if (tempI && tempHit.HitDistance < localHit.HitDistance)
			localHit = tempHit;
	}

	hit = (localHit.HitDistance < hit.HitDistance) ? localHit : hit;
	return intersection;
}

void BoxTreeNode::Construct(int count, Triangle ** tri) {
	// Compute BoxMin & BoxMax to fit around all tri’s
	auto tempTri = *tri;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (tempTri->GetVertex(j).Position.x < BoxMin.x)
				BoxMin.x = tempTri->GetVertex(j).Position.x;

			if (tempTri->GetVertex(j).Position.y < BoxMin.y)
				BoxMin.y = tempTri->GetVertex(j).Position.y;

			if (tempTri->GetVertex(j).Position.z < BoxMin.z)
				BoxMin.z = tempTri->GetVertex(j).Position.z;

			if (tempTri->GetVertex(j).Position.x > BoxMax.x)
				BoxMax.x = tempTri->GetVertex(j).Position.x;

			if (tempTri->GetVertex(j).Position.y > BoxMax.y)
				BoxMax.y = tempTri->GetVertex(j).Position.y;

			if (tempTri->GetVertex(j).Position.z > BoxMax.z)
				BoxMax.z = tempTri->GetVertex(j).Position.z;
		}
		tempTri++;
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
	splitPlaneLoc = (BoxMax.x + BoxMin.x) / 2.0f; //Default to X-axis
	planeNormalAxis = 0;

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
	Triangle **leftGroup = new Triangle*[count];
	Triangle **rightGroup = new Triangle*[count];
	int leftCount = 0, rightCount = 0;

	// Place triangles into group 1 or group 2
	tempTri = *tri;
	for (int i = 0; i < count; i++) {
		// Compute center of triangle & determine which side of splitting plane
		auto center = tempTri->GetCenter();
		// Add to appropriate group
		if (center[planeNormalAxis] > splitPlaneLoc)
			leftGroup[leftCount++] = tempTri;
		else
			rightGroup[rightCount++] = tempTri;

		tempTri++;
	}
	// Check if either group is empty. If so, move (at least) 1 triangle into that group
	if (leftCount == 0) {
		leftGroup[0] = rightGroup[rightCount - 1];
		leftCount = 1;
		rightCount -= 1;
	}
	else if (rightCount == 0) {
		rightGroup[0] = leftGroup[leftCount - 1];
		rightCount = 1;
		leftCount -= 1;
	}

	if (BoxMax.x == -1e15f || BoxMax.y == -1e15f || BoxMax.z == -1e15f)
		std::cerr << "Incorrect BoxMax!" << std::endl;
	if (BoxMin.x == 1e15f || BoxMin.y == 1e15f || BoxMin.z == 1e15f)
		std::cerr << "Incorrect BoxMin!" << std::endl;

	// Recursively build sub-trees
	child1 = new BoxTreeNode; child2 = new BoxTreeNode;
	child1->Construct(leftCount, leftGroup);
	child2->Construct(rightCount, rightGroup);

	// Free up arrays
	delete[]leftGroup; delete[]rightGroup;
}