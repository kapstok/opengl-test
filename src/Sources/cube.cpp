#include "cube.hpp"
#include "glitter.hpp"

GLfloat* Face::getVertData() {
	GLfloat* result = new GLfloat[36];

	// Punt A
	result[0] = this->vertexData[0];
	result[1] = this->vertexData[1];
	result[2] = this->vertexData[2];
	result[3] = this->vertexData[12];
	result[4] = this->vertexData[13];
	result[5] = this->vertexData[14];

	// Punt B
	result[6] = this->vertexData[3];
	result[7] = this->vertexData[4];
	result[8] = this->vertexData[5];
	result[9] = result[3];
	result[10] = result[4];
	result[11] = result[5];

	// Punt C
	result[12] = this->vertexData[9];
	result[13] = this->vertexData[10];
	result[14] = this->vertexData[11];
	result[15] = result[3];
	result[16] = result[4];
	result[17] = result[5];

	// Punt E
	result[18] = this->vertexData[6];
	result[19] = this->vertexData[7];
	result[20] = this->vertexData[8];
	result[21] = result[3];
	result[22] = result[4];
	result[23] = result[5];

	// Punt C
	result[24] = this->vertexData[9];
	result[25] = this->vertexData[10];
	result[26] = this->vertexData[11];
	result[27] = result[3];
	result[28] = result[4];
	result[29] = result[5];

	// Punt B
	result[30] = this->vertexData[3];
	result[31] = this->vertexData[4];
	result[32] = this->vertexData[5];
	result[33] = result[3];
	result[34] = result[4];
	result[35] = result[5];

	return result;
}

void Face::calculateNormal(GLfloat &x, GLfloat &y, GLfloat &z) {
	glm::vec3 alpha(
		this->vertexData[3] - this->vertexData[0],
		this->vertexData[4] - this->vertexData[1],
		this->vertexData[5] - this->vertexData[2]
	);
	glm::vec3 beta(
		this->vertexData[6] - this->vertexData[0],
		this->vertexData[7] - this->vertexData[1],
		this->vertexData[8] - this->vertexData[2]
	);

	glm::vec3 crossProd = glm::cross(alpha, beta);
	glm::vec3 result = glm::normalize(crossProd);
	x = result.x;
	y = result.y;
	z = result.z;
}

Cube::Cube() {
	Face bottom = {
		-1, -1, -1,
		 1, -1, -1,
		 1, -1,  1,
		-1, -1,  1,
		 0, -1,  0
	};
	Face top = {
		-1,  1, -1,
		 1,  1, -1,
		 1,  1,  1,
		-1,  1,  1,
		 0,  1,  0
	};
	Face front = {
		 1, -1, -1,
		 1,  1, -1,
		 1,  1,  1,
		 1, -1,  1,
		 1,  0,  0
	};
	Face back = {
		-1, -1, -1,
		-1,  1, -1,
		-1,  1,  1,
		-1, -1,  1,
		-1,  0,  0
	};
	Face left = {
		 1, -1, 1,
		 1,  1, 1,
		-1,  1, 1,
		-1, -1, 1,
		 0,  0, 1
	};
	Face right = {
		 1, -1, -1,
		 1,  1, -1,
		-1,  1, -1,
		-1, -1, -1,
		 0,  0, -1
	};

	this->vertexData.push_back(bottom);
	this->vertexData.push_back(top);
	this->vertexData.push_back(front);
	this->vertexData.push_back(back);
	this->vertexData.push_back(left);
	this->vertexData.push_back(right);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * this->vertexData.size(), this->getVertData(this->vertDataSize), GL_STATIC_DRAW);
}

Cube::~Cube() {
	glDeleteBuffers(1, &this->vbo);
}

GLfloat* Cube::getVertData(size_t &size) {
	size = this->vertexData.size() * 36;
	GLfloat* result = new GLfloat[size];

	for (size_t i = 0; i < this->vertexData.size(); i++) {
		GLfloat* faceData = this->vertexData[i].getVertData();
		memcpy(result + i * 36, faceData, sizeof(GLfloat) * 36);
		delete[] faceData;
	}
	return result;
}

void Cube::draw() {
	glDrawArrays(GL_TRIANGLES, 0, this->vertDataSize);
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