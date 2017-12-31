////////////////////////////////////////
// Ray.hpp
////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"
#include <boost/simd/pack.hpp>

////////////////////////////////////////////////////////////////////////////////

class Ray {
public:
	Ray() {}
    ///First param is origin, second is direction.
	Ray(glm::vec3 o, glm::vec3 d) :
    Origin(o), Direction(d),
    p_Origin(
             o.x,
             o.y,
             o.z,
             1.f,
             o.x,
             o.y,
             o.z,
             1.f
               ),
    p_Direction(
             d.x,
             d.y,
             d.z,
             1.f,
             d.x,
             d.y,
             d.z,
             1.f
             )
    
    {}
	const glm::vec3 Origin;
	const glm::vec3 Direction;
    // Useful for SIMD processing.
    const boost::simd::pack<float, 8> p_Origin;
    const boost::simd::pack<float, 8> p_Direction;
    
};

////////////////////////////////////////////////////////////////////////////////
