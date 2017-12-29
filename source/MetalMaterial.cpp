#include "MetalMaterial.hpp"

void MetalMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) {
	glm::vec3 reflect = glm::normalize(glm::reflect(in, hit.Normal));
	glm::vec3 outNormalized = glm::normalize(out);

	if (glm::all(glm::equal(reflect, outNormalized))) {
		col.Scale(matColor, 1.0f);
	}
	else {
		col.Scale(Color::BLACK, 1.0f);
	}
}

void MetalMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
	auto & normal = isect.Normal;

	//Compute the reflection dir
	outDir = glm::reflect(inDir, normal);
    
	//Set the color
    if(m_texture)
        outColor = m_texture->SampleTexture(isect.TexCoord);
    else
        outColor = matColor;
}
