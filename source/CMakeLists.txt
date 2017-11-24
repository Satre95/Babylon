include_directories("../externals/assimp/include")
set( SOURCE_FILES Bitmap.cpp BoxTreeNode.cpp BoxTreeObject.cpp
			Camera.cpp Color.cpp DirectLight.cpp
			FogVolume.cpp InstanceObject.cpp LambertMaterial.cpp
			MeshObject.cpp MetalMaterial.cpp PointLight.cpp
			RayTrace.cpp Scene.cpp Triangle.cpp
            Model.cpp Texture.cpp Image.cpp
            TextureMaterial.cpp)

add_library(Babylon STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Babylon glm assimp ${Boost_LIBRARIES})
target_include_directories(Babylon PUBLIC ../include ${Boost_INCLUDE_DIRS} ${EXTERNALS_DIR}/boost.simd/include)
