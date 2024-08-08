#pragma once

#include "Texture.h"
#include <glm.hpp>
#include <string>

class Shader;

class Material
{
public:
	Material(const std::string& albedo = "", const std::string& mettalic = "", const std::string& roughness = "",
		const std::string& normal = "", const std::string& height = "", const std::string& ao = "", const std::string& emissive = "");

	Material(bool transparent);

	~Material();

	static Material* CreateGlassMaterial(const std::string& name, const glm::vec3& baseColor, float alpha, float metallic, float roughness);

	void Set(Shader& shader);

	inline void SetUsePBR(bool val) { bUsePBR = val; }
	inline bool GetUsePBR() const { return bUsePBR; }

	inline void SetTransparent(bool val) { bTransparent = val; }
	inline bool GetTransparent() const { return bTransparent; }

	void SetName(const std::string& newName) { name = newName; }
	inline std::string& GetName() { return name; }

	inline float GetHeightScale() const { return heightScale; }
	inline float GetEmissiveScale() const { return emissiveScale; }
	void SetHeightScale(float val) { heightScale = val; }
	void SetEmissiveScale(float val) { emissiveScale = val; }
	const std::string* GetFilePathes() const { return filePathes; }

	int GetUseShaderIndex();

	/** Material Parameter */
	bool useAlbedo;
	glm::vec3 baseColor = glm::vec3(1.0f, 0.0f, 0.0f);
	bool useMetallic;
	float matellic = 0.0f;
	bool useRoughness;
	float roughness = 0.5f;
	bool useNormal;
	bool useHeight;
	bool useAO;
	bool useEmissive;
	float alpha = 0.5f;
	float heightScale = 0.2f;
	float emissiveScale = 1.0f;

protected:
	std::string name;

	Texture* textures[TextureType::TEXTYPE_MAX];
	std::string filePathes[TextureType::TEXTYPE_MAX];

	bool bUsePBR;
	bool bTransparent = false;

	void SetShaderSampler(Shader& shader, TextureType type, int index);
private:
	
};

