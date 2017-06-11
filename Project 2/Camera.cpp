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

	if (!rayTracer) rayTracer = new RayTrace(scene);

	if (parallel) {
		//Use hyperthreading (# threads = 2 x # cores)
		//unsigned numThreads = std::thread::hardware_concurrency() * 2;
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

void Camera::RenderPixel(int aTile, Scene & scene)
{
	if (aTile < 0) return; //Sanity check
	//Compute the pixel bounds for the given tile.
	auto tile = tileCoords.at(aTile);
	int tileStartX = tile.first * tileWidth;
	int tileEndX = tileStartX + tileWidth;

	int tileStartY = tile.second * tileHeight;
	int tileEndY = tileStartY + tileHeight;

#ifdef DEBUG
	{
		std::lock_guard<std::mutex> lk(logMutex);
		std::cerr << "Thread " << std::this_thread::get_id() << " processing tile " << aTile
			<< "(" << tileStartX << ", "
			<< tileStartY << ")"
			<< std::endl;
	}
#endif // DEBUG

	//Iterate over pixels in tile
	for (int y = tileStartY; y < tileEndY; y++)
	{
		for (int x = tileStartX; x < tileEndX; x++)
		{
			if (x >= width || y >= height) break; //In case image dims not multiple of tile size
			RenderPixel(x, y, scene);
		}
	}
}

void Camera::RenderPixelsParallel(Scene &scene) {
	while (tileCoordIndex >= 0) {
		RenderPixel(tileCoordIndex--, scene);
	}
}

void Camera::SaveBitmap(std::string filename) {
	img->SaveBMP(filename.c_str());
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

void Camera::SetResolution(int x, int y) {
	width = x; height = y;
	//Calculate the number of tiles.
	int tileArea = tileWidth * tileHeight;
	numTilesX = (x + tileWidth - 1) / tileWidth; //round up
	numTilesY = (y + tileHeight - 1) / tileHeight; //round up
	for (int j = 0; j < numTilesY; j++) {
		for (int i = 0; i < numTilesX; i++) {
			tileCoords.push_back(std::make_pair(i, j));
		}
	}
	tileCoordIndex = tileCoords.size() - 1;
	SetAspect(float(width) / float(height));
}