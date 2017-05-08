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

class Utilities {
public:

	static float randomFloatInRange(float LO, float HI) {
		return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	}
};