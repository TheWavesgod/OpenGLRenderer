#pragma once

#include <vector>

#include "glad/glad.h"
#include "glfw3.h"

class window;
class Shader;
class Mesh;
class Camera;
class Texture;
class CubeMap;


class glRenderer
{
public:
	glRenderer(window* w);
	~glRenderer(void);

	void Render();

protected:
	void InitializeRenderer();
	void GenerateScreenQuad();

	bool CreateShaderPrograms();

	void CreateFrameBuffer();

	void DrawScene();

private:
	std::vector<Shader*> shaders;

	window* currentWindow;

	bool bHasInitilized;
	
	Mesh* cube;

	Texture* tex;

	CubeMap* skybox;

	GLuint framebuffer;
	GLuint textureColorbuffer;
	GLuint screenQuadVAO;

public:
	inline bool HasInitialized() { return bHasInitilized; }
	Camera* camera;
};

