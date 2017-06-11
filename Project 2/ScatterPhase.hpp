#pragma once
#include <glm/vec3.hpp>

class ScatterPhase
{
public:

	virtual ~ScatterPhase() {}

protected:
	virtual float PhaseFn(float cosAngle) = 0;
};