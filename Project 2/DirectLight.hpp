//
//  DirectLight.hpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Light.hpp"

class DirectLight : public Light {
public:
	DirectLight(glm::vec3 direction) { this->direction = direction; }
	DirectLight() { direction = glm::vec3(0); }

	void SetDirection(glm::vec3 d) { direction = d; }
	float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos);
private:
	glm::vec3 direction;
};