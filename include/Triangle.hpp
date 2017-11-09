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
	Triangle(Vertex * v0, Vertex * v1, Vertex * v2, Material * m);
	void Init(Vertex * v0, Vertex * v1, Vertex * v2, Material * m);

	bool Intersect(const Ray &ray, Intersection &hit) const;

	const Vertex & GetVertex(int i) const { return *(m_vertices.at(i)); }

	glm::vec3 GetCenter()const;

private:
	std::array<Vertex *, 3> m_vertices;
	Material * material;
};

////////////////////////////////////////////////////////////////////////////////