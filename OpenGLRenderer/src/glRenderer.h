#pragma once

#include <vector>

#include "glad/glad.h"
#include "glfw3.h"

class window;
class Shader;
class Level;

class glRenderer
{
public:
	glRenderer(window* w);
	~glRenderer(void);

	void SetSceneBufferReady();
	void MultiSample();
	void PostProcess();

protected:
	void InitializeRenderer();

	bool CreateShaderPrograms();

	void GenerateScreenQuad();
	void CreateFrameBuffer();
	void CreateUniformBuffer();

private:
	window* currentWindow;

	bool bHasInitilized;

	GLuint FBOPostProcess;
	GLuint colorTexPostProcess[2];
	
	GLuint FBOMultiSample;
	GLuint colorTexMultiSample;

	GLuint FBOpingpong[2];
	GLuint colorTexPingpong[2];

	GLuint screenQuadVAO;

	GLuint uboMatrices;

public:
	inline bool HasInitialized() { return bHasInitilized; }
	inline GLuint GetUboMatrices() const { return uboMatrices; }
};

