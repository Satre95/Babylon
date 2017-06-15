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

using namespace std::chrono;

void finalProject()
{
	//----------------------------------------------------------
	/// Create scene
	auto begin = steady_clock::now();
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Create boxes
	MeshObject box;
	box.MakeBox(1.0f, 1.0f, 1.0f);

	LambertMaterial mtl1;
	mtl1.SetColor(Color(0.4f, 0.8f, 0.2f));
	InstanceObject box1(box);
	box1.SetMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 5.0f)));
	box1.SetMaterial(&mtl1);
	scn.AddObject(box1);

	LambertMaterial mtl2;
	mtl2.SetColor(Color(0.7f, 0.2f, 0.2f));
	InstanceObject box2(box);
	box2.SetMatrix(
		glm::translate(
			glm::scale(
				glm::mat4(1.0f),
				glm::vec3(5.0f, 1.0f, 1.0f)
			),
			glm::vec3(0.f, 1.1f, 0.f)
		)
	);
	box2.SetMaterial(&mtl2);
	scn.AddObject(box2);

	//	MeshObject dragon;
	//	dragon.LoadPLY("dragon.ply");

	//	InstanceObject dragonInstance(dragon);
	//	dragonInstance.SetMatrix(glm::scale(glm::mat4(), glm::vec3(2.0f)));
	//	scn.AddObject(dragonInstance);

		// Create lights
	PointLight pointLightRight;
	pointLightRight.SetBaseColor(Color(1.f, 0.f, 0.f).Scale(3.f));
	pointLightRight.SetIntensity(2.0f);
	pointLightRight.SetPosition(glm::vec3(5.0f, 3.f, 0.f));
	scn.AddLight(pointLightRight);

	PointLight pointLightLeft;
	pointLightLeft.SetBaseColor(Color(0.2f, 0.65f, 0.2f).Scale(2.f));
	pointLightLeft.SetIntensity(2.0f);
	pointLightLeft.SetPosition(glm::vec3(-5.0f, 3.f, 0.f));
	scn.AddLight(pointLightLeft);

	PointLight pointLightFront;
	pointLightLeft.SetBaseColor(Color(0.2f, 0.8f, 0.65f).Scale(4.f));
	pointLightLeft.SetIntensity(2.0f);
	pointLightLeft.SetPosition(glm::vec3(0, 3.f, 5.f));
	scn.AddLight(pointLightFront);

	PointLight pointLightBack;
	pointLightBack.SetBaseColor(Color(0.2f, 0.2f, 0.2f).Scale(2.f));
	pointLightBack.SetIntensity(2.f);
	pointLightBack.SetPosition(glm::vec3(0, 3.f, -5.f));
	scn.AddLight(pointLightBack);

	//----------------------------------------------------------
	//Add Volume
	FogVolume fog;
	fog.SetAbsroptionCoeff(Color(0.3f));
	fog.SetScatteringCoeff(Color(0.3f));
	scn.AddVolume(fog);

	//----------------------------------------------------------
	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(3.0f, 4.f, 3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetResolution(200,200);
	cam.SetFoV(60.f);
	cam.SetFocus(0.3f);
	cam.SetfStop(200000.f);
	cam.SetSuperSample(1, 1);
	cam.SetJitter(true);
	cam.SetShirley(true);

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
		<< duration_cast<milliseconds> (end - begin).count()
		<< "ms" << std::endl;

	//----------------------------------------------------------
	//Save Image
	cam.SaveBitmap("finalProject.bmp");

	//----------------------------------------------------------
	//Open Image
#ifdef _WIN32
	std::system("finalProject.bmp");
#else
	std::system("open finalProject.bmp");
#endif // _WIN32

	//----------------------------------------------------------
	//Wait a few seconds to give time to read vals
	std::this_thread::sleep_for(milliseconds(5000));
}

void project3() {
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
	dragon.LoadPLY("dragon.ply");
	// Create box tree
	BoxTreeObject tree;
	tree.Construct(dragon);
	// Materials
	LambertMaterial white;
	white.SetColor(Color(0.7f, 0.7f, 0.7f));
	LambertMaterial red;
	red.SetColor(Color(0.7f, 0.1f, 0.1f));
	MetalMaterial metal;
	metal.SetColor(Color(0.95f, 0.64f, 0.54f));
	const int numDragons = 4;
	Material *mtl[numDragons] = { &white,&metal,&red,&white };
	// Create dragon instances
	glm::mat4 mtx;
	for (int i = 0; i < numDragons; i++) {
		InstanceObject *inst = new InstanceObject(tree);
		mtx[3] = glm::vec4(0.0f, 0.0f, 0.3f*(float(i) / float(numDragons - 1) - 0.5f), 1.0f);
		inst->SetMatrix(mtx);
		inst->SetMaterial(mtl[i]);
		scn.AddObject(*inst);
	}
	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(0.8f);
	sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);
	// Create camera
	Camera cam;
	cam.SetResolution(200, 200);
	cam.SetAspect(1.33f);
	cam.BuildCamera(0.45f * glm::vec3(-0.5f, 0.25f, -0.2f), glm::vec3(0.0f, 0.15f, 0.0f),
		glm::vec3(0, 1, 0));
	cam.SetFoV(40.0f);
	cam.SetSuperSample(2, 2);
	cam.SetJitter(true);
	cam.SetShirley(true);
	cam.SetFocus(0.2f);
	cam.SetfStop(30.0f);

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
		<< duration_cast<milliseconds> (end - begin).count()
		<< "ms" << std::endl;

	//----------------------------------------------------------
	//Save Image
	cam.SaveBitmap("project3.bmp");

	//----------------------------------------------------------
	//Open Image
#ifdef _WIN32
	std::system("project3.bmp");
#else
	std::system("open project3.bmp");
#endif // _WIN32

	//----------------------------------------------------------
	//Wait a few seconds to give time to read vals
	std::this_thread::sleep_for(milliseconds(5000));
}

int main() {
	finalProject();
	return 0;
}
