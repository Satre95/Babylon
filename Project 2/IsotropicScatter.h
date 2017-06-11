#pragma once

#include <glm/gtc/constants.hpp>
#include "ScatterPhase.hpp"

class IsotropicScatter : public ScatterPhase
{
public:
	IsotropicScatter() {}
	~IsotropicScatter() {}

protected:
	float PhaseFn(float cosAngle) override
	{
		return sphereConst;
	}

private:
	float sphereConst = 1.f / (4.f * glm::pi<float>());
};