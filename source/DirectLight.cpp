//
//  DirectLight.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "DirectLight.hpp"

float DirectLight::Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) {
	toLight = -direction;
	col = BaseColor;
	ltPos = pos - (1e10f * direction);
	return Intensity;
}