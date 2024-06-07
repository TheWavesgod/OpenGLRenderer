#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Shader*> Shader::shaders;

Shader::Shader(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc, const std::string& geometryShaderLoc)
{
	bCreateSuccess = BuildShaderProgram(vertexShaderLoc, fragmentShaderLoc, geometryShaderLoc);

	shaders.push_back(this);
}

bool Shader::BuildShaderProgram(const std::string& vertexShaderLoc, const std::string& fragmentShaderLoc, const std::string& geometryShaderLoc)
{
	shaderProgramID = glCreateProgram();

	GLuint shaderIDs[E_MAX];
	int num = 0;
	if (!vertexShaderLoc.empty())
	{
		if (!ReadAndCompileShader(vertexShaderLoc, E_VERTEX, &shaderIDs[E_VERTEX])) return false;
		glAttachShader(shaderProgramID, shaderIDs[E_VERTEX]);
		++num;
	}
	if (!fragmentShaderLoc.empty())
	{
		if (!ReadAndCompileShader(fragmentShaderLoc, E_FRAGMENT, &shaderIDs[E_FRAGMENT])) return false;
		glAttachShader(shaderProgramID, shaderIDs[E_FRAGMENT]);
		++num;
	}
	if (!geometryShaderLoc.empty())
	{
		if (!ReadAndCompileShader(geometryShaderLoc, E_GEOMETRY, &shaderIDs[E_GEOMETRY])) return false;
		glAttachShader(shaderProgramID, shaderIDs[E_GEOMETRY]);
		++num;
	}

	glLinkProgram(shaderProgramID);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED from " << vertexShaderLoc << " and " << fragmentShaderLoc << std::endl << infoLog << std::endl;
		return false;
	}

	for (int i = 0; i < num; ++i)
	{
		glDeleteShader(shaderIDs[i]);
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

	case E_GEOMETRY:
		*shaderID = glCreateShader(GL_GEOMETRY_SHADER);
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
		cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<"File location: " << fileLocation << endl << infoLog << endl << endl;
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


