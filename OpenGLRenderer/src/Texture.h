#pragma once

#include "glad/glad.h"
#include <string>
#include "Utilities.h"

enum TextureType {
	TEXTYPE_DIFFUSE = 0,
	TEXTYPE_SPECULAR,

	TEXTYPE_ALBEDO,
	TEXTYPE_METALLIC,
	TEXTYPE_ROUGHNESS,

	TEXTYPE_AO,
	TEXTYPE_NORMAL,
	TEXTYPE_HEIGHT,

	TEXTYPE_MAX
};

class Texture
{
public:
	Texture();

	Texture(const std::string& fileLoc, const TextureType type = TEXTYPE_DIFFUSE);

	~Texture() {}

	virtual void LoadFromFile(const std::string& fileLoc);

	TextureType type;
	std::string path;

	const std::string GetTypeName() const;

protected:
	GLuint textureID;

private:

public:
	inline GLuint TextureID() const { return textureID; }
};



