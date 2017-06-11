#include "FogVolume.hpp"

FogVolume::FogVolume()
{
	scatterPhase = new IsotropicScatter();
}

void FogVolume::Evaluate(
	const glm::vec3 & pos,
	float & absorptionCoeff,
	float & scatteringCoeff,
	float & emissionPhaseFn,
	float & scatteringPhaseFn
)
{
}