#pragma once

#include "glfw3.h"
#include "glad/glad.h"
#include "glm.hpp"

#include <vector>

class mesh
{
public:
	static mesh* GenerateTriangle();

protected:
	void BufferData();

private:
	std::vector<glm::vec3> vertices;

};

