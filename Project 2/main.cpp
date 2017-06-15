#define GLM_FORCE_AVX2
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>

#include <iostream>
#include "InstanceObject.hpp"
#include "Camera.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "Scene.hpp"
#include "BoxTreeObject.hpp"
#include "LambertMaterial.hpp"
#include "MetalMaterial.hpp"
#include "Volume.hpp"
#include "FogVolume.hpp"

#ifdef _WIN32
#include <conio.h>
#endif // _WIN32

using namespace std::chrono;
void simplePointLightFogTest() {
	//----------------------------------------------------------
	/// Create scene
	auto begin = steady_clock::now();
	Scene scn;
	scn.SetSkyColor(Color(0.5f, 0.5f, 1.0f));

	//Add lights
	PointLight pLight1;
	pLight1.SetBaseColor(Color(3.0f, 1.3f, 0.4f));
	pLight1.SetPosition(glm::vec3(2.0f, 0, -5.f));
	pLight1.SetIntensity(5.0f);
	scn.AddLight(pLight1);

	PointLight pLight2;
	pLight2.SetBaseColor(Color(1.3f, 3.0f, 0.4f));
	pLight2.SetPosition(glm::vec3(-2.0f, 0, -5.0f));
	pLight2.SetIntensity(5.0f);
	scn.AddLight(pLight2);

	//----------------------------------------------------------
	//Add Volume
	FogVolume fog;
	fog.SetAbsroptionCoeff(Color(0.3f));
	fog.SetScatteringCoeff(Color(0.3f));
	scn.AddVolume(fog);

	//----------------------------------------------------------
	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(0.f, 0.f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetResolution(800, 800);
	cam.SetFoV(60.f);
	cam.SetFocus(0.5f);
	cam.SetfStop(20.f);
	cam.SetSuperSample(15, 15);
	cam.SetJitter(true);
	cam.SetShirley(true);

	auto end = steady_clock::now();
	std::cerr << "Scene construction took "
		<< duration_cast<milliseconds> (end - begin).count()
		<< "ms" << std::endl;

	//----------------------------------------------------------
	// Render image
	begin = steady_clock::now();
	cam.Render(scn, true, true);
	end = steady_clock::now();
	std::cerr << "Render took "
		<< duration_cast<seconds> (end - begin).count()
		<< "s" << std::endl;

	//----------------------------------------------------------
	//Save Image
	cam.SaveBitmap("simplePointLightFogTest.bmp");

	//----------------------------------------------------------
	//Open Image
#ifdef _WIN32
	std::system("simplePointLightFogTest.bmp");
	std::cout << "Press any key to exit" << std::endl;
	while (true) {
		if (_kbhit() != 0) break;
	}
#else
	std::system("open -a Fragment simpleBoxAndFogTest.bmp");
	std::this_thread::sleep_for(milliseconds(5000));
#endif // _WIN32
}
void simpleBoxFogTest()
{
	//----------------------------------------------------------
	/// Create scene
	auto begin = steady_clock::now();
	Scene scn;
	scn.SetSkyColor(Color(0.5f, 0.5f, 1.0f));

	// Create boxes
	MeshObject box;
	box.MakeBox(2.0f, 2.0f, 2.0f);

	LambertMaterial greenMat;
	greenMat.SetColor(Color(0.f, 0.5f, 0.2f));
	InstanceObject centerBox(box);
	centerBox.SetMatrix(glm::mat4(1.0f));
	centerBox.SetMaterial(&greenMat);
	scn.AddObject(centerBox);

	//Add lights
	PointLight pLight;
	pLight.SetBaseColor(Color(3.0f, 1.3f, 0.4f));
	pLight.SetPosition(glm::vec3(0, 0, -5.f));
	pLight.SetIntensity(5.0f);
	scn.AddLight(pLight);

	//----------------------------------------------------------
	//Add Volume
	FogVolume fog;
	fog.SetAbsroptionCoeff(Color(0.3f));
	fog.SetScatteringCoeff(Color(0.3f));
	scn.AddVolume(fog);

	//----------------------------------------------------------
	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(0.f, 0.f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetResolution(800, 800);
	cam.SetFoV(60.f);
	cam.SetFocus(0.5f);
	cam.SetfStop(20.f);
	cam.SetSuperSample(15, 15);
	cam.SetJitter(true);
	cam.SetShirley(true);

	auto end = steady_clock::now();
	std::cerr << "Scene construction took "
		<< duration_cast<milliseconds> (end - begin).count()
		<< "ms" << std::endl;

	//----------------------------------------------------------
	// Render image
	begin = steady_clock::now();
	cam.Render(scn, true, true);
	end = steady_clock::now();
	std::cerr << "Render took "
		<< duration_cast<seconds> (end - begin).count()
		<< "s" << std::endl;

	//----------------------------------------------------------
	//Save Image
	cam.SaveBitmap("simpleBoxAndFogTest.bmp");

	//----------------------------------------------------------
	//Open Image
#ifdef _WIN32
	std::system("simpleBoxAndFogTest.bmp");
	std::cout << "Press any key to exit" << std::endl;
	while (true) {
		if (_kbhit() != 0) break;
	}
#else
	std::system("open -a Fragment simpleBoxAndFogTest.bmp");
	std::this_thread::sleep_for(milliseconds(5000));
#endif // _WIN32
}

int main() {
	simpleBoxFogTest();
	simplePointLightFogTest();
	return 0;
}