#include "ModelObject.hpp"

#include <iostream>
////////////////////////////////////////////////////////////////////////////////

ModelObject::~ModelObject() {
	for(auto aMesh: m_meshes)
		delete aMesh;
	m_meshes.clear();
}

////////////////////////////////////////////////////////////////////////////////

// TODO: Implement 
void ModelObject::Load(std::string path) {
	Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, 
    					aiProcess_Triangulate 
    					| aiProcess_GenNormals
    					| aiProcess_OptimizeMeshes
    					| aiProcess_OptimizeGraph);	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

////////////////////////////////////////////////////////////////////////////////

bool ModelObject::Intersect(const Ray &ray, Intersection &hit) {
	bool success = false;
	for(auto aMesh: m_meshes)
		if(aMesh->Intersect(ray, hit))
			success = true;

	return success;
}

void ModelObject::ProcessNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        m_meshes.push_back(ProcessMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}  

MeshObject* ModelObject::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
    std::vector<size_t> indices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 vector; 
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z; 
		vertex.Position = vector;
        
        vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;  

		//TODO: Load Texcoords

        vertices.push_back(vertex);
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
	    aiFace face = mesh->mFaces[i];
	    for(unsigned int j = 0; j < face.mNumIndices; j++)
	        indices.push_back(face.mIndices[j]);
	} 

	//TODO: Process Textures & Materials.

    return new MeshObject(vertices, indices);
}
////////////////////////////////////////////////////////////////////////////////
