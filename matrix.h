/* By Oleksiy Grechnyev 2016
Matrix operations like in GLM

This module never allocates any matrices,
They must be local arrays, or pre-allocated

Note: here I use matrix in normal notation,
so they must be either loaded with transpose=GL_TRUE (is it even safe ?)
or transposed
*/


#ifndef MATRIX_H
#define MATRIX_H

/* Matrix multiplication : result = A*B */
void mxMul(ESMatrix *result, ESMatrix *A, ESMatrix *B);

/* Identity matrix */
void mxOne(ESMatrix *result);

/* Create scaling operation:  */
void mxScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);

/* Create translation operation
T = { 1 0 0 tx
0 1 0 ty
0 0 1 tz
0 0 0  1}
*/
void mxTran(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

/*
Create rotation operation
rx, ry, rz = rotation axis
angle in radians
*/
void mxRot(ESMatrix *result, GLfloat angle, GLfloat rx, GLfloat ry, GLfloat rz);

/* Translate deg to radians */
GLfloat mxRad(GLfloat x);

/* Apply matrix to a vector */
void mxApply(GLfloat result[4], ESMatrix *matrix, GLfloat vector[4]);

/* Norm of a vector */
GLfloat mxNorm(GLfloat x, GLfloat y, GLfloat z);

/* Transpose a matrix into itself or another one */
void mxTranspose(ESMatrix *result, ESMatrix *a);

#endif