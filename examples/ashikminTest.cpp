#include <chrono>
#include <iostream>

#include "Camera.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "Scene.hpp"
#include "BoxTreeObject.hpp"
#include "AnisotropicPhongMaterial.hpp"
#include "Model.hpp"
#include <boost/simd/pack.hpp>

using namespace std::chrono;

int main(int argc, char* argv[]) {
    if(argc <= 1) {
        std::cout << "ERROR: No path for model given" << std::endl;
        return 1;
    }
    
    namespace bs = boost::simd;
    using pack_t = bs::pack<glm::vec3>;
    
    auto pack_size = bs::cardinal_of<pack_t>();
    std::cout << "cardinal: " << pack_size << std::endl;
    return 0;
    
    // ----------------------------------------------------------
    // Model Loading.
    std::string path(argv[1]);
    Model model(path);
    model.PrintInfo(std::cout);
    
    // ----------------------------------------------------------
    // Scene Construction
    Scene scene;
    scene.SetSkyColor(Color(0.8f, 0.85f, 0.95f));
    
    //Add lights
    DirectLight dLight;
    dLight.SetDirection(glm::vec3(0.f, -1.f, -1.f));
    scene.AddLight(dLight);
    
    //Construct BoxTreeObjects from Model's meshes
    std::vector<BoxTreeObject> treeObjects;
    treeObjects.reserve(model.NumMeshes());
    auto meshes = model.GetMeshes();
    for (int i = 0; i < model.NumMeshes(); ++i) {
        treeObjects.emplace_back();
        treeObjects.back().Construct(*meshes.at(i));
    }
    
    //Add BoxTrees to scene.
    for(auto & aBoxTree: treeObjects)
        scene.AddObject(aBoxTree);
    
    //Make a camera
    Camera cam;
//    cam.BuildCamera( glm::vec3(0, 0, 0.5f), glm::vec3(0, 0.f, 0), glm::vec3(0, 1.0f, 0));
    cam.BuildCamera( glm::vec3(0, 20.f, 20.f), glm::vec3(0, 10.f, 0), glm::vec3(0, 1.0f, 0));
    cam.SetResolution(800, 800);
    cam.SetFoV(45.f);
    cam.SetFocus(1.f);
    cam.SetfStop(20000.f);
    cam.SetSuperSample(20, 20);
    cam.SetJitter(true);
    cam.SetShirley(true);
    cam.SetMaxPathLength(10);
    
    // ----------------------------------------------------------
    // Render!
    auto begin = steady_clock::now();
    cam.Render(scene, true);
    auto end = steady_clock::now();
    std::cout << "Render took "
    << duration_cast<milliseconds> (end - begin).count() / 1000.f
    << "s" << std::endl;
    
    // ----------------------------------------------------------
    // Write image to file.
    cam.SaveBitmap("ashikminTest.bmp");
    // if(!cam.SaveImage("basic")) {
    //     std::cerr << "Failed to write image basic to file" << std::endl;
    // }
}
