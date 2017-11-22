#include "Model.hpp"

#include <iostream>
////////////////////////////////////////////////////////////////////////////////

Model::~Model() {
	for(auto aMesh: m_meshes)
		delete aMesh;
	m_meshes.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Model::Load(std::string path) {
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

void Model::ProcessNode(aiNode *node, const aiScene *scene)
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

////////////////////////////////////////////////////////////////////////////////

MeshObject* Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
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

		// For now, only take the first set of texture coordinates.
        if(mesh->mTextureCoords[0]) {
            glm::vec2 texCoords;
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = texCoords;
        }

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

void Model::PrintInfo(std::ostream & stream) {
    stream << "This model contains " << NumMeshes() << ((NumMeshes() == 1) ? " mesh." : " meshes.") << std::endl;
    size_t count = 0;
    for(auto & aMesh: m_meshes) {
        stream << "\tMesh " << count << " has " << aMesh->GetNumVertexes() << " vertices";
        stream << " and " << aMesh->GetNumTriangles() * 3 << " indices." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        if(m_textureNames.find(std::string(str.C_Str())) == m_textureNames.end()) {
            m_textureNames.insert(std::string(str.C_Str()));
            m_textures.emplace(std::string(str.C_Str()));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////