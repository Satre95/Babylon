////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial :public Material {
public:	
    LambertMaterial(): Material() {}
    LambertMaterial(std::string file): Material(file) {}
    LambertMaterial(Texture * tex): Material(tex) {}
	LambertMaterial(Color c) { DiffuseColor = c; }
	void ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit);

	void GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor);

	void SetColor(Color & c) { DiffuseColor = c; }
	void SetColor(Color && c) { DiffuseColor = c; }
private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
