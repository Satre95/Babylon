#pragma once

#include "Material.hpp"
#include <glm/gtc/constants.hpp>

class MetalMaterial : public Material
{
public:
	void ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit);
	void GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor);

	void SetColor(Color & c) { matColor = c; }
	void SetColor(Color && c) { matColor = c; }
private:
	Color matColor;
};