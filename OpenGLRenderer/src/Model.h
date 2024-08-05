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

class Model
{
public:
	Model(const std::string& filePath) { LoadModel(filePath); }

	void Draw(Shader& shader);
	void DrawToLightDepthMap();

	void SetShaderIndex(unsigned int index);
	unsigned int shaderIndex;

	void SetMaterial(Material* material);

protected:
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	std::string directory;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textype);

private:
	std::vector<Texture> textures_loaded;
};

