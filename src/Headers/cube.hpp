#pragma once

#include "glitter.hpp"
#include <vector>

class Cube;

class Face {
public:
	Face(
		Cube* cube,
		GLfloat x1, GLfloat y1, GLfloat z1,
		GLfloat x2, GLfloat y2, GLfloat z2,
		GLfloat x3, GLfloat y3, GLfloat z3,
		GLfloat x4, GLfloat y4, GLfloat z4
	) : parent(cube),
		vertexData {
		x1, y1, z1, x2, y2, z2,
		x3, y3, z3, x4, y4, z4
		} {}

	GLfloat* getVBOData(bool noRecalc); // Do not forget to free
	GLfloat* getVertices(); // No need to free

	void calculateNormal(GLfloat &x, GLfloat &y, GLfloat &z, bool noRecalc);

	Cube* parent;
private:
	GLfloat vertexData[12];
};

class Cube {
public:
	Cube();
	~Cube();

	GLfloat* getVertData(); // No need to free
	void draw();
	glm::vec3 getCenter(bool noRecalc);
private:
	std::vector<Face> faces;
	size_t vertDataSize = 0;
	bool needs_recalc = true;
	GLfloat* vertexData = NULL;
	glm::vec3 center;
	GLuint vbo;
	void resizeVertexBufferObject(GLint newSize);
};