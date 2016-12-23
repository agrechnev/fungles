/* By Oleksiy Grechnyev 2016
Matrix operations like in GLM

This module never allocates any matrices,
They must be local arrays, or pre-allocated

Note: here I use matrix in normal notation,
so they must be either loaded with transpose=GL_TRUE (is it even safe ?)
or transposed
*/

#include <math.h>



#include "esUtil.h"

#include "matrix.h"

#define PI 3.1415926535897932384626433832795f

/* Norm of a vector */
GLfloat mxNorm(GLfloat x, GLfloat y, GLfloat z) {
	return sqrtf(x*x + y*y + z*z);
}

/* Matrix multiplication : result = A*B 
 It is safe, i.e. result can be equal to A or B */
void mxMul(ESMatrix *result, ESMatrix *A, ESMatrix *B) {
	// Make a clean temp matrix
	ESMatrix tmp;
	memset(&tmp, 0, sizeof(ESMatrix));

	int         i, j, k;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			for (k = 0; k < 4; k++)
			{
				tmp.m[i][j] += A->m[i][k] * B->m[k][j];
			}

	// Copy it to where we want it
	memcpy(result, &tmp, sizeof(ESMatrix));
}

/* Identity matrix */
void mxOne(ESMatrix *result) {
	// Set everything to 0
	memset(result, 0, sizeof(ESMatrix));

	// Set diagonals to 1
	result->m[0][0] = 1.0f;
	result->m[1][1] = 1.0f;
	result->m[2][2] = 1.0f;
	result->m[3][3] = 1.0f;
}

/* Create scaling operation:  */
void mxScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz) {
	// Set everything to 0
	memset(result, 0, sizeof(ESMatrix));

	// Set diagonals 
	result->m[0][0] = sx;
	result->m[1][1] = sy;
	result->m[2][2] = sz;
	result->m[3][3] = 1.0f;
}

/* Create translation operation
		 T = { 1 0 0 tx
			   0 1 0 ty
			   0 0 1 tz
			   0 0 0  1}
			  */
void mxTran(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz) {
	mxOne(result);
	result->m[0][3] = tx;
	result->m[1][3] = ty;
	result->m[2][3] = tz;
}

/*
Create rotation operation
rx, ry, rz = rotation axis
angle in radians
*/
void mxRot(ESMatrix *result, GLfloat angle, GLfloat rx, GLfloat ry, GLfloat rz) {
	// Normalize the rotation axis
	GLfloat len = mxNorm(rx, ry, rz);
	rx /= len;
	ry /= len;
	rz /= len;

	// Start with unit matrix
	mxOne(result);

	// Trig functions
	GLfloat c =  cosf(angle);
	GLfloat c1 = 1.0f - cosf(angle);
	GLfloat s = sinf(angle);

	// Set up matrix elements
	result->m[0][0] = c + rx*rx*c1;
	result->m[0][1] = rx*ry*c1 - rz*s;
	result->m[0][2] = rx*rz*c1 + ry*s;

	result->m[1][0] = rx*ry*c1 + rz*s;
	result->m[1][1] = c + ry*ry*c1;
	result->m[1][2] = ry*rz*c1 - rx*s;

	result->m[2][0] = rx*rz*c1 - ry*s;
	result->m[2][1] = ry*rz*c1 + rx*s;
	result->m[2][2] = c + rz*rz*c1;
}

/* Translate deg to radians */
GLfloat mxRad(GLfloat x) {
	return x*PI / 180.0f;
}

/* Apply matrix to a vector */
void mxApply(GLfloat result[4], ESMatrix *matrix, GLfloat vector[4]) {
	int i, j;

	for (i = 0; i < 4; i++) {
		result[i] = 0.0f;

		for (j = 0; j < 4; j++) result[i] += matrix->m[i][j] * vector[j];
	}
}

/* Transpose a matrix into itself or another one */
void mxTranspose(ESMatrix *result, ESMatrix *a) {
	ESMatrix tmp;
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			tmp.m[i][j] = a->m[j][i];

	// Copy it to where we want it
	memcpy(result, &tmp, sizeof(ESMatrix));
}

/* Create the lookAt matrix - later */
void lookAt(ESMatrix *result,
	float posX, float posY, float posZ,
	float lookAtX, float lookAtY, float lookAtZ,
	float upX, float upY, float upZ) {


}


