#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Utilities.h"
#include "Texture.h"

class Shader;
class Mesh;
class Material;
class Transform;

class Model
{
public:
	Model(const std::string& newName);
	Model(const std::string& newName, const std::string& filePath) : name(newName) { LoadModel(filePath); }

	void DrawToLightDepthMap();

	void SetMaterial(Material* material);
	void SetMaterialIndex(int slot, int index);

	std::vector<Mesh*>& GetMeshes() { return meshes; }
	
	std::vector<int>* GetMaterialIndices() { return &materialIndices; }

	std::string name;

protected:
	std::vector<Mesh*> meshes;
	std::vector<int> materialIndices;
	std::string directory;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textype);

private:
	std::vector<Texture> textures_loaded;
};

