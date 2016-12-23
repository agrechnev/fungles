/* By Oleksiy Grechnyev 2016
Create VAO, this includes creating vertices, VBO, EBO */

#include "esUtil.h"

#include "fatalerr.h"
#include "vao.h"

/* Create VAO, this includes creating vertices, VBO, EBO
 * pVAO, pVBO, pEBO = pointers to VAO, VBO, EBO
 * vertices[], indices[]  = data arrays
 * indices == 0 : don't use EBO
 * ver_size = sizeof(vertices)
 * ind_size = sizeof(indices)
 * ver_type = type of vertex data, e.g. VAO_XYZRBST
 */
void createVAO(VaoObject *pVO, GLfloat* vertices, size_t ver_size, GLuint* indices, size_t ind_size, int ver_type) {


	// VBO, VAO

	glGenVertexArrays(1, &(pVO->VAO));
	glGenBuffers(1, &(pVO->VBO));

	// Bind VAO
	glBindVertexArray(pVO->VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, pVO->VBO);
	glBufferData(GL_ARRAY_BUFFER, ver_size, vertices, GL_STATIC_DRAW);

	// Create and bind EBO, ignore if pEBO==null
	if (indices != NULL) {
		pVO->useEbo = TRUE;
		glGenBuffers(1, &(pVO->EBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pVO->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_size, indices, GL_STATIC_DRAW);
	}
	else {
		pVO->useEbo = FALSE;
	}

	int stride; // The stride of data arrays

	// Set up the arrays
	switch (ver_type) {
	case VAO_XYZ:  // xyz
		stride = 3;
	   // xyz array (0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		break;

	case VAO_XYZRGB:  // xyzrgb
		stride = 6;
	   // xyz array (0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		// rgb array (1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		break;

	case VAO_XYZST:  // xyzst
		stride = 5;
		// xyz array (0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		// st array (2) (texture coords)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		break;

	case VAO_XYZRGBST:  // xyzrgbst
		stride = 8;
	   // xyz array (0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		// rgb array (1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// st array (2) (texture coords)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		break;

	default:
		fatalError("Error: createVAO: wrong ver_type !");
	}

	// Set up number of elements to draw
	if (pVO->useEbo) {
		pVO->nDrawElements = ind_size / sizeof(GLuint); // Number of vertices to draw
	} else {
		pVO->nDrawElements = ver_size / (stride*sizeof(GLfloat)); // Number of vertices
	}

	// Unbind VBO+VAO, but not EBO !!!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/* Delete VAO, VBO and EBO
 *  EBO is skipped if !useEbo 
 */
void deleteVAO(VaoObject *pVO) {
	glDeleteVertexArrays(1, &(pVO->VAO));
	glDeleteBuffers(1, &(pVO->VBO));
	if (pVO->useEbo) glDeleteBuffers(1, &(pVO->EBO));
}

/* 
 * Draw a Vao object
 */
void drawVAO(VaoObject *pVO) {

	// Bind it first
	glBindVertexArray(pVO->VAO);

	if (pVO->useEbo) {
		// If using EBO, draw like this
		glDrawElements(GL_TRIANGLES, pVO->nDrawElements, GL_UNSIGNED_INT, 0);
	}
	else {
		// If not using EBO, draw like this
		glDrawArrays(GL_TRIANGLES, 0, pVO->nDrawElements);
	}
}
