#pragma once

#include "Texture.h"
#include <string>

class Shader;

class Material
{
public:
	Material();

	Material(const std::string& albedo, const std::string& mettalic, const std::string& roughness, 
		const std::string& normal, const std::string& height = "", const std::string& ao = "", const std::string& emissive = "");

	~Material();

	void Set(Shader& shader);

	inline void SetUsePBR(bool val) { bUsePBR = val; }
	inline bool GetUsePBR() const { return bUsePBR; }

	void SetName(const std::string& newName) { name = newName; }
	inline std::string& GetName() { return name; }

	inline float GetHeightScale() const { return heightScale; }
	inline float GetEmissiveScale() const { return emissiveScale; }
	void SetHeightScale(float val) { heightScale = val; }
	void SetEmissiveScale(float val) { emissiveScale = val; }
	const std::string* GetFilePathes() const { return filePathes; }

protected:
	std::string name;
	/** Parameter */
	float heightScale = 0.2f;
	float emissiveScale = 1.0f;

	Texture* textures[TextureType::TEXTYPE_MAX];
	std::string filePathes[TextureType::TEXTYPE_MAX];

	bool bUsePBR;

	void SetShaderSampler(Shader& shader, TextureType type, int index);
private:
	
};

