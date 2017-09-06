//
//  Scene.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/11/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Scene.hpp"
#include "Volume.hpp"

bool Scene::Intersect(const Ray &ray, Intersection &hit) const {
	bool success = false;
	for (unsigned int i = 0; i < Objects.size(); i++)
		if (Objects[i]->Intersect(ray, hit)) {
			success = true;
		}

	return success;
}

void Scene::AddVolume(Volume & vol)
{
	Volumes.push_back(&vol);
}

std::vector<Volume *> Scene::IntersectVolumes(const Ray & ray) const
{
	std::vector<Volume *> hitVols;
	for (auto & aVol : Volumes)
	{
		if (aVol->Intersect(ray))
			hitVols.push_back(aVol);
	}
	return hitVols;
}