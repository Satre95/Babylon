#pragma once

#include "Volume.hpp"
#include "IsotropicScatter.h"
#include "RayTrace.hpp"

//A homogenous, infinite volume
class FogVolume : public Volume
{
public:
	FogVolume();

	void EvaluateRadiance(
		Color & incomingRad,
		const Scene & scene,
		const glm::vec3 & pos,
		float segment
	) override;

	void SetAbsroptionCoeff(Color & abIn) { absorptionCoeff = abIn; extinctionCoeff = scatteringCoeff + abIn; }
	void SetScatteringCoeff(Color & scIn) { scatteringCoeff = scIn; extinctionCoeff = absorptionCoeff + scIn; }
	bool Intersect(const Ray & ray) override;

protected:
	void EvaluateExtinction(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) override;
	void EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) override;
	void EvaluateInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) override;

	void EvaluateDirectInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step);
	void EvaluateIndirectInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step);

private:
	Color absorptionCoeff;
	Color scatteringCoeff;
	Color extinctionCoeff;
};