////////////////////////////////////////
// Scene.hpp
////////////////////////////////////////

#pragma once

#include "Object.hpp"
#include "Light.hpp"
#include <vector>

class Volume;
////////////////////////////////////////////////////////////////////////////////

class Scene {
public:
	Scene() { SkyColor.Set(0.2f, 0.2f, 0.5f); }
	~Scene() { Objects.clear(); Lights.clear(); }
	void AddObject(Object & obj) { Objects.push_back(&obj); }
	void AddLight(Light &lgt) { Lights.push_back(&lgt); }
	void AddVolume(Volume & vol);
	void SetSkyColor(const Color sky) { SkyColor = sky; }

	int GetNumLights() { return (int)Lights.size(); }
	Light &GetLight(int i) { return *Lights.at(i); }
	Color GetSkyColor() { return SkyColor; }

	bool Intersect(const Ray &ray, Intersection &hit);
	std::vector<Volume *> IntersectVolumes(const Ray & ray);
private:
	std::vector<Object*> Objects;
	std::vector<Light*> Lights;
	std::vector<Volume*> Volumes;
	Color SkyColor;
};
////////////////////////////////////////////////////////////////////////////////