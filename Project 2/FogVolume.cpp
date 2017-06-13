#include "FogVolume.hpp"

FogVolume::FogVolume()
{
	scatterPhase = new IsotropicScatter();
}

void FogVolume::EvaluateRadiance(
	Color & incomingRad,
	const Scene & scene,
	const glm::vec3 & pos,
	float segment)
{
	//1. Add Emission scaled by segment
	EvaluateEmission(incomingRad, scene, pos, segment);

	//2. Add In scattering, scaled by segment
	EvaluateInScattering(incomingRad, scene, pos, segment);

	//3. Multiply by extinction
	EvaluateExtinction(incomingRad, scene, pos, segment);
}

bool FogVolume::Intersect(const Ray & ray)
{
	return true; //Infinite volume
}

void FogVolume::EvaluateExtinction(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{
	//TODO: Store this in a var instead of recalculating each tick.

	//Calculate each spectrum individually.
	float rScale = glm::pow(glm::e<float>(), -1.f * extinctionCoeff.GetRed() * step);
	float gScale = glm::pow(glm::e<float>(), -1.f * extinctionCoeff.GetGreen() * step);
	float bScale = glm::pow(glm::e<float>(), -1.f * extinctionCoeff.GetBlue() * step);

	incomingRad.Multiply(Color(rScale, gScale, bScale)); //Homogenous vol, so constant.
}

void FogVolume::EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{} //Do nothing, as fog does not emit light.

void FogVolume::EvaluateInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{
	//1. compute in scattering from indirect sources around the environment. (only one)
	EvaluateIndirectInScattering(incomingRad, scene, pos, step);
	//2. compute in scattering from direct lights.
	EvaluateDirectInScattering(incomingRad, scene, pos, step);
}

void FogVolume::EvaluateDirectInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{
	for (int i = 0; i < scene.GetNumLights(); i++)
	{
		Light & aLight = scene.GetLight(i);
		//Shoot a ray to the light
		glm::vec3 toLight, lightPos;
		Color illuminatedColor;
		//Use the Illuminate fn to fetch light vars
		aLight.Illuminate(pos, illuminatedColor, toLight, lightPos);
		Ray shadowRay;
		shadowRay.Origin = pos;
		shadowRay.Direction = glm::normalize(lightPos - pos);
		Intersection shadowHit;

		//If ray doesn't hit anything, then test for vols.
		if (scene.Intersect(shadowRay, shadowHit) == false)
		{
			//If reached this far, can assume ray that got us here intersects this volume.

			//Homogenous volume, so attenuate by length of ray to light through vol
			float length = glm::length(lightPos - pos);

			//Reuse the extinction to calculate the attenuation of the light through the volume.
			EvaluateExtinction(illuminatedColor, scene, pos, length);
			incomingRad.Add(illuminatedColor);
		}
	}
}

//TODO: Figure this shit out.
//Note: Matteo thinks that this should be some very small constant
void FogVolume::EvaluateIndirectInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{} //Do nothing, for now.