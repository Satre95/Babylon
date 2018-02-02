////////////////////////////////////////
// MeshObject.cpp
////////////////////////////////////////

#include <cstring>
#include "MeshObject.hpp"
#include "LambertMaterial.hpp"
#include "MetalMaterial.hpp"
#include "AnisotropicPhongMaterial.hpp"
#include "glm/glm.hpp"

Material* MeshObject::s_defaultMaterial = new MetalMaterial();

////////////////////////////////////////////////////////////////////////////////

MeshObject::MeshObject(std::vector<Vertex> & verts, std::vector<size_t> & indices,
	std::vector<Texture *> textures) {
	m_vertices = verts;

    
    if(textures.empty())
        m_material = s_defaultMaterial;
    else
        m_material = new LambertMaterial(textures.front());

	GenerateTriangles(indices);
	m_textures = textures;
}

////////////////////////////////////////////////////////////////////////////////

MeshObject::MeshObject(std::vector<Vertex> & verts, std::vector<Triangle> & tris,
	std::vector<Texture *> textures) {
	m_vertices = verts;
	m_triangles = tris;
	
    if(textures.empty())
        m_material = s_defaultMaterial;
    // TODO: Replace with correct allocation of specific texture types.
    else
        m_material = new LambertMaterial(textures.front());
    
    for(auto & aTri: tris) aTri.SetMaterial(m_material);
    
	m_textures = textures;
	BuildTrianglePointerArray();

}

////////////////////////////////////////////////////////////////////////////////

MeshObject::~MeshObject() {
	m_vertices.clear();
	m_triangles.clear();
//    delete m_material;
	delete[] m_trianglePtrs;

}

////////////////////////////////////////////////////////////////////////////////

bool MeshObject::Intersect(const Ray &ray, Intersection &hit) {
	bool success = false;
	for(const Triangle & aTri: m_triangles)
		if (aTri.Intersect(ray, hit)) 
			success = true;

	return success;
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::GenerateTriangles(const std::vector<size_t> & indices) {
	m_triangles.clear();
	m_triangles.reserve(indices.size() / 3);
	for (int i = 0; i < indices.size(); i+=3) {
		m_triangles.emplace_back(
			&m_vertices.at(indices.at(i)),
			&m_vertices.at(indices.at(i+1)),
			&m_vertices.at(indices.at(i+2)),
			m_material
			);
	}

	BuildTrianglePointerArray();
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::MakeBox(float x, float y, float z, Material *mtl) {
	// Allocate arrays
	m_vertices.clear();
	m_vertices.reserve(24);
	m_triangles.clear();
	m_triangles.reserve(12);
	if(mtl == nullptr)
		mtl = new LambertMaterial();
	m_material = mtl;

	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;

	// Corners
	glm::vec3 p000(-x, -y, -z);
	glm::vec3 p001(-x, -y, z);
	glm::vec3 p010(-x, y, -z);
	glm::vec3 p011(-x, y, z);
	glm::vec3 p100(x, -y, -z);
	glm::vec3 p101(x, -y, z);
	glm::vec3 p110(x, y, -z);
	glm::vec3 p111(x, y, z);

	// Tex coords
	glm::vec3 t00(0.0f, 0.0f, 0.0f);
	glm::vec3 t01(0.0f, 1.0f, 0.0f);
	glm::vec3 t10(1.0f, 0.0f, 0.0f);
	glm::vec3 t11(1.0f, 1.0f, 0.0f);

	// Axis vectors
	static const glm::vec3 xAxis(1, 0, 0);
	static const glm::vec3 yAxis(0, 1, 0);
	static const glm::vec3 zAxis(0, 0, 1);

	// Right
	m_vertices[0].Set(p101, xAxis, t00);
	m_vertices[1].Set(p100, xAxis, t10);
	m_vertices[2].Set(p110, xAxis, t11);
	m_vertices[3].Set(p111, xAxis, t01);
	m_triangles[0].Init(&m_vertices[0], &m_vertices[1], &m_vertices[2], mtl);
	m_triangles[1].Init(&m_vertices[0], &m_vertices[2], &m_vertices[3], mtl);

	// Left
	m_vertices[4].Set(p000, -xAxis, t00);
	m_vertices[5].Set(p001, -xAxis, t10);
	m_vertices[6].Set(p011, -xAxis, t11);
	m_vertices[7].Set(p010, -xAxis, t01);
	m_triangles[2].Init(&m_vertices[4], &m_vertices[5], &m_vertices[6], mtl);
	m_triangles[3].Init(&m_vertices[4], &m_vertices[6], &m_vertices[7], mtl);

	// Top
	m_vertices[8].Set(p011, yAxis, t00);
	m_vertices[9].Set(p111, yAxis, t10);
	m_vertices[10].Set(p110, yAxis, t11);
	m_vertices[11].Set(p010, yAxis, t01);
	m_triangles[4].Init(&m_vertices[8], &m_vertices[9], &m_vertices[10], mtl);
	m_triangles[5].Init(&m_vertices[8], &m_vertices[10], &m_vertices[11], mtl);

	// Bottom
	m_vertices[12].Set(p000, -yAxis, t00);
	m_vertices[13].Set(p100, -yAxis, t10);
	m_vertices[14].Set(p101, -yAxis, t11);
	m_vertices[15].Set(p001, -yAxis, t01);
	m_triangles[6].Init(&m_vertices[12], &m_vertices[13], &m_vertices[14], mtl);
	m_triangles[7].Init(&m_vertices[12], &m_vertices[14], &m_vertices[15], mtl);

	// Front
	m_vertices[16].Set(p001, zAxis, t00);
	m_vertices[17].Set(p101, zAxis, t10);
	m_vertices[18].Set(p111, zAxis, t11);
	m_vertices[19].Set(p011, zAxis, t01);
	m_triangles[8].Init(&m_vertices[16], &m_vertices[17], &m_vertices[18], mtl);
	m_triangles[9].Init(&m_vertices[16], &m_vertices[18], &m_vertices[19], mtl);

	// Back
	m_vertices[20].Set(p100, -zAxis, t00);
	m_vertices[21].Set(p000, -zAxis, t10);
	m_vertices[22].Set(p010, -zAxis, t11);
	m_vertices[23].Set(p110, -zAxis, t01);
	m_triangles[10].Init(&m_vertices[20], &m_vertices[21], &m_vertices[22], mtl);
	m_triangles[11].Init(&m_vertices[20], &m_vertices[22], &m_vertices[23], mtl);

	BuildTrianglePointerArray();
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::BuildTrianglePointerArray() {
	delete[] m_trianglePtrs;
	m_trianglePtrs = new Triangle*[GetNumTriangles()];
	for (int i = 0; i < GetNumTriangles(); ++i)
		m_trianglePtrs[i] = &m_triangles.at(i);
}

////////////////////////////////////////////////////////////////////////////////
