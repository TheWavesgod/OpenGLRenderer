#include "Model.h"

#include "Mesh.h"
#include "Shader.h"

#include <iostream>
#include <cstring>
#include <utility>

Model::Model(const std::string& newName) : name(newName)
{
}

void Model::DrawToLightDepthMap()
{
	for (Mesh* mesh : meshes)
	{
		mesh->DrawToLightDepthMap();
	}
}

void Model::SetMaterial(Material* material)
{
	for (auto& mesh : meshes)
	{
		mesh->material = material;
	}
}

void Model::SetMaterialIndex(int slot, int index)
{
	if (slot >= materialIndices.size()) materialIndices.resize(slot + 1);
	
	materialIndices[slot] = index;
}

void Model::LoadModel(const std::string& path, bool flipVertical)
{
	/**
	 *  aiProcess_Triangulate: if the model does not (entirely) consist of triangles, it should transform all the model's primitive shapes to triangles first.
	 *  aiProcess_FlipUVs : flips the texture coordinates on the y-axis where necessary during processing (most images in OpenGL were reversed around the y-axis, this postprocessing option fixes that for us)
	 */
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	
	materialIndices.resize(scene->mNumMaterials);
	ProcessNode(scene->mRootNode, scene, flipVertical);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, bool flip)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(ProcessMesh(mesh, scene, flip));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, flip);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, bool flip)
{
	Mesh* m = new Mesh();

	m->vertices.resize(mesh->mNumVertices, glm::vec3());
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		// Retrieve position
		m->vertices[i].x = mesh->mVertices[i].x;
		m->vertices[i].y = mesh->mVertices[i].y;
		m->vertices[i].z = mesh->mVertices[i].z;
	}

	// Texture coordinates
	m->texCoords.resize(mesh->mNumVertices, glm::vec2(0.0f));
	if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			// Assimp allows a model to have up to 8 different texture coordinates per vertex. We're not going to use 8, we only care about the first set of texture coordinates.
			m->texCoords[i].x = mesh->mTextureCoords[0][i].x; 
			m->texCoords[i].y = flip ? 1- mesh->mTextureCoords[0][i].y : mesh->mTextureCoords[0][i].y;
		}
	}

	// Normals
	if (mesh->HasNormals())
	{
		m->normals.resize(mesh->mNumVertices, glm::vec3());
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			m->normals[i].x = mesh->mNormals[i].x;
			m->normals[i].y = mesh->mNormals[i].y;
			m->normals[i].z = mesh->mNormals[i].z;
		}
	}

	if (mesh->HasTangentsAndBitangents())
	{
		m->tangents.resize(mesh->mNumVertices, glm::vec3());
		m->biTangents.resize(mesh->mNumVertices, glm::vec3());
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			m->tangents[i].x = mesh->mTangents[i].x;
			m->tangents[i].y = mesh->mTangents[i].y;
			m->tangents[i].z = mesh->mTangents[i].z;

			m->biTangents[i].x = mesh->mBitangents[i].x;
			m->biTangents[i].y = mesh->mBitangents[i].y;
			m->biTangents[i].z = mesh->mBitangents[i].z;
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			m->indices.push_back(mesh->mFaces[i].mIndices[j]);
	}	

	if (mesh->mMaterialIndex >= 0)
	{
		m->parentMaterialIndex = mesh->mMaterialIndex;
	}

	m->SetupMesh();

	return m; 
}

void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textype)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (size_t j = 0; j < textures_loaded.size(); ++j)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.LoadFromFile(directory + "/" + std::string(str.C_Str()));
			texture.type = textype;
			texture.path = std::string(str.C_Str());
			textures.push_back(texture);
			textures_loaded.push_back(texture);	
		}
	}
}
