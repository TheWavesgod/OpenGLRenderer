#pragma once

#include <vector>


class window;
class shader;
class mesh;
class texture;

class glRenderer
{
public:
	glRenderer(window* w);

	void Render();

protected:
	bool CreateShaderPrograms();

private:
	std::vector<shader*> shaders;

	window* currentWindow;

	bool bHasInitilized;

	mesh* triangle;
	mesh* quad;

	texture* tex;

public:
	inline bool HasInitialized() { return bHasInitilized; }

};

