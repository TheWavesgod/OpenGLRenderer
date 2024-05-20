#pragma once

#include "glad/glad.h"
#include<string>

class shader
{
public:
	shader(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc);

	enum shaderType : uint32_t
	{
		E_VERTEX = 0,
		E_FRAGMENT,

		E_MAX
	};

protected:
	GLuint shaderProgramID;

private:
	std::string shaderText;
	
	bool bCreateSuccess;

	bool BuildShaderProgram(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc);
	bool ReadAndCompileShader(const std::string& fileLocation, shaderType type, GLuint* shaderID);

public:
	inline GLuint GetShaderProgram() const { return shaderProgramID; }
	inline bool HasInitialized() const { return bCreateSuccess; }
};

