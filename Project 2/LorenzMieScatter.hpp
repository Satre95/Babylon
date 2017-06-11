#pragma once

#include <glm/gtc/constants.hpp>

#include "ScatterPhase.hpp"

class LorenzMieScatter : ScatterPhase
{
public:
	LorenzMieScatter(float zIn = 8) : z(zIn) {}

protected:
	float PhaseFn(float cosAngle) override
	{
		return const1_4Pi *
			(0.5f + 0.5f * (z + 1.f) * powf((1.f + cosAngle) * 0.5f, z));
	}
private:
	float z;
	float const1_4Pi = 1.f / (4.f * glm::pi<float>());
};