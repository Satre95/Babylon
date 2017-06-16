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
	pLight1.SetBaseColor(Color(3.0f, 0.f, 0.f));
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
	fog.SetAbsroptionCoeff(Color(0.4f));
	fog.SetScatteringCoeff(Color(0.4f));
	scn.AddVolume(fog);

	//----------------------------------------------------------
	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(0.f, 0.f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetResolution(100, 100);
	cam.SetFoV(60.f);
	cam.SetFocus(0.5f);
	cam.SetfStop(20.f);
	cam.SetSuperSample(1, 1);
	cam.SetJitter(true);
	cam.SetShirley(true);
	cam.SetMaxPathLength(3);

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
#else
	std::system("open -a Fragment simpleBoxAndFogTest.bmp");
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

	BoxTreeObject boxTree;
	boxTree.Construct(box);
	InstanceObject centerBox(boxTree);

	centerBox.SetMatrix(glm::mat4(1.0f));
	centerBox.SetMaterial(&greenMat);

	scn.AddObject(centerBox);

	//Add lights
	PointLight pLight;
	pLight.SetBaseColor(Color(1.0f, 1.3f, 2.0f));
	pLight.SetPosition(glm::vec3(0, 0, -2.1f));
	pLight.SetIntensity(10.0f);
	scn.AddLight(pLight);

	//----------------------------------------------------------
	//Add Volume
	//FogVolume fog;
	//fog.SetAbsroptionCoeff(Color(0.3f));
	//fog.SetScatteringCoeff(Color(0.15f));
	//scn.AddVolume(fog);

	//----------------------------------------------------------
	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(0.f, 0.f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetResolution(1000, 1000);
	cam.SetFoV(60.f);
	cam.SetFocus(0.5f);
	cam.SetfStop(2.f);
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
	cam.Render(scn, true, false);
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
#else
	std::system("open -a Fragment simpleBoxAndFogTest.bmp");
#endif // _WIN32
}

void simpleDragonFogTest() {
	//----------------------------------------------------------
	//Construct Scene
	auto begin = steady_clock::now();
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Create ground
	LambertMaterial groundMtl;
	groundMtl.SetColor(Color(0.25f, 0.25f, 0.25f));
	MeshObject ground;
	ground.MakeBox(2.0f, 0.11f, 2.0f, &groundMtl);
	scn.AddObject(ground);

	// Load dragon mesh
	MeshObject dragon;
	dragon.LoadPLY("happy_buddha.ply");

	// Create box tree
	BoxTreeObject tree;
	tree.Construct(dragon);

	// Materials
	LambertMaterial diffuseMtl;
	diffuseMtl.SetColor(Color(0.35f, 0.64f, 0.54f));

	// Create dragon instances
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	glm::mat4 finalMat = glm::translate(scale, glm::vec3(0.f, -0.05, 0.5f));

	InstanceObject inst(tree);
	inst.SetMatrix(finalMat);
	inst.SetMaterial(&diffuseMtl);
	scn.AddObject(inst);

	LambertMaterial diffuseMtl2;
	diffuseMtl2.SetColor(Color(0.66f, 0.2f, 0.35f));

	scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
	finalMat = glm::translate(scale, glm::vec3(0.f, -0.05f, -0.1f));

	InstanceObject inst2(tree);
	inst2.SetMatrix(finalMat);
	inst2.SetMaterial(&diffuseMtl2);
	scn.AddObject(inst2);

	// Create lights
    PointLight rightLight;
    rightLight.SetBaseColor(Color(0.8f, 0.4f, 0.f));
    rightLight.SetIntensity(8.0f);
    rightLight.SetPosition(glm::vec3(1.5f, 0.f, 0.f));
    scn.AddLight(rightLight);
    
    PointLight leftLight;
    leftLight.SetBaseColor(Color(0.4f, 0.8f, 0.f));
    leftLight.SetIntensity(8.0f);
    leftLight.SetPosition(glm::vec3(1.5f, 0.f, 0.f));
    scn.AddLight(leftLight);

	// Create camera
	Camera cam;
	cam.SetResolution(400, 400);
	cam.BuildCamera(glm::vec3(0.0f, 0.6f, 1.3f), glm::vec3(0.0f, 0.f, 0.0f),
		glm::vec3(0, 1, 0));
	cam.SetFoV(50.0f);
	cam.SetMaxPathLength(3);
	cam.SetSuperSample(3, 3);
	cam.SetJitter(true);
	cam.SetShirley(true);
	cam.SetFocus(1.f);
	cam.SetfStop(80.0f);

	auto end = steady_clock::now();
	std::cerr << "Scene construction took "
		<< duration_cast<milliseconds> (end - begin).count()
		<< "ms" << std::endl;

	//----------------------------------------------------------
	// Render image
	begin = steady_clock::now();
	cam.Render(scn, true);
	end = steady_clock::now();
	std::cerr << "Render took "
		<< duration_cast<seconds> (end - begin).count()
		<< "s" << std::endl;

	//----------------------------------------------------------
	//Save Image
	cam.SaveBitmap("simpleDragonFogTest.bmp");

	//----------------------------------------------------------
	//Open Image
#ifdef _WIN32
	std::system("simpleDragonFogTest.bmp");
#else
	std::system("open -a Fragment simpleDragonFogTest.bmp");
#endif // _WIN32
}

int main() {
	simpleDragonFogTest();

#ifdef _WIN32
	std::cout << "Press any key to exit" << std::endl;
	while (true) {
		if (_kbhit() != 0) break;
	}
#else
	std::this_thread::sleep_for(milliseconds(5000));
#endif // _WIN32
	return 0;
}
