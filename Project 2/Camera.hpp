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
#include <glm/gtc/random.hpp>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>

#include "Scene.hpp"
#include "Bitmap.hpp"
#include "Material.hpp"
#include "RayTrace.hpp"

class Camera {
public:
	Camera() {}
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, glm::vec2 dims);
	~Camera() {}

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
	void SetResolution(int x, int y);

	void SetSuperSample(int xSamples, int ySamples) {
		superSamples = std::make_pair(xSamples, ySamples);
	}

	void SetFocus(float dist) { focalPlane = dist; }

	void SetfStop(float fstop) {
		fStop = fstop;
	}
	void SetJitter(bool enable) { jitterEnabled = enable; }
	void SetShirley(bool enable) { shirleyEnabled = enable; }

	void BuildCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	void Render(Scene & scene, bool parallel = false);
	void SaveBitmap(std::string filename);

private:
	//Camera params
	int width, height;
	glm::mat4 C;
	glm::mat4 V;
	float aspect;
	float vFov;
	float hFov;
	float fStop = 1e10f, focalPlane = 1.f;
	BitmapPtrUnique img;

	//Sampling params.
	std::pair<int, int> superSamples = std::make_pair(1, 1);
	std::pair<float, float> subPixelDims;
	bool jitterEnabled = false;
	bool shirleyEnabled = false;

	//Parallel scheduling params.
	std::atomic_int currX;
	std::atomic_int currY;
	std::unique_ptr<RayTrace> rayTracer;
	std::atomic_int tileCoordIndex;
	std::vector<std::pair<uint32_t, uint32_t>> tileCoords;
	int numTilesX, numTilesY;
	int tileWidth = 8, tileHeight = 6;
    std::unique_ptr<std::thread> previewThread;
    std::atomic_bool previewWrite;
    std::atomic_bool finished;
    
#ifdef DEBUG
	std::mutex logMutex;
#endif // DEBUG

	void RenderPixel(int x, int y, Scene & scene);
	void RenderPixel(int aTile, Scene & scene);

	///Modifies the subpixel sample point to be randomized within the subpixel
	void JitterSubPixel(float & subX, float & subY);
	///Modifies the subpixel sample point to be weighted with shirley distribution.
	void ApplyShirleyWeight(float & subX, float & subY);

	///Renders the pixel with CPU parallelism. Range is not inclusive of ending point.
	void RenderPixelsParallel(Scene & scene);
    
    ///Thread that allows one to preview the image as it is begin generated.
    void PreviewImageFunc();
};
