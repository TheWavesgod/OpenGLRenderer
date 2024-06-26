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

protected:
	Texture* textures[TextureType::TEXTYPE_MAX];

	bool bUsePBR;

	void SetShaderSampler(Shader& shader, TextureType type, int index);
private:
	
};

