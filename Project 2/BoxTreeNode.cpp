#include "BoxTreeNode.hpp"

BoxTreeNode::BoxTreeNode()
{
	BoxMin = BoxMax = glm::vec3(0);
}

BoxTreeNode::~BoxTreeNode() {}

bool BoxTreeNode::Intersect(const Ray & ray, Intersection & hit) {
	return true;
}

void BoxTreeNode::Construct(int count, Triangle ** tri) {
}