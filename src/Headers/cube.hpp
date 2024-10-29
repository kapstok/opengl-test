#pragma once

#include "glitter.hpp"
#include <vector>

class Face {
public:
	Face(
		GLfloat x1, GLfloat y1, GLfloat z1,
		GLfloat x2, GLfloat y2, GLfloat z2,
		GLfloat x3, GLfloat y3, GLfloat z3,
		GLfloat x4, GLfloat y4, GLfloat z4,
		GLfloat nx, GLfloat ny, GLfloat nz
	) : vertexData {
		x1, y1, z1, x2, y2, z2,
		x3, y3, z3, x4, y4, z4,
		nx, ny, nz
	} {}

	// Do not forget to free
	GLfloat* getVertData();

	void calculateNormal(GLfloat &x, GLfloat &y, GLfloat &z);
private:
	GLfloat vertexData[15];
};

class Cube {
public:
	Cube();
	~Cube();

	// Do not forget to free
	GLfloat* getVertData(size_t& size);
	void draw();
private:
	std::vector<Face> vertexData;
	size_t vertDataSize = 0;
	GLuint vbo;
	void resizeVertexBufferObject(GLint newSize);
};