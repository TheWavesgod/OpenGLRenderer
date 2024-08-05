#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>


Texture::Texture()
{
	textureID = 0;
	type = TEXTYPE_DIFFUSE;
	path = "";
}

Texture::Texture(const std::string& fileLoc, const TextureType type)
{
	this->type = type;
	LoadFromFile(fileLoc);
}

void Texture::LoadFromFile(const std::string& fileLoc)
{
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileLoc.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum internalFormat;
		GLenum format;

		switch (nrChannels)
		{
		case 1:
			internalFormat = GL_RED;
			format = GL_RED;
			break;

		case 3:
			internalFormat = GL_RGB;
			format = GL_RGB;
			break;

		case 4:
			internalFormat = GL_RGBA;
			format = GL_RGBA;
			break;
		}
		if (type == TEXTYPE_ALBEDO || type == TEXTYPE_DIFFUSE)
		{
			if (nrChannels == 3)
				internalFormat = GL_SRGB;
			else if (nrChannels == 4)
				internalFormat = GL_SRGB_ALPHA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture at path: "<< fileLoc << std::endl;
	}
	stbi_image_free(data);
}

const std::string Texture::GetTypeName() const
{
	std::string name;
	switch (type)
	{
	case TEXTYPE_DIFFUSE:
		name = "diffuse";
		break;

	case TEXTYPE_SPECULAR:
		name = "specular";
		break;

	case TEXTYPE_ALBEDO:
		name = "albedo";
		break;

	case TEXTYPE_METALLIC:
		name = "metallic";
		break;

	case TEXTYPE_ROUGHNESS:
		name = "roughness";
		break;

	case TEXTYPE_AO:
		name = "ao";
		break;

	case TEXTYPE_NORMAL:
		name = "normal";
		break;

	case TEXTYPE_HEIGHT:
		name = "height";
		break;

	case TEXTYPE_EMISSIVE:
		name = "emissive";
		break;
	}

	return name;
}

std::string Texture::GetTypeString(TextureType type)
{
	std::string name;
	switch (type)
	{
	case TEXTYPE_DIFFUSE:
		name = "diffuse";
		break;

	case TEXTYPE_SPECULAR:
		name = "specular";
		break;

	case TEXTYPE_ALBEDO:
		name = "albedo";
		break;

	case TEXTYPE_METALLIC:
		name = "metallic";
		break;

	case TEXTYPE_ROUGHNESS:
		name = "roughness";
		break;

	case TEXTYPE_AO:
		name = "ao";
		break;

	case TEXTYPE_NORMAL:
		name = "normal";
		break;

	case TEXTYPE_HEIGHT:
		name = "height";
		break;

	case TEXTYPE_EMISSIVE:
		name = "emissive";
		break;
	}

	return name;
}
