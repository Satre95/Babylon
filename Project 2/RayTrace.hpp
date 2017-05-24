#pragma once

#include "Intersection.hpp"
#include "Material.hpp"
#include "Scene.hpp"
#include "Utilities.hpp"

class RayTrace {
public:
	RayTrace(Scene & scn, int maxD = 10) : scene(scn), maxDepth(maxD)
	{}

	void TraceRay(Intersection & hit, Ray & ray, int depth = 1);

private:
	Scene & scene;
	int maxDepth;
};