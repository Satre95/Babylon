
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

#include "TextureMaterial.hpp"

////////////////////////////////////////////////////////////////////////////////

TextureMaterial::TextureMaterial(Texture * tex):
m_texture(tex) {

}

////////////////////////////////////////////////////////////////////////////////

TextureMaterial::TextureMaterial(std::string file):
m_texture(new Texture(file)) {

}

////////////////////////////////////////////////////////////////////////////////

TextureMaterial::~TextureMaterial() {}

////////////////////////////////////////////////////////////////////////////////

// For now just approximate the texture material as a Lambert diffuse surface.
// TODO: Implement proper specuar + diffuse + ambient properties.
void TextureMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit)  {
	// Sample the texture
	col = m_texture->SampleTexture(hit.TexCoord);
	// Scale by the cosine weight hemisphere
	col.Scale(1.f / glm::pi<float>());
}

////////////////////////////////////////////////////////////////////////////////

void TextureMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
	//Randomly sample an output dir over the cosing weighted hemisphere.
	float s = glm::linearRand(0.f, 1.f);
	float t = glm::linearRand(0.f, 1.f);
	float u = 2.f * glm::pi<float>() * s;
	float v = sqrtf(1.f - t);

	glm::vec3 i = glm::normalize(glm::vec3(isect.Normal.x + 5.f, isect.Normal.y - 2.f, isect.Normal.z));
	glm::vec3 j = isect.Normal;
	glm::vec3 k = glm::cross(i, j);

	outDir = v * cosf(u) * i + sqrtf(t) * j + v * sinf(u) * k;

	//Set the output color;
	outColor = m_texture->SampleTexture(isect.TexCoord);
}
