#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shader;
class Mesh;
//class aiNode;
//class aiMesh;
//class aiScene;
//class aiMaterial;
//enum aiTextureType;
class Texture;

class Model
{
public:
	Model(const std::string& filePath) { LoadModel(filePath); }

	void Draw(Shader& shader);

protected:
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<Texture> textures_loaded;
};

