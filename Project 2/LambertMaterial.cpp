#include "LambertMaterial.hpp"
#include "Utilities.hpp"
void LambertMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit)
{
	col = DiffuseColor;
	col.Scale(1.f / glm::pi<float>());
}
void LambertMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
	//Randomly sample an output dir over the cosing weighted hemisphere.
	float s = Utilities::randomFloatInRange(0.f, 1.f);
	float t = Utilities::randomFloatInRange(0.f, 1.f);
	float u = 2.f * glm::pi<float>() * s;
	float v = pow(1 - t, 0.5f);
	outDir.x = v * cos(u);
	outDir.y = pow(t, 0.5f);
	outDir.z = v * sin(u);

	//Set the output color;
	outColor = DiffuseColor;
}