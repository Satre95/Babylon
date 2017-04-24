#include "BoxTreeNode.hpp"
#include <iterator>
#include <algorithm>

BoxTreeNode::BoxTreeNode()
{
	child1 = child2 = nullptr;
	BoxMin = glm::vec3(100000000.0f);
	BoxMax = glm::vec3(-100000000.0f);
}

BoxTreeNode::~BoxTreeNode() {
	if (child1)
		delete child1;
	if (child2)
		delete child2;
}

bool BoxTreeNode::Intersect(const Ray & ray, Intersection & hit) {
	return true;
}

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
		return;
	}

	float splitPlaneLoc = std::numeric_limits<float>::min();
	int planeNormalAxis = 0;
	// Determine largest box dimension x, y, or z
	// Compute splitting plane halfway along largest dimension
	if (BoxMax.x - BoxMin.x > splitPlaneLoc)
	{
		splitPlaneLoc = BoxMax.x - BoxMin.x;
		planeNormalAxis = 0;
	}
	if (BoxMax.y - BoxMin.y > splitPlaneLoc)
	{
		splitPlaneLoc = BoxMax.y - BoxMin.y;
		planeNormalAxis = 1;
	}
	if (BoxMax.z - BoxMin.z > splitPlaneLoc) 
	{
		splitPlaneLoc = BoxMax.z - BoxMin.z;
		planeNormalAxis = 2;
	}
	// Allocate two new temporary arrays
	Triangle **leftGroup = new Triangle*[count]; 
	Triangle **rightGroup = new Triangle*[count];
	int leftCount = 0, rightCount = 0;

	// Place triangles into group 1 or group 2
	for (int i = 0; i < count; i++) {
		// Compute center of triangle & determine which side of splitting plane
		auto center = tri[i]->GetCenter();
		// Add to appropriate group
		if (center[planeNormalAxis] > splitPlaneLoc)
			leftGroup[leftCount++] = tri[i];
		else
			rightGroup[rightCount++] = tri[i];
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

	// Recursively build sub-trees
	child1 = new BoxTreeNode; child2 = new BoxTreeNode;
	child1->Construct(leftCount, leftGroup);
	child2->Construct(rightCount, rightGroup);

	// Free up arrays
	delete[]leftGroup; delete[]rightGroup;
}