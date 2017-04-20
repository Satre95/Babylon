////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.hpp"

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial :public Material {
public:
	void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
		col.Multiply(DiffuseColor);
		float ang = glm::dot(hit.Normal, in);

		if (ang <= 0) {
			col = Color::BLACK;
		}
		else {
			col.Scale(ang);
		}
		//		float pi=3.1415926f;
		//		col.Scale(DiffuseColor,1.0f/pi);
	}

private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////