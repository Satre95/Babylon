//
//  Utilities.hpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#pragma once

#define FLOAT_THRESHOLD 1e-8f

#include <ctime>
#include <cstdlib>
#include <string>

class Utilities {
public:

	/// Extracts the file name from the given path.
	static std::string FilenameFromPath(std::string & path) {
		return path.substr(0, path.find_last_of('/'));
	}
};

template <typename Enumeration>
auto RawType(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
