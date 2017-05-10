////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.hpp"
#include <glm/gtc/constants.hpp>

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial :public Material {
public:
	void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
		col = DiffuseColor;
		col.Scale(1 / glm::pi<float>());
	}

	void SetColor(Color & c) { DiffuseColor = c; }
private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////