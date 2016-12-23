/* I couldn't find esTransform.h, so I made it myself*/
#ifndef ESTRANSFORM_H
#define ESTRANSFORM_H

void ESUTIL_API
esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);


void ESUTIL_API
esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

void ESUTIL_API
esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void ESUTIL_API
esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

void ESUTIL_API
esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

void ESUTIL_API
esOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

void ESUTIL_API
esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

void ESUTIL_API
esMatrixLoadIdentity(ESMatrix *result);

void ESUTIL_API
esMatrixLookAt(ESMatrix *result,
	float posX, float posY, float posZ,
	float lookAtX, float lookAtY, float lookAtZ,
	float upX, float upY, float upZ);



#endif