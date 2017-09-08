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
	bool LoadPLY(const char * filename, Material * mtl = nullptr);
	void Smooth();

	int GetNumTriangles() const { return NumTriangles; }
	int GetNumVertexes() const { return NumVertexes; }
	Triangle * GetTriangles() { return Triangles; }
	Triangle ** GetTrianglePtrs();

private:

	void BuildTrianglePointerArray();

	int NumVertexes, NumTriangles;
	Vertex * Vertexes;
	Triangle * Triangles;
	Triangle ** TrianglePtrs;
};

////////////////////////////////////////////////////////////////////////////////