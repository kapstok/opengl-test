#pragma once

#include "glitter.hpp"
#include <vector>

class Cube {
public:
	Cube();
	~Cube();

	// No need to free
	GLfloat* getVertData(size_t& size);
	void draw();
private:
	std::vector<GLfloat> vertexData;
	GLuint vbo;
	void resizeVertexBufferObject(GLint newSize);
};