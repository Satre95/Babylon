#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "AnisotropicPhongMaterial.hpp"
#include "Rand.hpp"

AnisotropicPhongMaterial::AnisotropicPhongMaterial(glm::vec3 diffuse, glm::vec3 specular) : m_RDiffuse(diffuse), m_RSpecular(specular)
{}
AnisotropicPhongMaterial::~AnisotropicPhongMaterial() {}

//TODO: Implement reflectance calculation fn.
void AnisotropicPhongMaterial::ComputeReflectance(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit)  {
    
}

//TODO: Implement sampe generation fn.
void AnisotropicPhongMaterial::GenerateSample(const Intersection & isect, const glm::vec3 & inDir, glm::vec3 & outDir, Color & outColor) {
    float rand1 = Rand::randFloat();
    float rand2 = Rand::randFloat();
    
    float term1 = glm::sqrt((m_specU + 1.f) / (m_specV + 1.f));
    float term2 = glm::tan(glm::pi<float>() * rand1 * 0.5f);
    float phi = glm::atan(term1 * term2);
    
    float base = 1.f - rand2;
    float exponent = 1.f / (m_specU * glm::cos(phi) * glm::cos(phi) + m_specV * glm::sin(phi) * glm::sin(phi) + 1.f);
    float cosTheta = glm::pow(base, exponent);
    

}


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

glm::vec3 AnisotropicPhongMaterial::ComputeDiffuseReflectance(
        const glm::vec3 & k1,
        const glm::vec3 & k2,
        const Intersection & isect)
{
    const auto & norm = isect.Normal;
    const auto k1DotNorm = glm::dot(k1, norm);
    const auto k2DotNorm = glm::dot(k2, norm);
    
    // First term
    const auto term1 = 28.f * m_RDiffuse
                        / (23.f * glm::pi<float>())
                        * (glm::vec3(1.f) - m_RSpecular);
    
    // Second term
    const auto term2 = 1.f - glm::pow(1.f - 0.5f * k1DotNorm, 5.f);
    
    // Third term
    const auto term3 = 1.f - glm::pow(1.f - 0.5f * k2DotNorm, 5.f);
    
    auto diffuseReflectance = term1 * term2 * term3;
    return diffuseReflectance;
}
