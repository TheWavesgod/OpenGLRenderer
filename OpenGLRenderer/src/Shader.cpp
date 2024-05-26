#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc)
{
	bCreateSuccess = BuildShaderProgram(vertexShaderLoc, fragmentShaderLoc);
}

bool Shader::BuildShaderProgram(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc)
{
	shaderProgramID = glCreateProgram();

	GLuint shaderIDs[E_MAX];
	if (!vertexShaderLoc.empty())
	{
		if (!ReadAndCompileShader(vertexShaderLoc, E_VERTEX, &shaderIDs[E_VERTEX])) return false;
		glAttachShader(shaderProgramID, shaderIDs[E_VERTEX]);

	}
	if (!fragmentShaderLoc.empty())
	{
		if (!ReadAndCompileShader(fragmentShaderLoc, E_FRAGMENT, &shaderIDs[E_FRAGMENT])) return false;
		glAttachShader(shaderProgramID, shaderIDs[E_FRAGMENT]);
	}

	glLinkProgram(shaderProgramID);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		return false;
	}

	for (auto& i : shaderIDs)
	{
		glDeleteShader(i);
	}

	return true;
}

bool Shader::ReadAndCompileShader(const std::string& fileLocation, shaderType type, GLuint* shaderID)
{
	using namespace std;

	ifstream shaderFile(fileLocation, ios::in);

	if (!shaderFile)
	{
		std::cout << "Failed to open shader file from " << fileLocation << endl;
		return false;
	}

	stringstream buffer;

	buffer << shaderFile.rdbuf();

	shaderText = buffer.str();

	//cout << shaderText << endl;

	shaderFile.close();

	switch (type)
	{
	case E_VERTEX:
		*shaderID = glCreateShader(GL_VERTEX_SHADER);
		break;

	case E_FRAGMENT:
		*shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}
	const char* shaderSource = shaderText.c_str();
	glShaderSource(*shaderID, 1, &shaderSource, nullptr);
	glCompileShader(*shaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shaderID, 512, nullptr, infoLog);
		cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl << endl;
		return false;
	}
	shaderText.clear();

	return success;
}

void Shader::SetUniformVec3(const std::string& variable, float x, float y, float z)
{
	GLuint valLoc = glGetUniformLocation(shaderProgramID, variable.c_str());
	if (valLoc == -1)
	{
		std::cerr << "Couldn't find uniform variable " << variable << std::endl;
		return;
	}
	glUniform3f(valLoc, x, y, z);
}

void Shader::SetUniformFloat(const std::string& variable, float val)
{
	GLuint valLoc = glGetUniformLocation(shaderProgramID, variable.c_str());
	if (valLoc == -1)
	{
		std::cerr << "Couldn't find uniform variable " << variable << std::endl;
		return;
	}
	glUniform1f(valLoc, val);
}

void Shader::SetUniformMat4(const std::string& variable, const glm::mat4& val)
{
	GLuint valLoc = glGetUniformLocation(shaderProgramID, variable.c_str());
	if (valLoc == -1)
	{
		std::cerr << "Couldn't find uniform variable " << variable << std::endl;
		return;
	}
	glUniformMatrix4fv(valLoc, 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::SetUniformInt(const std::string& variable, GLuint val)
{
	GLuint valLoc = glGetUniformLocation(shaderProgramID, variable.c_str());
	if (valLoc == -1)
	{
		std::cerr << "Couldn't find uniform variable " << variable << std::endl;
		return;
	}
	glUniform1i(valLoc, val);
}