#include "cube.hpp"
#include "glitter.hpp"
#include "arc.hpp"

#include <vector>

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
		-0.8f, -1, -0.8f,
		 0.8f, -1, -0.8f,
		 0.8f, -1,  0.8f,
		-0.8f, -1,  0.8f
	};
	Face top = {
		this,
		-0.8f,  1, -0.8f,
		 0.8f,  1, -0.8f,
		 0.8f,  1,  0.8f,
		-0.8f,  1,  0.8f
	};
	std::vector<Face> fronttoparc = faceToArc(
		{ 1,  0.8f,  0.8f},
		{ 1,  0.8f, -0.8f},
		{ 0.8f,  1, -0.8f},
		{ 0.8f,  1,  0.8f},
		3, 1
	);
	Face fronttop = {
		this,
		 1,  0.8f,  0.8f,	// Front C
		 1,  0.8f,  -0.8f,	// Front B
		 0.8f,  1, -0.8f,	// Top B
		 0.8f,  1,  0.8f	// Top C
	};
	Face front = {
		this,
		 1, -0.8f, -0.8f,
		 1,  0.8f, -0.8f,
		 1,  0.8f,  0.8f,
		 1, -0.8f,  0.8f
	};
	Face frontbottom = {
		this,
		 1, -0.8f, -0.8f, // Front A
		 1, -0.8f,  0.8f, // Front D
		 0.8f, -1,  0.8f, // Bottom C
		 0.8f, -1, -0.8f  // Bottom B
	};
	Face frontleft = {
		this,
		 1,  0.8f,  0.8f, // Front C
		 1, -0.8f,  0.8f, // Front D
		 0.8f, -0.8f,  1, // Left A
		 0.8f,  0.8f,  1  // Left B
	};
	Face lefttop = {
		this,
		-0.8f,  0.8f,  1, // Left C
		 0.8f,  0.8f,  1, // Left B
		 0.8f,  1,  0.8f, // Top C
		-0.8f,  1,  0.8f  // Top D
	};
	Face left = {
		this,
		 0.8f, -0.8f, 1,
		 0.8f,  0.8f, 1,
		-0.8f,  0.8f, 1,
		-0.8f, -0.8f, 1
	};
	Face leftbottom = {
		this,
		 0.8f, -0.8f,  1, // Left A
		-0.8f, -0.8f,  1, // Left D
		-0.8f, -1,  0.8f, // Bottom D
		 0.8f, -1,  0.8f  // Bottom C
	};
	Face leftback = {
		this,
		-0.8f,  0.8f,  1, // Left C
		-0.8f, -0.8f,  1, // Left D
		-1, -0.8f,  0.8f, // Back D
		-1,  0.8f,  0.8f  // Back C
	};
	Face backtop = {
		this,
		-1,  0.8f,  0.8f, // Back C
		-1,  0.8f, -0.8f, // Back B
		-0.8f,  1, -0.8f, // Top A
		-0.8f,  1,  0.8f  // Top D
	};
	Face back = {
		this,
		-1, -0.8f, -0.8f,
		-1,  0.8f, -0.8f,
		-1,  0.8f,  0.8f,
		-1, -0.8f,  0.8f
	};
	Face backbottom = {
		this,
		-1, -0.8f, -0.8f, // Back A
		-1, -0.8f,  0.8f, // Back D
		-0.8f, -1,  0.8f, // Bottom D
		-0.8f, -1, -0.8f  // Bottom A
	};
	Face backright = {
		this,
		-1, -0.8f, -0.8f, // Back A
		-1,  0.8f, -0.8f, // Back B
		-0.8f,  0.8f, -1, // Right C
		-0.8f, -0.8f, -1  // Right D
	};
	Face righttop = {
		this,
		-0.8f,  0.8f, -1, // Right C
		 0.8f,  0.8f, -1, // Right B
		 0.8f,  1, -0.8f, // Top B
		-0.8f,  1, -0.8f  // Top A
	};
	Face right = {
		this,
		 0.8f, -0.8f, -1,
		 0.8f,  0.8f, -1,
		-0.8f,  0.8f, -1,
		-0.8f, -0.8f, -1
	};
	Face rightbottom = {
		this,
		 0.8f, -0.8f, -1, // Right A
		-0.8f, -0.8f, -1, // Right D
		-0.8f, -1, -0.8f, // Bottom A
		 0.8f, -1, -0.8f  // Bottom B
	};
	Face rightfront = {
		this,
		 0.8f,  0.8f, -1, // Right B
		 0.8f, -0.8f, -1, // Right A
		 1, -0.8f, -0.8f, // Front A
		 1,  0.8f, -0.8f  // Front B
	};

	for (size_t i = 0; i < fronttoparc.size(); i++) {
		fronttoparc[i].parent = this;
	}

	this->faces.push_back(bottom);
	this->faces.push_back(top);
	//this->faces.push_back(fronttop);
	this->faces.push_back(fronttoparc[0]);
	this->faces.push_back(front);
	this->faces.push_back(frontbottom);
	this->faces.push_back(frontleft);
	this->faces.push_back(lefttop);
	this->faces.push_back(left);
	this->faces.push_back(leftbottom);
	this->faces.push_back(leftback);
	this->faces.push_back(backtop);
	this->faces.push_back(back);
	this->faces.push_back(backbottom);
	this->faces.push_back(backright);
	this->faces.push_back(righttop);
	this->faces.push_back(right);
	this->faces.push_back(rightbottom);
	this->faces.push_back(rightfront);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * this->faces.size(), this->getVertData(), GL_STREAM_DRAW);
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