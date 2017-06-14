////////////////////////////////////////
// Intersection.h
////////////////////////////////////////

#pragma once

#include "Color.hpp"
#include "glm/glm.hpp"
class Material;
#define HIT_THRESHOLD 1e-10
#define MAX_HIT_DISTANCE 1e8

////////////////////////////////////////////////////////////////////////////////

class Intersection {
public:
	Intersection() : HitDistance(MAX_HIT_DISTANCE), Mtl(nullptr) {}

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