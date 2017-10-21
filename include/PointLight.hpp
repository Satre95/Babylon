//
//  PointLight.hpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#pragma once

#include "Light.hpp"
#include <glm/vec3.hpp>

class PointLight : public Light {
public:

	float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos);

	void SetPosition(glm::vec3 p) { position = p; }
private:
	glm::vec3 position;
};