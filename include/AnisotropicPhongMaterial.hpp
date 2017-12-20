#pragma once
#include <glm/vec3.hpp>
#include "Material.hpp"
#include "Intersection.hpp"

class AnisotropicPhongMaterial: public Material
{
public:
    AnisotropicPhongMaterial(glm::vec3 diffuse = glm::vec3(1.f), glm::vec3 specular = glm::vec3(0.9f, 0.f, 0.3f));
	~AnisotropicPhongMaterial();
    
    void ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) override;
    void GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) override;
    
private:
    
    glm::vec3 ComputeSpecularReflectance(const glm::vec3 & k1, const glm::vec3 & k2, const Intersection & isect);
    glm::vec3 ComputeDiffuseReflectance(const glm::vec3 & k1, const glm::vec3 & k2, const Intersection & isect);
    
    /// Diffuse Reflectance of this material
    glm::vec3 m_RDiffuse;
    /// Specular reflectance of this material
    glm::vec3 m_RSpecular;
    /// U Specular Exponent
    float m_specU;
    /// V Specular Exponent
    float m_specV;
	
};
