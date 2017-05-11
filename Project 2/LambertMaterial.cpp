#include "LambertMaterial.hpp"

void LambertMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit)
{
	col = DiffuseColor;
	col.Scale(1.f / glm::pi<float>());
}
void LambertMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
}