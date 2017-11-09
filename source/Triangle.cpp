//
//  Triangle.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Triangle.hpp"

Triangle::Triangle(Vertex * v0, Vertex * v1, Vertex * v2, Material * m) {
	Init(v0, v1, v2, m);
}

void Triangle::Init(Vertex * v0, Vertex * v1, Vertex * v2, Material * m) {
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;
	material = m;
}

bool Triangle::Intersect(const Ray &ray, Intersection &hit) const {
	const auto & a = m_vertices[0]->Position;
	const auto & b = m_vertices[1]->Position;
	const auto & c = m_vertices[2]->Position;
	const auto & p = ray.Origin;
	const auto & d = ray.Direction;

	float det_M = glm::dot(-d, glm::cross(b - a, c - a));
	//Check if ray is || to plane
	if (det_M < FLOAT_THRESHOLD)
		return false;

	float t = glm::dot(p - a, glm::cross(b - a, c - a)) / det_M;
	float alpha = glm::dot(-d, glm::cross(p - a, c - a)) / det_M;
	float beta = glm::dot(-d, glm::cross(b - a, p - a)) / det_M;

	//Check if intersection is within bounds of triangle
	if (alpha <= 0 || beta <= 0 || alpha + beta >= 1.0f || t <= 0)
		return false;

	//Compute world space intersection point
	glm::vec3 position = a + alpha * (b - a) + beta * (c - a);
	float dist = glm::distance(position, p);

	if (dist < hit.HitDistance && dist > FLOAT_THRESHOLD) {
		hit.HitDistance = dist;
		hit.Position = position;
		hit.Normal = glm::normalize((1 - alpha - beta) * m_vertices[0]->Normal + alpha * m_vertices[1]->Normal + beta * m_vertices[2]->Normal);
		hit.Mtl = material;

		hit.TangentU = glm::cross(glm::vec3(0, 1.f, 0), hit.Normal);
		if (glm::length(hit.TangentU) < FLOAT_THRESHOLD)
			hit.TangentU = glm::cross(glm::vec3(1.f, 0, 0), hit.Normal);
		hit.TangentU = glm::normalize(hit.TangentU);
		hit.TangentV = glm::cross(hit.Normal, hit.TangentU);

		//Interpolate the texture coords of the three vertices.
		hit.TexCoord = (1.f - alpha - beta) * m_vertices[0]->TexCoord + alpha * m_vertices[1]->TexCoord + beta * m_vertices[2]->TexCoord;

		return true;
	}
	return false;
}

glm::vec3 Triangle::GetCenter() const {
	//Avg the coordinates
	return (m_vertices[0]->Position + m_vertices[1]->Position + m_vertices[2]->Position) / 3.f;
}