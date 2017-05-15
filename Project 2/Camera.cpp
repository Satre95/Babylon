//
//  Camera.cpp
//  Project 1
//
//  Created by Siddharth Atre on 4/10/17.
//  Copyright © 2017 Siddharth Atre. All rights reserved.
//

#include "Camera.hpp"
#include "Utilities.hpp"
#include "BoxTreeNode.hpp"
#include <vector>
#include <thread>
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, glm::vec2 dims) {
	SetResolution(int(dims.x), int(dims.y));
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

	if (!rayTracer) rayTracer = new RayTrace(scene, 5);

	if (parallel) {
		unsigned numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;

		for (size_t i = 0; i < numThreads; i++) {
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
	std::vector<Color> pixelColors;
	subPixelDims.first = 1.0f / superSamples.first / float(width);
	subPixelDims.second = 1.0f / superSamples.second / float(height);
	float halfSubPixelWidth = subPixelDims.first * 0.5f;
	float halfSubPixelHeight = subPixelDims.second * 0.5f;

	for (int v = 0; v < superSamples.second; v++) {
		for (int u = 0; u < superSamples.first; u++) {
			//TODO: Get rid of this hack-y fix to parallel synch problem.
			if (x >= width || y >= height) return;

			float subX = 0.5f, subY = 0.5f;

			if (jitterEnabled) JitterSubPixel(subX, subY);

			float subSampleY = (subY + v) / superSamples.second;
			float subSampleX = (subX + u) / superSamples.first;
			if (shirleyEnabled) ApplyShirleyWeight(subSampleX, subSampleY);

			//Get coords of center of subpixel
			float fx = (float(x) + subSampleX) / float(width);
			float fy = (float(y) + subSampleY) / float(height);

			//Subtract 0.5 to get in [-0.5, 0.5] range
			fx -= 0.5f;
			fy -= 0.5f;

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
			rayTracer->TraceRay(hitData, ray);

			pixelColors.push_back(hitData.Shade);
		}
	}

	//Average out the colors from the rays and save them to the image.
	img->SetPixel(x, y, Color::AverageColors(pixelColors).ToInt());
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

void Camera::JitterSubPixel(float & subX, float & subY) {
	subX = Utilities::randomFloatInRange(0.f, 1.f);
	subY = Utilities::randomFloatInRange(0.f, 1.f);
}

void Camera::ApplyShirleyWeight(float & s, float & t) {
	assert(s >= 0.f && s <= 1.f);
	assert(t >= 0.f && t <= 1.f);

	s = (s < 0.5f) ?
		(-0.5f + pow(2.f*s, 0.5f)) :
		(1.5f - pow(2.f - 2.f * s, 0.5f));

	t = (t < 0.5f) ?
		(-0.5f + pow(2.f*t, 0.5f)) :
		(1.5f - pow(2.f - 2.f*t, 0.5f));
}