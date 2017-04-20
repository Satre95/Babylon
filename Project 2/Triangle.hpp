////////////////////////////////////////
// Triangle.hpp
////////////////////////////////////////

#pragma once

#include "Vertex.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Utilities.hpp"
#include <array>
////////////////////////////////////////////////////////////////////////////////

class Triangle {
public:
    Triangle(){}
    void Init(Vertex * v0,Vertex * v1,Vertex * v2, Material * m);

	bool Intersect(const Ray &ray, Intersection &hit) const;

private:
//    std::array<Vertex *, 3> vertices;
    Vertex * vertices[3];
	Material * material;
};


////////////////////////////////////////////////////////////////////////////////
