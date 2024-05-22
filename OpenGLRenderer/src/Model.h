#pragma once

#include <string>
#include <vector>


class Shader;
class Mesh;
class aiNode;
class aiMesh;
class aiScene;
class aiMaterial;
enum aiTextureType;

namespace XGL
{
	class Texture;
}

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
	std::vector<XGL::Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<XGL::Texture> textures_loaded;
};

