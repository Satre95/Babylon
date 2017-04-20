//
//  Scene.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/11/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Scene.hpp"

bool Scene::Intersect(const Ray &ray, Intersection &hit) {
	Intersection tempHit;

	bool success = false;
	for (unsigned int i = 0; i < Objects.size(); i++)
		if (Objects[i]->Intersect(ray, tempHit)) {
			if (tempHit.HitDistance < hit.HitDistance && tempHit.HitDistance > HIT_THRESHOLD)
				hit = tempHit;
			success = true;
		}

	return success;
}