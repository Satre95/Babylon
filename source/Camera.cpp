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
#include <iostream>
#include <sstream>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, glm::vec2 dims) {
	SetResolution(int(dims.x), int(dims.y));
	BuildCamera(pos, target, up);
}

void Camera::BuildCamera(glm::vec3 pos, glm::vec3 target, glm::vec3  up) {
	auto d = pos;
	auto c = glm::normalize(d - target); // z-axis in world space
	auto a = glm::normalize(glm::cross(up, c)); //x axis in world space
	auto b = glm::cross(c, a); // y-axis in world space

	C = glm::mat4(glm::vec4(a, 0), glm::vec4(b, 0), glm::vec4(c, 0), glm::vec4(d, 1.0f));
	V = glm::inverse(C);
}

void Camera::Render(const Scene & scene, bool parallel) {
	img = std::make_unique<Bitmap>(width, height);
	finished = false;
	previewThreadWriting = false;
	finishedTiles = 0;
	// this->showProgress = showProgress;

	std::cout << tileCoords.size() << " tiles in " << width
		<< "x" << height << " image." << std::endl;

	//previewThread = std::thread(&Camera::PreviewImageFunc, this);

	rayTracer = std::make_unique<RayTrace>(scene, maxPathLength);

	if (parallel) {
		//Use hyperthreading (# threads = 2 x # cores)
		unsigned numThreads = std::thread::hardware_concurrency() * 2;
		//unsigned numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread *> threads;

		for (size_t i = 0; i < numThreads; i++) {
			std::thread * t = new std::thread([scene, this] { RenderPixelsParallel(&scene); } );
			threads.push_back(t);
		}

		for (auto & aThread : threads) {
			aThread->join();
		}
	}
	else {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				RenderPixel(x, y, &scene);
			}
		}
	}

	finished = true;
	previewThreadCV.notify_all();
	// previewThread.join();
}

void Camera::RenderPixel(int x, int y, const Scene * scenePtr) {
	const Scene & scene = *scenePtr;
	std::vector<Color> pixelColors;
	subPixelDims.first = 1.0f / superSamples.first / float(width);
	subPixelDims.second = 1.0f / superSamples.second / float(height);

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
			ray.Direction = glm::normalize(fx * scaleX * a + fy * scaleY * b - (focalPlane *c));

			//Randomize the origin point around the camera aperture
			float aperture = focalPlane / fStop;
			float displacement = aperture / 2.f;
			glm::vec3 randX = a * Utilities::randomFloatInRange(-displacement, displacement);
			glm::vec3 randY = b * Utilities::randomFloatInRange(-displacement, displacement);
			ray.Origin += (randX + randY);

			Intersection hitData;
			rayTracer->TraceRay(hitData, ray);

			pixelColors.push_back(hitData.Shade);
		}
	}

	//Average out the colors from the rays and save them to the image.
	img->SetPixel(x, y, Color::AverageColors(pixelColors).ToInt());
}

void Camera::RenderTile(int aTile, const Scene * scenePtr)
{
	const Scene & scene = *scenePtr;
	if (aTile < 0) return; //Sanity check
	//Compute the pixel bounds for the given tile.
	auto tile = tileCoords.at(aTile);
	int tileStartX = tile.first * tileWidth;
	int tileEndX = tileStartX + tileWidth;

	int tileStartY = tile.second * tileHeight;
	int tileEndY = tileStartY + tileHeight;

	//Iterate over pixels in tile
	for (int y = tileStartY; y < tileEndY; y++)
	{
		for (int x = tileStartX; x < tileEndX; x++)
		{
			if (x >= width || y >= height) break; //In case image dims not multiple of tile size
			RenderPixel(x, y, scenePtr);
		}
	}

	//report finishing the tile and wake up the preview thread
	finishedTiles++;
	previewThreadCV.notify_one();
}

void Camera::RenderPixelsParallel(const Scene * scenePtr) {
	while (tileCoordIndex >= 0) {
		RenderTile(tileCoordIndex--, scenePtr);
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
	numTilesX = (x + tileWidth - 1) / tileWidth; //round up
	numTilesY = (y + tileHeight - 1) / tileHeight; //round up
	for (int j = 0; j < numTilesY; j++) {
		for (int i = 0; i < numTilesX; i++) {
			tileCoords.push_back(std::make_pair(i, j));
		}
	}
	tileCoordIndex = int(tileCoords.size()) - 1;
	SetAspect(float(width) / float(height));

	numTilesPerBlock = 0.20f * float(tileCoords.size());
}

void Camera::PreviewImageFunc()
{
	using namespace std::chrono;

	static int previewNum = 0;
	while (!finished)
	{
		std::unique_lock<std::mutex> lk(previewMutex);
		previewThreadCV.wait(lk, [this]
		{
			if (finished) return true;

			if (finishedTiles == numTilesPerBlock) {
				finishedTiles = 0;
				std::cout << "Finished Rendering 20% of image" << std::endl;
				return true;
			}
			return false;
		});

		/*
		if (showProgress)
		{
			previewNum++;
			previewThreadWriting = true;
			std::stringstream ss;
			ss << "tempPreview_" << previewNum << ".bmp";
			img->SaveBMP(ss.str().c_str());
			previewThreadWriting = false;
#ifdef _WIN32
			std::system(ss.str().c_str());
#else
			std::stringstream ss2;
			ss2 << "open -a Fragment " << ss.str();
			std::system(ss2.str().c_str());
#endif
		}
		*/
		lk.unlock();
	}
#ifdef _WIN32
	//Delete the temporary images
	std::system("del tempPreview*");
#else
    std::system("rm -f tempPreview*");
#endif
    
}
