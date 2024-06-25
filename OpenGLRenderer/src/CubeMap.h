#pragma once

#include "glad/glad.h"

#include <vector>
#include <string>

class Shader;

class CubeMap
{
public:
	CubeMap(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);

	CubeMap(const std::string& hdrFileLoc);

	void BindIrradianceMap(int i);
	void BindprefilterMap(int i);
	void BindBRDFLUT(int i);
	void Draw();

private:
	GLuint textureID;
	GLuint irrandianceMap;
	GLuint prefilterMap;
	GLuint brdfLUT;

	GLuint SkyboxVAO;
	GLuint hdrTextrue;

	GLuint captureFBO;
	GLuint captureRBO;

	std::vector<std::string> paths;
	
	void CreateCubeMapMesh();

	void LoadCubeMap();
	void LoadHDRFile();

	void GenerateCaptureBuffer();

	void ConvertHDRtoCubemap();
	void GenerateIrradianceMap();
	void GeneratePrefilterMap();
	void GenerateBRDFLUT();
};