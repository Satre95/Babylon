include_directories("../externals/assimp/include")


add_library(Babylon STATIC
			# Headers
			../include/Bitmap.hpp ../include/BoxTreeNode.hpp ../include/BoxTreeObject.hpp
			../include/Camera.hpp ../include/Color.hpp ../include/DirectLight.hpp
			../include/FogVolume.hpp ../include/InstanceObject.hpp ../include/Intersection.hpp
			../include/IsotropicScatter.hpp ../include/LambertMaterial.hpp ../include/Light.hpp
			../include/LorenzMieScatter.hpp ../include/Material.hpp ../include/MeshObject.hpp
			../include/MetalMaterial.hpp ../include/Object.hpp ../include/PointLight.hpp
			../include/Ray.hpp ../include/RayTrace.hpp ../include/ScatterPhase.hpp 
			../include/Scene.hpp ../include/Triangle.hpp ../include/Utilities.hpp
			../include/Vertex.hpp ../include/Volume.hpp
			# Source files
			Bitmap.cpp BoxTreeNode.cpp BoxTreeObject.cpp
			Camera.cpp Color.cpp DirectLight.cpp
			FogVolume.cpp InstanceObject.cpp LambertMaterial.cpp
			MeshObject.cpp MetalMaterial.cpp PointLight.cpp
			RayTrace.cpp Scene.cpp Triangle.cpp
			)
			
target_link_libraries(Babylon glm assimp)
target_include_directories(Babylon PUBLIC ../include)