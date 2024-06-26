#pragma once

#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	inline void SetUsePBR(bool val) { bUsePBR = val; }

protected:
	Texture textures[TextureType::TEXTYPE_MAX];

	bool bUsePBR;

private:
	
};

