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

using namespace std::chrono;

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
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);
	// Create camera
	Camera cam;
	cam.SetResolution(640, 480);
	cam.SetAspect(1.33f);
	cam.LookAt(glm::vec3(-0.5f, 0.25f, -0.2f), glm::vec3(0.0f, 0.15f, 0.0f));
	cam.SetFOV(40.0f);
	cam.SetSuperSample(10, 10);
	cam.SetJitter(true);
	cam.SetShirley(true);

	auto end = steady_clock::now();
	std::cerr << "Scene construction took "
		<< duration_cast<milliseconds> (end - begin).count()
		<< "ms" << std::endl;

	//----------------------------------------------------------
	// Render image
	begin = steady_clock::now();
	cam.Render(scn);
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
	std::system("open project2.bmp");
#endif // _WIN32

	//----------------------------------------------------------
	//Wait a few seconds to give time to read vals
	std::this_thread::sleep_for(milliseconds(3000));
}

void project2() {
	auto begin = steady_clock::now();
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));

	// Create ground
	MeshObject ground;
	ground.MakeBox(5.0f, 0.1f, 5.0f);
	scn.AddObject(ground);

	MeshObject tallBox;
	tallBox.MakeBox(1.0f, 4.0f, 1.0f);
	//scn.AddObject(tallBox);

	// Create dragon
	MeshObject dragon;
	dragon.LoadPLY("dragon.ply");
	dragon.Smooth();

	BoxTreeObject tree;
	tree.Construct(dragon);
	//tree.Construct(tallBox);

	scn.AddObject(tree);

	//// Create instance
	InstanceObject inst(tree);
	glm::mat4x4 mtx = glm::eulerAngleY(glm::pi<float>());
	mtx[3] = glm::vec4(-0.05f, 0.0f, -0.1f, 1.0f);
	inst.SetMatrix(mtx);
	scn.AddObject(inst);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(0.02f);
	redlgt.SetPosition(glm::vec3(-0.2f, 0.2f, 0.2f));
	scn.AddLight(redlgt);

	PointLight bluelgt;
	bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
	bluelgt.SetIntensity(0.06f);
	bluelgt.SetPosition(glm::vec3(0.1f, 0.1f, 0.3f));
	scn.AddLight(bluelgt);

	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(-0.1f, 0.1f, 0.2f), glm::vec3(-0.05f, 0.12f, 0.0f),
		glm::vec3(0, 1.0f, 0));
	//cam.BuildCamera(glm::vec3(2.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetFoV(40.0f);
	cam.SetAspect(1.33f);
	cam.SetResolution(1200, 800);

	auto end = steady_clock::now();
	std::cerr << "Scene construction took " << duration_cast<milliseconds> (end - begin).count() << "ms" << std::endl;

	// Render image
	begin = steady_clock::now();
	cam.Render(scn);
	end = steady_clock::now();
	std::cerr << "Render took " << duration_cast<milliseconds> (end - begin).count() << "ms" << std::endl;
	cam.SaveBitmap("project2.bmp");

#ifdef _WIN32
	std::system("project2.bmp");
#else
	std::system("open project2.bmp");
#endif // _WIN32

	//Wait a few seconds to give time to read vals
	std::this_thread::sleep_for(milliseconds(3000));
}

void project1() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Create boxes
	MeshObject box1;
	box1.MakeBox(5.0f, 0.1f, 5.0f);
	scn.AddObject(box1);

	MeshObject box2;
	box2.MakeBox(1.0f, 1.0f, 1.0f);

	InstanceObject inst1(box2);
	glm::mat4 mtx = glm::rotate(glm::mat4(), 0.5f, glm::vec3(1, 0, 0));
	mtx[3][1] = 1.0f;
	inst1.SetMatrix(mtx);
	scn.AddObject(inst1);

	InstanceObject inst2(box2);
	glm::mat4 mtx2 = glm::rotate(glm::mat4(), 1.0f, glm::vec3(0, 1, 0));
	mtx2[3] = glm::vec4(-1, 0, 1, 1);
	inst2.SetMatrix(mtx2);
	scn.AddObject(inst2);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(0.5f);
	sunlgt.SetDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(2.0f);
	redlgt.SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	scn.AddLight(redlgt);

	// Create camera
	Camera cam;
	cam.BuildCamera(glm::vec3(2.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	cam.SetResolution(800, 600);
	cam.SetFoV(40.0f);
	cam.SetAspect(1.33f);

	// Render image
	cam.Render(scn, false);
	cam.SaveBitmap("project1.bmp");

#ifdef _WIN32
	std::system("project1.bmp");
#else
	std::system("open project1.bmp");
#endif // _WIN32

	//Open the image
	std::system("open project1.bmp");
}

int main() {
	//srand(static_cast<unsigned>(time(0)));
	//project3();
	//project2();
	//project1();
	return 0;
}