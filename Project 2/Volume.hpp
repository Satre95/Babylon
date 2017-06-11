#pragma once
#include <glm/vec3.hpp>
#include <memory>

#include "ScatterPhase.hpp"
#include "Ray.hpp"

class Volume
{
public:
	virtual float EvaluateRadiance(
		const glm::vec3 & pos,
		const Ray & ray,
		float step
	) = 0;

	virtual bool Intersect(const Ray & ray) = 0;
	virtual ~Volume() { delete scatterPhase; };

protected:
	ScatterPhase * scatterPhase;
private:
};