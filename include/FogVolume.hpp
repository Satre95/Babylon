#pragma once

#define GLM_FORCE_AVX2
#define GLM_FORCE_INLINE
#include "Volume.hpp"
#include "IsotropicScatter.hpp"
#include "LorenzMieScatter.hpp"
#include "RayTrace.hpp"

//A homogenous, infinite volume
class FogVolume : public Volume
{
public:
	FogVolume();

	void EvaluateRadiance(
		Color & incomingRad,
		const Ray & incomingRay,
		const RayTrace & rayTracer,
		const Scene & scene,
		const glm::vec3 & pos, //x
		int depth
	) override;

	void SetAbsroptionCoeff(Color & abIn);
	void SetScatteringCoeff(Color & scIn);
	void SetAbsroptionCoeff(Color && abIn);
	void SetScatteringCoeff(Color && scIn);
	void SetMarchSamples(int marchIn) { numMarchSamples = marchIn; }

	bool Intersect(const Ray & ray) override;

protected:
	void EvaluateExtinction(Color & incomingRad, const glm::vec3 & pos, float dist);

	void EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float dist);

	void EvaluateInScattering(Color & incomingRad, const Ray & incomingRay, const RayTrace & rayTracer, const Scene & scene, const glm::vec3 & pos, float step, int depth);

	void EvaluateDirectInScattering(Color & incomingRad, const Ray & incomingRay, const Scene & scene, const glm::vec3 & pos, float step);

	void EvaluateIndirectInScattering(Color & incomingRad, const Ray & incomingRay, const RayTrace & rayTracer, const Scene & scene, const glm::vec3 & pos, float step, int depth);

private:
	Color absorptionCoeff; //sigma_a
	Color scatteringCoeff; //sigma_s
	Color extinctionCoeff; //sigma_t

	int numMarchSamples = 3;
};