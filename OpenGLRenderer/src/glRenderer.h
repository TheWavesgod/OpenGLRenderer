#pragma once

#include <vector>


class window;
class Shader;
class Mesh;
class Camera;
class Texture;


class glRenderer
{
public:
	glRenderer(window* w);
	~glRenderer(void);

	void Render();

protected:
	bool CreateShaderPrograms();

private:
	std::vector<Shader*> shaders;

	window* currentWindow;

	bool bHasInitilized;
	
	Mesh* cube;

	Texture* tex;

	Camera* camera;

public:
	inline bool HasInitialized() { return bHasInitilized; }

};

