////////////////////////////////////////
// Vertex.hpp
////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

////////////////////////////////////////////////////////////////////////////////

class Vertex {
public:
	void Set(const glm::vec3 &p,const glm::vec3 &n,const glm::vec3 &t)		{Position=p; Normal=n; TexCoord=t;}

public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoord;
};


////////////////////////////////////////////////////////////////////////////////
