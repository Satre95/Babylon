//
//  Triangle.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Triangle.hpp"



void Triangle::Init(Vertex * v0,Vertex * v1,Vertex * v2, Material * m) {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    material = m;
}

bool Triangle::Intersect(const Ray &ray, Intersection &hit) const {
    const auto & a = vertices[0]->Position;
    const auto & b = vertices[1]->Position;
    const auto & c = vertices[2]->Position;
    const auto & p = ray.Origin;
    const auto & d = ray.Direction;
    
    float det_M = glm::dot(-d, glm::cross(b-a, c-a));
    //Check if ray is || to plane
    if (det_M < FLOAT_THRESHOLD)
        return false;
    
    float t = glm::dot(p-a, glm::cross(b-a, c-a)) / det_M;
    float alpha = glm::dot(-d, glm::cross(p-a, c-a)) / det_M;
    float beta = glm::dot(-d, glm::cross(b-a, p-a)) / det_M;
    
    //Check if intersection is within bounds of triangle
    if(alpha <= 0 || beta <= 0 || alpha + beta >= 1.0f || t <= 0 )
        return false;
    
    //Compute world space intersection point
    hit.Position = a+ alpha * (b-a) + beta * (c-a);
    
    //Calculate normal
    hit.Normal = glm::normalize((1-alpha-beta) * vertices[0]->Normal + alpha * vertices[1]->Normal + beta * vertices[2]->Normal);
    
    //Set info
    hit.HitDistance = glm::distance(hit.Position, p);
    hit.Mtl = material;
    
    return true;
}
