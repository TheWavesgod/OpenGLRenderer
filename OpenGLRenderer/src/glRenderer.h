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

	double launch_time;

	/** Setting parameter */
	bool bGammaCorrection = true;
	float exposure = 1.0f;
	int blendEquation;
	bool bEnableBloom = true;
	bool bEnableVSync = false;
protected:
	void InitializeRenderer();

	bool CreateShaderPrograms();

	void GenerateScreenQuad();
	void CreateFrameBuffer();
	void CreateUniformBuffer();

	void SetBlendQquation(int i);

private:
	window* currentWindow;

	bool bHasInitilized;

	GLuint FBOPostProcess;
	GLuint colorTexPostProcess;
	
	GLuint FBOMultiSample;
	GLuint colorTexMultiSample;

	GLuint FBOBloom;
	GLuint colorTexBloom;

	GLuint FBOpingpong[2];
	GLuint colorTexPingpong[2];

	GLuint screenQuadVAO;

	GLuint uboCamera;

public:
	inline bool HasInitialized() { return bHasInitilized; }
	inline GLuint GetUboCamera() const { return uboCamera; }
};

