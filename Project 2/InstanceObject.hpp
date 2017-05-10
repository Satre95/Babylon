//
//  InstanceObject.hpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#pragma once

#include "MeshObject.hpp"

class InstanceObject : public Object {
public:
	InstanceObject();
	InstanceObject(glm::mat4 & mtx);
	InstanceObject(Object & obj);
	~InstanceObject();

	bool Intersect(const Ray &ray, Intersection &hit);

	void SetChild(Object & obj) { child = &obj; }
	void SetMatrix(glm::mat4 & mtx) {
		M = mtx;
		M_Inverse = glm::inverse(mtx);
	}

	void SetMaterial(Material * mat) { mtl = mat; }

	glm::mat4 GetMatrix() { return M; }
	glm::mat4 GetInverseMatrix() { return M_Inverse; }
	Object* GetChild() { return child; }
private:
	Material * mtl = nullptr;
	glm::mat4 M;
	glm::mat4 M_Inverse;
	Object * child;
};