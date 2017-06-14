#pragma once
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "ScatterPhase.hpp"
#include "Ray.hpp"
#include "RayTrace.hpp"
#include "Light.hpp"
#include "Scene.hpp"

class Volume
{
public:
    ///Takes the radiance at a given point and marches back along the ray, modifying it by the volume.
	virtual void EvaluateRadiance(
		Color & incomingRad,
		const Ray & incomingRay,
        const RayTrace & rayTracer,
		const Scene & scene,
		const glm::vec3 & pos //x
	) = 0;

	virtual bool Intersect(const Ray & ray) = 0;
	virtual ~Volume() { if (scatterPhase) delete scatterPhase; };

protected:
	ScatterPhase * scatterPhase;
};
