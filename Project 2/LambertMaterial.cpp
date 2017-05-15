#include "LambertMaterial.hpp"
#include "Utilities.hpp"
void LambertMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit)
{
	col = DiffuseColor;
	col.Scale(1.f / glm::pi<float>());
}
void LambertMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
	//Randomly sample an output dir over the cosing weighted hemisphere.
	float s = glm::linearRand(0.f, 1.f);
	float t = glm::linearRand(0.f, 1.f);
	float u = 2.f * glm::pi<float>() * s;
	float v = sqrtf(1.f - t);

	glm::vec3 i = glm::normalize(glm::vec3(isect.Normal.x + 5.f, isect.Normal.y - 2.f, isect.Normal.z));
	glm::vec3 j = isect.Normal;
	glm::vec3 k = glm::cross(i, j);

	outDir = v * cos(u) * i + sqrtf(t) * j + v * sin(u) * k;

	//Set the output color;
	outColor = DiffuseColor;
}