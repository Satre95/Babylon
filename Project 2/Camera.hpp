//
//  Camera.hpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <string>
#include <mutex>

#include "Scene.hpp"
#include "Bitmap.hpp"
#include "Material.hpp"

class Camera {
public:
	Camera() {}
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, glm::vec2 dims);

	glm::mat4 GetViewMatrix() { return V; }
	glm::mat4 GetCamMatrix() { return C; }

	void SetFoV(float fov) {
		vFov = glm::radians(fov);
		hFov = 2.0f * atanf(aspect * tanf(vFov / 2.0f));
	}

	void SetAspect(float a) {
		aspect = a;
		SetFoV(glm::degrees(vFov));
	}
	void SetResolution(int x, int y) {
		width = x; height = y;
		SetAspect(width / height);
	}

	void BuildCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	void Render(Scene & scene, bool parallel = false);
	void SaveBitmap(std::string filename);

private:
	float width;
	float height;
	glm::mat4 C;
	glm::mat4 V;
	float aspect;
	float vFov;
	float hFov;
	BitmapPtrUnique img;
	std::mutex queryMutex;

	void RenderPixel(int x, int y, Scene & scene);
	bool PixelsRemaining();
	std::pair<int, int> GetNextPixel();
	int currX = 0;
	int currY = 0;

	///Renders the pixel with CPU parallelism. Range is not inclusive of ending point.
	void RenderPixelsParallel(Scene & scene);
};