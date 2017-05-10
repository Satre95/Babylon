//
//  InstanceObject.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "InstanceObject.hpp"

InstanceObject::InstanceObject() {
	M = glm::mat4(1.0f);
	M_Inverse = glm::mat4(1.0f);
}

InstanceObject::InstanceObject(glm::mat4 & mtx) {
	M = mtx;
	M_Inverse = glm::inverse(mtx);
}

InstanceObject::InstanceObject(Object & obj) {
	child = &obj;
	M = glm::mat4(1.0f);
	M_Inverse = glm::mat4(1.0f);
}

bool InstanceObject::Intersect(const Ray &ray, Intersection &hit) {
	//Covnert ray to object space.
	Ray ray2;
	ray2.Origin = glm::vec3(M_Inverse * glm::vec4(ray.Origin, 1));
	ray2.Direction = glm::vec3(M_Inverse * glm::vec4(ray.Direction, 0));

	//Perform intersection test.
	if (child->Intersect(ray2, hit) == false)
		return false;

	//Set data in world space
	hit.Position = glm::vec3(M * glm::vec4(hit.Position, 1));
	hit.Normal = glm::vec3(M * glm::vec4(hit.Normal, 0));
	hit.HitDistance = glm::distance(ray.Origin, hit.Position);
	//Assign material if we have one
	if (mtl != nullptr) hit.Mtl = mtl;
	return true;
}

InstanceObject::~InstanceObject() {}