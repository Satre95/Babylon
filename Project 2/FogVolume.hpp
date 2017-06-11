#pragma once

#include "Volume.hpp"
#include "IsotropicScatter.h"

//A homogenous, infinite volume
class FogVolume : Volume
{
public:
	FogVolume();

	float EvaluateRadiance(
		const glm::vec3 & pos,
		const Ray & ray,
		float step
	) override;

	bool Intersect(const Ray & ray) override;
private:
	float EvaluateExtinction(const glm::vec3 pos, const Ray & ray, float step);

	float absorptionCoeff;
	float outScatteringCoeff;
	float extinctionCoeff;
};