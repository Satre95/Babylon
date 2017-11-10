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

	size_t GetNumTriangles()		{ return m_triangles.size();	}
	size_t GetNumVertexes()			{ return m_vertices.size();		}
	Triangle * GetTriangles()		{ return m_triangles.data();	}
	Triangle ** GetTrianglePtrs()	{ return m_trianglePtrs;		}
	Vertex * GetVertices()			{ return m_vertices.data();		}

private:
	void GenerateTriangles(const std::vector<size_t> & indices);
	void BuildTrianglePointerArray();
	std::vector<Vertex> m_vertices;
	std::vector<Triangle> m_triangles;
	Material * m_material = nullptr;
	Triangle ** m_trianglePtrs = nullptr;
};

////////////////////////////////////////////////////////////////////////////////