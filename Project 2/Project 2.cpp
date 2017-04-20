#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <glm/ext.hpp>

#include <iostream>
#include "InstanceObject.hpp"
#include "Camera.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "Scene.hpp"

int main()
{
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
	cam.Render(scn, true);
	cam.SaveBitmap("project2.bmp");

	//Open the image
#ifdef _WIN32
	std::system("project2.bmp");
#else
	std::system("open project1.bmp");
#endif // _WIN32

	return 0;
}