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
	virtual void EvaluateRadiance(
		Color & incomingRad, // L(x, w)
		const Scene & scene,
		const glm::vec3 & pos, //x
		float step //s
	) = 0;

	virtual bool Intersect(const Ray & ray) = 0;
	virtual ~Volume() { delete scatterPhase; };

protected:
	ScatterPhase * scatterPhase;

	virtual void EvaluateExtinction(Color & incomringRad, const Scene & scene, const glm::vec3 & pos, float step) = 0;
	virtual void EvaluateEmission(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) = 0;
	virtual void EvaluateInScattering(Color & incomingRad, const Scene & scene, const glm::vec3 & pos, float step) = 0;
private:
};