#pragma once

#include "Volume.hpp"
#include "IsotropicScatter.h"
#include "RayTrace.hpp"

//A homogenous, infinite volume
class FogVolume : Volume
{
public:
	FogVolume();

	void EvaluateRadiance(
		Color & incomingRad,
		const Scene & scene,
		const glm::vec3 & pos,
		float segment
	) override;

	bool Intersect(const Ray & ray) override;
protected:
	void EvaluateExtinction(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) override;
	void EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) override;
	void EvaluateInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) override;

	float EvaluateDirectInScattering(const Scene & scene, const glm::vec3 & pos, float step);
	float EvaluateIndirectInScattering(const Scene & scene, const glm::vec3 & pos, float step);

private:
	Color absorptionCoeff;
	Color scatteringCoeff;
	float extinctionCoeff;
	//TODO: define different extinction coeffs for each RGB val.
	//Color extinctionCoeff;
};