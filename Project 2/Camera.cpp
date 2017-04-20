//
//  Camera.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Camera.hpp"
#include <vector>
#include <thread>
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, glm::vec2 dims) {
	SetResolution(dims.x, dims.y);
	BuildCamera(pos, target, up);
}

void Camera::BuildCamera(glm::vec3 pos, glm::vec3 target, glm::vec3  up) {
	auto d = pos;
	auto c = glm::normalize(d - target);
	auto a = glm::normalize(glm::cross(up, c));
	auto b = glm::cross(c, a);

	C = glm::mat4(glm::vec4(a, 0), glm::vec4(b, 0), glm::vec4(c, 0), glm::vec4(d, 1.0f));
	V = glm::inverse(C);
}

void Camera::Render(Scene & scene, bool parallel) {
	img = std::make_unique<Bitmap>(width, height);

	if (parallel) {
		unsigned numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;

		for (int i = 0; i < numThreads; i++) {
			threads.emplace_back(&Camera::RenderPixelsParallel, this, std::ref(scene));
		}

		for (auto & aThread : threads) {
			aThread.join();
		}
	}
	else {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				RenderPixel(x, y, scene);
			}
		}
	}
}

void Camera::RenderPixel(int x, int y, Scene &scene) {
	float fx = (float(x) + 0.5f) / width - 0.5f;
	float fy = (float(y) + 0.5f) / height - 0.5f;

	auto a = glm::vec3(C[0]);
	auto b = glm::vec3(C[1]);
	auto c = glm::vec3(C[2]);
	auto d = glm::vec3(C[3]);

	float scaleX = 2.0f * tanf(hFov / 2.0f);
	float scaleY = 2.0f * tanf(vFov / 2.0f);

	Ray ray;
	ray.Origin = d;
	ray.Direction = glm::normalize(fx * scaleX * a + fy * scaleY * b - c);

	Intersection hitData;
	if (scene.Intersect(ray, hitData)) {
		Color pixelColor = Color::BLACK;
		for (int i = 0; i < scene.GetNumLights(); i++) {
			Light & light = scene.GetLight(i);
			Color tempColor;
			glm::vec3 lightPos;
			glm::vec3 toLight;
			float intensity = light.Illuminate(hitData.Position, tempColor, toLight, lightPos);
			hitData.Mtl->ComputeReflectance(tempColor, toLight, glm::vec3(), hitData);

			pixelColor.AddScaled(tempColor, intensity);
		}

		img->SetPixel(x, y, pixelColor.ToInt());
	}
	else {
		img->SetPixel(x, y, scene.GetSkyColor().ToInt());
	}
}

void Camera::RenderPixelsParallel(Scene &scene) {
	while (PixelsRemaining()) {
		auto coord = GetNextPixel();
		RenderPixel(coord.first, coord.second, scene);
	}
}

void Camera::SaveBitmap(std::string filename) {
	img->SaveBMP(filename.c_str());
}

bool Camera::PixelsRemaining() {
	std::lock_guard<std::mutex> lock(queryMutex);
	return currX * currY < (width - 1) * (height - 1);
}

//TODO: Fix demand partitioning.
std::pair<int, int> Camera::GetNextPixel() {
	std::lock_guard<std::mutex> lock(queryMutex);
	currX = (currX + 1 < width) ? currX + 1 : 0;
	currY = (currX == 0) ? currY + 1 : currY;

	//    std::cerr << "(" << currX << ", " << currY << ")" << std::endl;

	return std::make_pair(currX, currY);
}