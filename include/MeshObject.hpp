////////////////////////////////////////
// MeshObject.hpp
////////////////////////////////////////

#pragma once

#include <vector>
#include "Object.hpp"
#include "Triangle.hpp"
#include "Texture.hpp"
////////////////////////////////////////////////////////////////////////////////

class MeshObject :public Object {
public:
	MeshObject(std::vector<Vertex> & verts, std::vector<size_t> & indices, 
		std::vector<Texture *> textures = std::vector<Texture *>());
	MeshObject(std::vector<Vertex> & verts, std::vector<Triangle> & tris, 
		std::vector<Texture *> textures = std::vector<Texture *>());
	~MeshObject();

	bool Intersect(const Ray &ray, Intersection &hit);
	void MakeBox(float x, float y, float z, Material * mtl = nullptr);

	size_t GetNumTriangles()		{ return m_triangles.size();	}
	size_t GetNumVertexes()			{ return m_vertices.size();		}
	size_t GetNumTextures()			{ return m_textures.size();		}
	Triangle * GetTriangles()		{ return m_triangles.data();	}
	Triangle ** GetTrianglePtrs()	{ return m_trianglePtrs;		}
	Vertex * GetVertices()			{ return m_vertices.data();		}

private:
	void GenerateTriangles(const std::vector<size_t> & indices);
	void BuildTrianglePointerArray();
	std::vector<Vertex> m_vertices;
	std::vector<Triangle> m_triangles;
	std::vector<Texture *> m_textures;
	Material * m_material = nullptr;
	Triangle ** m_trianglePtrs = nullptr;
};

////////////////////////////////////////////////////////////////////////////////