//
//  Scene.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/11/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Scene.hpp"

bool Scene::Intersect(const Ray &ray, Intersection &hit) {
	bool success = false;
	for (unsigned int i = 0; i < Objects.size(); i++)
		if (Objects[i]->Intersect(ray, hit)) {
			success = true;
		}

	return success;
}