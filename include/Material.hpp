////////////////////////////////////////
// Material.hpp
////////////////////////////////////////

#pragma once

#include "Intersection.hpp"
#include "Texture.hpp"

#include <memory>

////////////////////////////////////////////////////////////////////////////////

class Material {
public:
	virtual ~Material() {};
    Material() {}
    Material(std::string file): m_texture(new Texture(file)) {}
    Material(Texture * tex): m_texture(tex) {}
	virtual void ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) = 0;
	virtual void GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) = 0;
protected:
    Texture * m_texture = nullptr;
};
typedef std::shared_ptr<Material> MaterialPtr;

////////////////////////////////////////////////////////////////////////////////
