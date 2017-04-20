////////////////////////////////////////
// MeshObject.hpp
////////////////////////////////////////

#pragma once

#define _CRT_SECURE_NO_WARNINGS

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
	bool LoadPLY(const char * filename, Material * mtl);
	void Smooth();

private:
	int NumVertexes, NumTriangles;
	Vertex * Vertexes;
	Triangle * Triangles;
};

////////////////////////////////////////////////////////////////////////////////