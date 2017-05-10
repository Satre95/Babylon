//
//  Utilities.hpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#pragma once

#define FLOAT_THRESHOLD 1e-10f

#include <ctime>
#include <cstdlib>
#include <random>

class Utilities {
public:

	static float randomFloatInRange(const float & LO, const float & HI) {
		std::random_device rand_dev;
		std::mt19937 generator(rand_dev());
		std::uniform_real_distribution<float> dist(LO, HI);
		return dist(generator);
	}
};