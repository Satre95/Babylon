////////////////////////////////////////
// Intersection.h
////////////////////////////////////////

#pragma once

#include "Color.hpp"
#include "glm/glm.hpp"
class Material;
#define HIT_THRESHOLD 1e-10

////////////////////////////////////////////////////////////////////////////////

class Intersection {
public:
	Intersection() { HitDistance = 1e10; Mtl = 0; }

public:
	// Ray intersection data
	float HitDistance;
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TangentU;
	glm::vec3 TangentV;

	Material *Mtl;

	// Shaded color
	Color Shade;
};

////////////////////////////////////////////////////////////////////////////////