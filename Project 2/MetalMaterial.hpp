#pragma once

#include "Material.hpp"
#include <glm/gtc/constants.hpp>

class MetalMaterial : public Material
{
public:
	void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {};

	void SetColor(Color & c) { matColor = c; }
private:
	Color matColor;
};