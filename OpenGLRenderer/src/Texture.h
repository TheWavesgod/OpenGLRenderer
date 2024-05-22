#pragma once

#include "glad/glad.h"
#include <string>

namespace XGL 
{
	class Texture
	{
	public:
		Texture();

		Texture(const std::string& fileLoc);

		~Texture() {}

		void LoadFromFile(const std::string& fileLoc);

		std::string type;
		std::string path;

	protected:
		GLuint textureID;

	private:

	public:
		inline GLuint TextureID() const { return textureID; }
	};
}



