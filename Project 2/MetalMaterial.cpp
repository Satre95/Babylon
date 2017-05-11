#include "MetalMaterial.hpp"

void MetalMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
}

void MetalMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
	auto & normal = isect.Normal;

	//Compute the reflection dir
	outDir = glm::normalize(2.f * normal * glm::dot(normal, inDir) - inDir);
}