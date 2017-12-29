#include "LambertMaterial.hpp"
#include "Utilities.hpp"
#include "Rand.hpp"

void LambertMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit)
{
	col = DiffuseColor;
	col.Scale(1.f / glm::pi<float>());
}
void LambertMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
	//Randomly sample an output dir over the cosing weighted hemisphere.
    float s = Rand::randFloat();
    float t = Rand::randFloat();
	float u = 2.f * glm::pi<float>() * s;
	float v = sqrtf(1.f - t);

	glm::vec3 i = glm::normalize(glm::vec3(isect.Normal.x + 5.f, isect.Normal.y - 2.f, isect.Normal.z));
	glm::vec3 j = isect.Normal;
	glm::vec3 k = glm::cross(i, j);

	outDir = v * cosf(u) * i + sqrtf(t) * j + v * sinf(u) * k;

	//Set the output color;
    if(m_texture)
        outColor = m_texture->SampleTexture(isect.TexCoord);
    else
        outColor = DiffuseColor;
}
