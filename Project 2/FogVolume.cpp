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
	incomingRad.Scale(segment);

	//2. Add In scattering, scaled by segment
	EvaluateInScattering(incomingRad, scene, pos, segment);
	incomingRad.Scale(segment);

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
	incomingRad.Scale(glm::pow(glm::e<float>(), extinctionCoeff * step)); //Homogenous vol, so constant.
}

void FogVolume::EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{} //Do nothing, as fog does not emit light.

void FogVolume::EvaluateInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{
	//1. compute in scattering from indirect sources around the environment. (only one)

	//2. compute in scattering from direct lights.

	//3. add it up
}

float FogVolume::EvaluateDirectInScattering(const Scene & scene, const glm::vec3 & pos, float segment)
{
	float radiance = 0;
	for (int i = 0; i < scene.GetNumLights(); i++)
	{
		Light & aLight = scene.GetLight(i);
		//Shoot a ray to the light
		glm::vec3 toLight, lightPos;
		//Use the Illuminate fn to fetch light vars
		aLight.Illuminate(pos, Color(), toLight, lightPos);
		Ray shadowRay;
		shadowRay.Origin = pos;
		shadowRay.Direction = glm::normalize(lightPos - pos);
		Intersection shadowHit;

		//If ray doesn't hit anything, then test for vols.
		if (scene.Intersect(shadowRay, shadowHit) == false)
		{
			//Get the volumes this ray intersects with.
			auto vols = scene.IntersectVolumes(shadowRay);

			//Homogenous volume, so attenuate by length of ray to light through vol
			float length = glm::length(lightPos - pos);

			//TODO: Evaluate extinction and multiply that into the color val
		}
	}

	return 0;
}

//Note: Matteo thinks that this should be some very small constant
float FogVolume::EvaluateIndirectInScattering(const Scene & scene, const glm::vec3 & pos, float segment)
{
	return 0; //For now ignore.
}