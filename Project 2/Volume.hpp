#pragma once
#include <glm/vec3.hpp>
#include <memory>

#include "ScatterPhase.hpp"

class Volume
{
public:
	virtual void Evaluate(
		const glm::vec3 & pos,
		float & absorptionCoeff,
		float & scatteringCoeff,
		float & emissionPhaseFn,
		float & scatteringPhaseFn
	) = 0;
	virtual ~Volume() { delete scatterPhase; };

protected:
	ScatterPhase * scatterPhase;
private:
};