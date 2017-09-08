include_directories("../externals/assimp/include")
target_include_directories(Babylon PUBLIC ../include)

add_library(Babylon STATIC
			Bitmap.hpp BoxTreeNode.hpp BoxTreeObject.hpp
			Camera.hpp Color.hpp DirectLight.hpp
			FogVolume.hpp InstanceObject.hpp Intersection.hpp
			IsotropicScatter.hpp LambertMaterial.hpp Light.hpp
			LorenzMieScatter.hpp Material.hpp MeshObject.hpp
			MetalMaterial.hpp Object.hpp PointLight.hpp
			Ray.hpp RayTrace.hpp ScatterPhase.hpp 
			Scene.hpp Triangle.hpp Utilities.hpp
			Vertex.hpp Volume.hpp
			)
			
target_link_libraries(Babylon glm assimp)
