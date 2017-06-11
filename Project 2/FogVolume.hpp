#pragma once

#include "Volume.hpp"
#include "IsotropicScatter.h"

//A homogenous, infinite volume
class FogVolume : Volume
{
public:
	FogVolume();

	void Evaluate(
		const glm::vec3 & pos,
		float & absorptionCoeff,
		float & scatteringCoeff,
		float & emissionPhaseFn,
		float & scatteringPhaseFn
	) override;
private:
	float absorptionCoeff;
	float outScatteringCoeff;
	float extinctionCoeff;
};