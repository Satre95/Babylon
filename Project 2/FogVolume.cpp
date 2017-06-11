#include "FogVolume.hpp"

FogVolume::FogVolume()
{
	scatterPhase = new IsotropicScatter();
}

float FogVolume::EvaluateRadiance(const glm::vec3 & pos, const Ray & ray, float step)
{
}

bool FogVolume::Intersect(const Ray & ray)
{
	return true; //Infinite volume
}

float FogVolume::EvaluateExtinction(const glm::vec3 pos, const Ray & ray, float step)
{
	return glm::pow(glm::e<float>(), extinctionCoeff * step);
}