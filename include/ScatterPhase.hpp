#pragma once
#define GLM_FORCE_AVX2
#define GLM_FORCE_INLINE
#include <glm/vec3.hpp>
#include "Color.hpp"

class ScatterPhase
{
public:

	virtual ~ScatterPhase() {}

	virtual Color PhaseFn(float cosAngle) = 0;
};