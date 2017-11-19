#pragma once

#include "Intersection.hpp"
#include "Material.hpp"
#include "Scene.hpp"
#include "Utilities.hpp"

class RayTrace {
public:
	RayTrace(const Scene & scn, int maxD = 15) : scene(scn), maxDepth(maxD)
	{}

	void TraceRay(Intersection & hit, const Ray & ray, int depth = 1) const;

private:
	const Scene & scene;
	int maxDepth;
};
