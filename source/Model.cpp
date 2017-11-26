#include "Model.hpp"

#include <iostream>
////////////////////////////////////////////////////////////////////////////////

Model::~Model() {
	for(auto aMesh: m_meshes)
		delete aMesh;
	m_meshes.clear();
    for(auto aTexPair: m_textures)
        delete aTexPair.second;
    m_textures.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Model::Load(std::string path) {
	Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, 
    					aiProcess_Triangulate
                        | aiProcess_GenNormals
                        | aiProcess_JoinIdenticalVertices
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
    std::vector<Texture *> meshTextures;

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

	// Process Material
    aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

    // 3 categories of supported textures: diffuse, specular, & normal maps
    // 1. Diffuse maps
    auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. Specular maps
    auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
    // 3. Normal Maps
    auto normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS);
    meshTextures.insert(meshTextures.end(), normalMaps.begin(), normalMaps.end());


    return new MeshObject(vertices, indices, meshTextures);
}
////////////////////////////////////////////////////////////////////////////////

void Model::PrintInfo(std::ostream & stream) {
    stream << "This model contains " << NumMeshes() << ((NumMeshes() == 1) ? " mesh." : " meshes.") << std::endl;
    size_t count = 0;
    for(auto & aMesh: m_meshes) {
        stream << "\tMesh " << count++ << " has " << aMesh->GetNumVertexes() << " vertices, ";
        stream << aMesh->GetNumTriangles() * 3 << " indices, and ";
        stream << aMesh->GetNumTextures() << " textures." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Texture *> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type) {
    std::vector<Texture *> textures;
    textures.reserve(mat->GetTextureCount(type));

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        TEXTURE_TYPE texType;
        switch(type) {
            case aiTextureType_SPECULAR:
            texType = TEXTURE_TYPE::SPECULAR;
            break;

            case aiTextureType_NORMALS:
            texType = TEXTURE_TYPE::NORMAL;
            break;

            default:
            texType = TEXTURE_TYPE::DIFFUSE;
            break;
        }

        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texName(str.C_Str());

        Texture * tex;

        // Only load a new texture if this texture hasn't been seen before.
        if(m_textures.find(texName) == m_textures.end()) {
            tex = new Texture(texName, texType);
            m_textures.insert(std::make_pair(texName, tex));
        } else {
            tex = m_textures.at(texName);
        }
        textures.push_back(tex);
    }

    return textures; 
}


////////////////////////////////////////////////////////////////////////////////
