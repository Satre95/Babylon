////////////////////////////////////////
// Ray.hpp
////////////////////////////////////////

#pragma once

#define GLM_FORCE_AVX2
#define GLM_FORCE_INLINE
#include "glm/glm.hpp"

////////////////////////////////////////////////////////////////////////////////

class Ray {
public:
	Ray() {}
	Ray(glm::vec3 o, glm::vec3 d) : Origin(o), Direction(d) {}
	glm::vec3 Origin;
	glm::vec3 Direction;
};

////////////////////////////////////////////////////////////////////////////////