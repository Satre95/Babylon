#pragma once

#define GLM_FORCE_AVX2
#define GLM_FORCE_INLINE
#include <glm/gtc/constants.hpp>

#include "ScatterPhase.hpp"

class LorenzMieScatter : public ScatterPhase
{
public:
	LorenzMieScatter(float zIn = 8) : z(zIn) {}

	Color PhaseFn(float cosAngle) override
	{
		return Color(const1_4Pi *
			(0.5f + 0.5f * (z + 1.f) * powf((1.f + cosAngle) * 0.5f, z))
		);
	}
private:
	float z;
	float const1_4Pi = 1.f / (4.f * glm::pi<float>());
};