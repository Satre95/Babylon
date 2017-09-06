//
//  PointLight.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "PointLight.hpp"

float PointLight::Illuminate(const glm::vec3 &objPos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) {
	toLight = position - objPos;
	float bright = Intensity / pow(glm::length(toLight), 2.0f); // Inverse square falloff
	toLight = glm::normalize(toLight);
	col = BaseColor;
	ltPos = position;
	return bright;
}