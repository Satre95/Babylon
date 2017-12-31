include_directories(${INCLUDES})

set(HEADER_FILES ../include/Bitmap.hpp ../include/BoxTreeNode.hpp ../include/BoxTreeObject.hpp
		../include/Camera.hpp ../include/Color.hpp ../include/DirectLight.hpp
		../include/FogVolume.hpp ../include/InstanceObject.hpp ../include/Intersection.hpp
		../include/IsotropicScatter.hpp ../include/LambertMaterial.hpp ../include/Light.hpp
		../include/LorenzMieScatter.hpp ../include/Material.hpp ../include/MeshObject.hpp
		../include/MetalMaterial.hpp ../include/Object.hpp ../include/PointLight.hpp
		../include/Ray.hpp ../include/RayTrace.hpp ../include/ScatterPhase.hpp
		../include/Scene.hpp ../include/Triangle.hpp ../include/Utilities.hpp
		../include/Vertex.hpp ../include/Volume.hpp ../include/Model.hpp
		../include/Image.hpp ../include/Texture.hpp
		../include/AnisotropicPhongMaterial.hpp ../include/Rand.hpp
		../include/stb_image.h ../include/stb_image_write.h)

set( SOURCE_FILES Bitmap.cpp BoxTreeNode.cpp BoxTreeObject.cpp
		Camera.cpp Color.cpp DirectLight.cpp
		FogVolume.cpp InstanceObject.cpp LambertMaterial.cpp
		MeshObject.cpp MetalMaterial.cpp PointLight.cpp
		RayTrace.cpp Scene.cpp Triangle.cpp
		Model.cpp Texture.cpp Image.cpp
        AnisotropicPhongMaterial.cpp)

add_library(Babylon STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(Babylon PROPERTIES COMPILE_FLAGS "-mavx2")

target_link_libraries(Babylon glm assimp ${Boost_LIBRARIES})
