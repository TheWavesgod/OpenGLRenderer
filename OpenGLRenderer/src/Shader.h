#pragma once

#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <string>
#include <vector>

class Shader
{
public:
	Shader(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc, const std::string& geometryShaderLoc = "");

	enum shaderType : uint32_t
	{
		E_VERTEX = 0,
		E_FRAGMENT,
		E_GEOMETRY,

		E_MAX
	};

	inline void Use() { glUseProgram(shaderProgramID); }

	// Set Shader Uniform Values
	void SetUniformVec3(const std::string& variable, float x, float y, float z);
	void SetUniformFloat(const std::string& variable, float val);
	void SetUniformMat4(const std::string& variable, const glm::mat4& val);
	void SetUniformInt(const std::string& variable, GLuint val);

	static inline Shader* GetShaderByIndex(int i) { return shaders[i]; }

protected:
	GLuint shaderProgramID;

	static std::vector<Shader*> shaders;

private:
	std::string shaderText;
	
	bool bCreateSuccess;

	bool BuildShaderProgram(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc, const std::string& geometryShaderLoc);
	bool ReadAndCompileShader(const std::string& fileLocation, shaderType type, GLuint* shaderID);

public:
	inline GLuint GetShaderProgram() const { return shaderProgramID; }
	inline bool HasInitialized() const { return bCreateSuccess; }
};

