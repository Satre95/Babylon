#include "FogVolume.hpp"
#include <glm/gtc/random.hpp>

FogVolume::FogVolume()
{
	scatterPhase = new LorenzMieScatter();
}

void FogVolume::EvaluateRadiance(
    Color & incomingRad,
    const Ray & incomingRay,
    const RayTrace & rayTracer,
    const Scene & scene,
    const glm::vec3 & pos, //x
    int depth
)
{
    //Walk back along the ray
    float dist = glm::distance(pos, incomingRay.Origin);
    float step = dist / float(numMarchSamples);
    glm::vec3 reverseRay = -incomingRay.Direction;
    
    for(int i = 0; i < numMarchSamples; i++)
    {
        //Get the bounds of this segment
        float low = float(i) * step;
        float high = float(i+1) * step;
        
        //Sample at random point in the segment
        float sampleDist = Utilities::randomFloatInRange(low, high);
        glm::vec3 samplePoint = pos + reverseRay * sampleDist;
        
        //Now to real business, compute parts of eq
        //1. Add Emission scaled by segment
        EvaluateEmission(incomingRad, scene, samplePoint, sampleDist);
        
        //2. Add In scattering, scaled by segment
        EvaluateInScattering(incomingRad, incomingRay, rayTracer, scene, samplePoint, sampleDist, depth);
        
        
        //3. Multiply by extinction
        EvaluateExtinction(incomingRad, samplePoint, sampleDist);

    }
}

bool FogVolume::Intersect(const Ray & ray)
{
	return true; //Infinite volume
}

void FogVolume::EvaluateExtinction(Color & incomingRad, const glm::vec3 & pos, float step)
{
	//Calculate each spectrum individually.
	float rScale = glm::pow(glm::e<float>(), -1.f * extinctionCoeff.GetRed() * step);
	float gScale = glm::pow(glm::e<float>(), -1.f * extinctionCoeff.GetGreen() * step);
	float bScale = glm::pow(glm::e<float>(), -1.f * extinctionCoeff.GetBlue() * step);

	incomingRad.Multiply(Color(rScale, gScale, bScale)); //Homogenous vol, so constant.
}

void FogVolume::EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step)
{} //Do nothing, as fog does not emit light.

void FogVolume::EvaluateInScattering(
                                     Color & incomingRad,
                                     const Ray & incomingRay,
                                     const RayTrace & rayTracer,
                                     const Scene & scene,
                                     const glm::vec3 & pos,
                                     float step,
                                     int depth)
{
	//1. compute in scattering from indirect sources around the environment. (only one)
	EvaluateIndirectInScattering(incomingRad, incomingRay, rayTracer, scene, pos, step, depth);
	//2. compute in scattering from direct lights.
	EvaluateDirectInScattering(incomingRad, incomingRay, scene, pos, step);
    
    //Finally scale by the distance
    incomingRad.Multiply(step);
    
}

void FogVolume::EvaluateDirectInScattering(Color & incomingRad, const Ray & incomingRay, const Scene & scene, const glm::vec3 & pos, float step)
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

			//Reuse the extinction fn to calculate the attenuation of the light through the volume.
			EvaluateExtinction(illuminatedColor, pos, length);
			incomingRad.Add(illuminatedColor);
		}
	}
}

//TODO: Figure this shit out.
//Note: Matteo thinks that this should be some very small constant
void FogVolume::EvaluateIndirectInScattering(
                                             Color & incomingRad,
                                             const Ray & incomingRay,
                                             const RayTrace & rayTrace,
                                             const Scene & scene,
                                             const glm::vec3 & pos,
                                             float step,
                                             int depth
                                             )
{
	//Generate a random ray
    float u = Utilities::randomFloatInRange(0.f, 1.f);
	float v = Utilities::randomFloatInRange(0.f, 1.f);
	float theta = 2.f * glm::pi<float>() * u;
	float phi = acosf(2.f * v - 1.f);

	float x = sinf(phi) * cosf(theta);
	float y = sinf(phi) * sinf(phi);
	float z = cosf(phi);

	Ray randomRay;
	randomRay.Direction = glm::vec3(x, y, z);
	randomRay.Origin = pos;
    
    //Recursively cast the generated ray into the volume.
    Intersection randHit;
    rayTrace.TraceRay(randHit, randomRay, depth + 1);
    
    //calc the phase fn
    Color phase = scatterPhase->PhaseFn(glm::dot(incomingRay.Direction, randomRay.Direction));
    //Apply phase
    randHit.Shade.Multiply(phase);
    
    //Apply extinction
    EvaluateExtinction(randHit.Shade, pos, step);
    
    //Multiply by 4π for solid angle
//    randHit.Shade.Scale(4.f * glm::pi<float>());
    
    //Finally add it into the incoming radiance
    incomingRad.Add(randHit.Shade);
}

void FogVolume::SetAbsroptionCoeff(Color & abIn)
{
	absorptionCoeff = abIn; extinctionCoeff = scatteringCoeff + abIn;
}

void FogVolume::SetScatteringCoeff(Color & scIn)
{
	scatteringCoeff = scIn; extinctionCoeff = absorptionCoeff + scIn;
}

void FogVolume::SetAbsroptionCoeff(Color && abIn)
{
    absorptionCoeff = abIn; extinctionCoeff = scatteringCoeff + abIn;
}

void FogVolume::SetScatteringCoeff(Color && scIn)
{
    scatteringCoeff = scIn; extinctionCoeff = absorptionCoeff + scIn;
}
