#include "AnisotropicPhongMaterial.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

AnisotropicPhongMaterial::AnisotropicPhongMaterial(glm::vec3 diffuse, glm::vec3 specular) : m_RDiffuse(diffuse), m_RSpecular(specular)
{}
AnisotropicPhongMaterial::~AnisotropicPhongMaterial() {}

/// Implement specular reflectance
glm::vec3 AnisotropicPhongMaterial::ComputeSpecularReflectance(
        const glm::vec3 & k1,
        const glm::vec3 & k2,
        const Intersection & isect)
{
    glm::vec3 h = glm::normalize(k1 + k2);
    const auto & tanU = isect.TangentU;
    const auto & tanV = isect.TangentV;
    const auto & norm = isect.Normal;
    const auto hDotNorm = glm::dot(h, norm);
    const auto hDotU = glm::dot(h, tanU);
    const auto hDotV = glm::dot(h, tanV);
    //TODO: Generate sampling ray k
    const auto sampleRay = glm::vec3(1.f);
    const auto hDotK = glm::dot(h, sampleRay);
    const auto normDotk1 = glm::dot(norm, k1);
    const auto normDotk2 = glm::dot(norm, k2);
    
    //Exponnent term
    const float term1 = glm::sqrt((m_specU + 1.f) * (m_specV + 1.f)) / (8.f * glm::pi<float>());
    
    //Specular angle term
    float exponent = m_specU * hDotU * hDotU + m_specV * hDotV * hDotV;
    exponent /= (1 - hDotNorm * hDotNorm);
    float term2_nume = glm::pow(hDotNorm, exponent);
    float term2_denom = hDotK * glm::max(normDotk1, normDotk2);
    const float term2 = term2_nume / term2_denom;
    
    //Fresnel term (Schlick's Approximation)
    const auto term3 = m_RSpecular + (glm::vec3(1.f) - m_RSpecular) * glm::pow(1 - hDotK, 5.f);
    
    auto specularReflectance = term1 * term2 * term3;
    return specularReflectance;
}

//TODO: Implement diffuse reflectance.
glm::vec3 AnisotropicPhongMaterial::ComputeDiffuseReflectance(
        const glm::vec3 & k1,
        const glm::vec3 & k2,
        const Intersection & isect)
{
    
    
    return glm::vec3(0.f);
}
