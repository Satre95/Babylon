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
	MeshObject(std::vector<Vertex> & verts, std::vector<size_t> & indices);
	MeshObject(std::vector<Vertex> & verts, std::vector<Triangle> & tris);
	~MeshObject();

	bool Intersect(const Ray &ray, Intersection &hit);
	void MakeBox(float x, float y, float z, Material * mtl = nullptr);

	size_t GetNumTriangles() const { return m_triangles.size(); }
	size_t GetNumVertexes() const { return m_vertices.size(); }
	Triangle * GetTriangles() { return m_triangles.data(); }
	Vertex * GetVertices() { return m_vertices.data(); }

private:
	void GenerateTriangles(std::vector<size_t> & indices);
	std::vector<Vertex> m_vertices;
	std::vector<Triangle> m_triangles;
	Material * m_material;
};

////////////////////////////////////////////////////////////////////////////////