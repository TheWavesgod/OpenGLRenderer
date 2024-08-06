#include "Material.h"
#include "Shader.h"

Material::Material()
{

}

Material::Material(const std::string& albedo, const std::string& mettalic, const std::string& roughness, const std::string& normal, const std::string& height, const std::string& ao, const std::string& emissive)
{
	for (auto& i : textures) i = nullptr;

	if (!albedo.empty())
	{
		textures[TEXTYPE_ALBEDO] = new Texture(albedo, TEXTYPE_ALBEDO);
		filePathes[TEXTYPE_ALBEDO] = albedo;
	}
	if (!mettalic.empty())
	{
		textures[TEXTYPE_METALLIC] = new Texture(mettalic, TEXTYPE_METALLIC);
		filePathes[TEXTYPE_METALLIC] = mettalic;
	}
	if (!roughness.empty())
	{
		textures[TEXTYPE_ROUGHNESS] = new Texture(roughness, TEXTYPE_ROUGHNESS);
		filePathes[TEXTYPE_ROUGHNESS] = roughness;
	}
	if (!normal.empty())
	{
		textures[TEXTYPE_NORMAL] = new Texture(normal, TEXTYPE_NORMAL);
		filePathes[TEXTYPE_NORMAL] = normal;
	}
	if (!height.empty())
	{
		textures[TEXTYPE_HEIGHT] = new Texture(height, TEXTYPE_HEIGHT);
		filePathes[TEXTYPE_HEIGHT] = height;
	}
	if (!ao.empty())
	{
		textures[TEXTYPE_AO] = new Texture(ao, TEXTYPE_AO);
		filePathes[TEXTYPE_AO] = ao;
	}
	if (!emissive.empty())
	{
		textures[TEXTYPE_EMISSIVE] = new Texture(emissive, TEXTYPE_EMISSIVE);
		filePathes[TEXTYPE_EMISSIVE] = emissive;
	}

	bUsePBR = true;
}

Material::~Material()
{

}

void Material::Set(Shader& shader)
{
	if (bUsePBR)
	{
		SetShaderSampler(shader, TEXTYPE_ALBEDO, 0);
		SetShaderSampler(shader, TEXTYPE_METALLIC, 1);
		SetShaderSampler(shader, TEXTYPE_ROUGHNESS, 2);
		SetShaderSampler(shader, TEXTYPE_NORMAL, 3);
		SetShaderSampler(shader, TEXTYPE_HEIGHT, 4);
		SetShaderSampler(shader, TEXTYPE_AO, 5);
		SetShaderSampler(shader, TEXTYPE_EMISSIVE, 6);

		shader.SetUniformInt("useEmissive", textures[TEXTYPE_EMISSIVE] != nullptr);
		shader.SetUniformInt("useHeight", textures[TEXTYPE_HEIGHT] != nullptr);

		shader.SetUniformFloat("heightScale", heightScale);
		shader.SetUniformFloat("emissiveScale", emissiveScale);

		shader.SetUniformInt("irradianceMap", 7);
		shader.SetUniformInt("prefilterMap", 8);
		shader.SetUniformInt("brdfLUT", 9);
	}
	else
	{
		shader.SetUniformFloat("material.shininess", 32.0f);
	}
}

int Material::GetUseShaderIndex()
{
	if (bUsePBR) return 6;
	return 6;
}

void Material::SetShaderSampler(Shader& shader, TextureType type, int index)
{
	if (textures[type] == nullptr) return;

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, textures[type]->TextureID());
	std::string name = textures[type]->GetTypeName();	// TODO: Why string can't return string&
	shader.SetUniformInt("material." + name, index);
}
