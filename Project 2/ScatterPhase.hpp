#pragma once
#include <glm/vec3.hpp>
#include "Color.hpp"

class ScatterPhase
{
public:

	virtual ~ScatterPhase() {}

	virtual Color PhaseFn(float cosAngle) = 0;
};