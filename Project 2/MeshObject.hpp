////////////////////////////////////////
// MeshObject.hpp
////////////////////////////////////////

#pragma once

#include <vector>
#include "Object.hpp"
#include "Triangle.hpp"
////////////////////////////////////////////////////////////////////////////////

class MeshObject :public Object {
public:
	MeshObject();
	~MeshObject();

	bool Intersect(const Ray &ray, Intersection &hit);

	void MakeBox(float x, float y, float z, Material * mtl = nullptr);

private:
	int NumVertexes, NumTriangles;
	Vertex * Vertexes;
	Triangle * Triangles;
};

////////////////////////////////////////////////////////////////////////////////