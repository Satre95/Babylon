#pragma once

#include <glm/gtc/constants.hpp>
#include "ScatterPhase.hpp"

class IsotropicScatter : public ScatterPhase
{
public:
	IsotropicScatter() {}
	~IsotropicScatter() {}

	Color PhaseFn(float cosAngle) override
	{
		return sphereConst;
	}

private:
	Color sphereConst = Color(1.f / (4.f * glm::pi<float>()));
};