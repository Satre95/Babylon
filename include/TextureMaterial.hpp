#pragma once
#include "Material.hpp"
#include "Texture.hpp"

class TextureMaterial: public Material {
public:
	TextureMaterial(Texture * tex);
	TextureMaterial(std::string file);
	~TextureMaterial();

	void ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) override;
	void GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) override;
	
private:
	Texture * m_texture = nullptr;


};