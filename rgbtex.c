/* By Oleksiy Grechnyev 2016
Create a simple RGB texture from an array
I am too lazy to load files now */



#include "esUtil.h"

// Note: for sharp texture image
// replace GL_LINEAR with GL_NEAREST
#define MY_INTERPOLATION  GL_LINEAR

GLuint createRGBTexture(GLubyte *pixels, GLsizei width, GLsizei height) {
	GLuint textureId;

	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Generate a texture object
	glGenTextures(1, &textureId);
	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, textureId);


	// Load the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);


	// Set wrapping parameters (REPEAT !)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MY_INTERPOLATION);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MY_INTERPOLATION);

	// Unbind the texture object for now
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureId;
};