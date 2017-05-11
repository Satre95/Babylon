#pragma once

#include "Intersection.hpp"
#include "Material.hpp"
#include "Scene.hpp"
#include "Utilities.hpp"

class RayTrace {
public:
	static void TraceRay(Intersection & hit, Ray & ray, Scene & scn);
};