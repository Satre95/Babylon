﻿#include "RayTrace.hpp"
#include "Volume.hpp"

void RayTrace::TraceRay(Intersection & hitData, const Ray & ray, int depth) {
	//If max depth reached, terminate recursion.
	if (depth == maxDepth) return;

	if (scene.Intersect(ray, hitData)) {
		hitData.Shade = Color::BLACK;
		for (int i = 0; i < scene.GetNumLights(); i++) {
			Light & light = scene.GetLight(i);
			glm::vec3 lightPos;
			glm::vec3 toLight;
			Color lightColor = Color::BLACK;
			float intensity = light.Illuminate(hitData.Position, lightColor, toLight, lightPos);

			if (intensity <= 0.0f) continue;

			//Check if this is shadowed by anything
			Intersection shadowHit;
			shadowHit.HitDistance = glm::length(lightPos - hitData.Position) - FLOAT_THRESHOLD;
			shadowHit.Position = hitData.Position;
			Ray shadowRay;
			shadowRay.Origin = shadowHit.Position;
			shadowRay.Direction = glm::normalize(lightPos - hitData.Position);
			if (!scene.Intersect(shadowRay, shadowHit)) {
				Color matColor = Color::BLACK;
                glm::vec3 temp;
				hitData.Mtl->ComputeReflectance(matColor, toLight, temp, hitData);
				matColor.Multiply(lightColor);
				matColor.Scale(glm::max(0.f, glm::dot(hitData.Normal, toLight)));
				matColor.Scale(intensity);

				hitData.Shade.Add(matColor);
			}
		}

		//Now recursively compute reflected ray and color
		//1. Generate new ray
		glm::vec3 reflection;
		Intersection reflectHit;
		reflectHit.Shade = Color::BLACK;
		Color tempColor;
		hitData.Mtl->GenerateSample(hitData, ray.Direction, reflection, tempColor);
		Ray reflectRay(hitData.Position, reflection);
		//2. Trace the ray
		TraceRay(reflectHit, reflectRay, depth + 1);
		reflectHit.Shade.Multiply(tempColor);

		//Now compute volumetric effects.
		for (int i = 0; i < scene.GetNumVolumes(); i++)
		{
			Volume * aVol = scene.GetVolume(i);
			aVol->EvaluateRadiance(
				reflectHit.Shade, //L(x, w) radiance at this point
				ray,
				scene,
				reflectHit.Position, // x' aka x + sw
				glm::length(reflectHit.Position - hitData.Position) //length of segment b/w x & x'
			);
		}

		hitData.Shade.Add(reflectHit.Shade);
	}
	else {
		hitData.Shade = scene.GetSkyColor();
	}
}
