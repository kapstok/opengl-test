#include "cube.hpp"
#include "glitter.hpp"

GLfloat* Face::getVBOData(bool noRecalc) {
	GLfloat* result = new GLfloat[36];

	// Punt A
	result[0] = this->vertexData[0];
	result[1] = this->vertexData[1];
	result[2] = this->vertexData[2];
	this->calculateNormal(result[3], result[4], result[5], noRecalc);

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

	// Punt D
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

GLfloat* Face::getVertices() {
	return this->vertexData;
}

void Face::calculateNormal(GLfloat &x, GLfloat &y, GLfloat &z, bool noRecalc) {
    glm::vec3 pA(this->vertexData[0], this->vertexData[1], this->vertexData[2]);
    glm::vec3 pB(this->vertexData[3], this->vertexData[4], this->vertexData[5]);
    glm::vec3 pC(this->vertexData[9], this->vertexData[10], this->vertexData[11]);
    glm::vec3 edgeBA = pB - pA;
    glm::vec3 edgeCA = pC - pA;

    glm::vec3 normal = glm::cross(edgeBA, edgeCA);
    normal = glm::normalize(normal);

    glm::vec3 faceCenter = (pA + pB + pC) / 3.0f;
    glm::vec3 toCenter = faceCenter - parent->getCenter(noRecalc);

    // If the normal points toward the reference point, flip it
    if (glm::dot(normal, toCenter) < 0) {
        normal *= -1;
    }

    x = normal.x;
    y = normal.y;
    z = normal.z;
}

Cube::Cube() {
	Face bottom = {
		this,
		-1, -1, -1,
		 1, -1, -1,
		 1, -1,  1,
		-1, -1,  1
	};
	Face top = {
		this,
		-1,  1, -1,
		 1,  1, -1,
		 1,  1,  1,
		-1,  1,  1
	};
	Face front = {
		this,
		 1, -1, -1,
		 1,  1, -1,
		 1,  1,  1,
		 1, -1,  1
	};
	Face back = {
		this,
		-1, -1, -1,
		-1,  1, -1,
		-1,  1,  1,
		-1, -1,  1
	};
	Face left = {
		this,
		 1, -1, 1,
		 1,  1, 1,
		-1,  1, 1,
		-1, -1, 1
	};
	Face right = {
		this,
		 1, -1, -1,
		 1,  1, -1,
		-1,  1, -1,
		-1, -1, -1
	};

	this->faces.push_back(bottom);
	this->faces.push_back(top);
	this->faces.push_back(front);
	this->faces.push_back(back);
	this->faces.push_back(left);
	this->faces.push_back(right);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * this->faces.size(), this->getVertData(), GL_STATIC_DRAW);
}

Cube::~Cube() {
	glDeleteBuffers(1, &this->vbo);
	delete[] this->vertexData;
}

GLfloat* Cube::getVertData() {
	if (!this->needs_recalc) {
		return this->vertexData;
	}

	if (this->vertexData != NULL) {
		delete[] this->vertexData;
	}

	this->vertDataSize = this->faces.size() * 36;
	GLfloat* data = new GLfloat[this->vertDataSize];

	/*
		Recalculate the center.
		Note: This has to be calculated before we loop
		through the faceData, as the normals of the
		faceData is calculated by this->center.
	*/
	this->center = glm::vec3(0, 0, 0);
	for (size_t i = 0; i < this->faces.size(); i++) {
		GLfloat* vertices = this->faces[i].getVertices();
		for (size_t j = 0; j < 12; j++) {
			center[j % 3] = vertices[j];
		}
	}
	center /= this->faces.size() * 12;

	for (size_t i = 0; i < this->faces.size(); i++) {
		GLfloat* faceData = this->faces[i].getVBOData(true);
		memcpy(data + i * 36, faceData, sizeof(GLfloat) * 36);
		delete[] faceData;
	}

	this->needs_recalc = false;
	this->vertexData = data;
	return data;
}

void Cube::draw() {
	glDrawArrays(GL_TRIANGLES, 0, this->vertDataSize);
}

glm::vec3 Cube::getCenter(bool noRecalc) {
	if (this->needs_recalc && !noRecalc) {
		this->getVertData();
	}

	return this->center;
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
		this->needs_recalc = true;
	} else {
		glDeleteBuffers(1, &newVbo);
	}
}