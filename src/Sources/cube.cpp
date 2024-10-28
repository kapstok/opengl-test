#include "cube.hpp"
#include "glitter.hpp"

Cube::Cube() {
	this->vertexData = {
		// Bottom
		-1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		// Top
		-1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		// Front
		-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		// Back
		-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
		 1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
		 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f,

		// Left
		-1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,

		// Right
		1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
	};

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
}

Cube::~Cube() {
	//delete Cube::vertexData;
	glDeleteBuffers(1, &this->vbo);
}

GLfloat* Cube::getVertData(size_t &size) {
	size = sizeof(GLfloat) * this->vertexData.size();
	return this->vertexData.data();
}

void Cube::draw() {
	glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size());
}

void Cube::resizeVertexBufferObject(GLint newSize) {
	GLuint newVbo;
	GLint oldSize;

	glGenBuffers(1, &newVbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_ARRAY_SIZE, &oldSize);

	void* oldData = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

	if (oldData) {
		glBindBuffer(GL_ARRAY_BUFFER, newVbo);
		glBufferData(GL_ARRAY_BUFFER, newSize, oldData, GL_STATIC_DRAW);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glDeleteBuffers(1, &this->vbo);
		this->vbo = newVbo;
	} else {
		glDeleteBuffers(1, &newVbo);
	}
}