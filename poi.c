
/* By Oleksiy Grechnyev 2016
 * Work with pois (2d objects to draw)
 * Each poi object includes (basic) center coordinates, size and (texture or color)
 */

#include "esUtil.h"

#include "rgbtex.h"
#include "vao.h"
#include "matrix.h"
#include "uniformcache.h"

#include "poi.h"



/* 
 * Create a POI with center (x,y,z) and size (width, height)
 * from an RGB texture
 * Texture is not repeated at present (easy to implement if needed)
 * Note: ize is in opengl coords it's better to keep size of the order 0.1-1.0
 */
void createPOItexture(Poi *poi, 
	                 GLfloat x, GLfloat y, GLfloat z,
	                 GLfloat width, GLfloat height, GLfloat normDist,
	                 GLubyte *pixels, GLsizei texWidth, GLsizei texHeight) {
	poi->x = x;
	poi->y = y;
	poi->z = z;
	poi->width = width;
	poi->height = height;
	poi->normDist = normDist;
	poi->useTexture = TRUE;

	// Let's create VAO
	GLfloat vertices[6][5] = {
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f
	};

	// Scale with width, height
	for (int i = 0; i < 6; i++) {
		vertices[i][0] *= width;
		vertices[i][1] *= height;
	}


	// Set up VAO from vertices without EBO 
	createVAO(&(poi->vaoObject), (GLfloat *)vertices, sizeof(vertices), NULL, 0, VAO_XYZST);

	// Set up texture
	poi->texture = createRGBTexture(pixels, texWidth, texHeight);
}

/*
* Create a POI with center (x,y,z) and size (width, height)
* with an RGBA color, no texture
* Texture is not repeated at present (easy to implement if needed)
* Note: ize is in opengl coords it's better to keep size of the order 0.1-1.0
*/
void createPOIcolor(Poi *poi,
	GLfloat x, GLfloat y, GLfloat z,
	GLfloat width, GLfloat height, GLfloat normDist,
	GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	poi->x = x;
	poi->y = y;
	poi->z = z;
	poi->width = width;
	poi->height = height;
	poi->normDist = normDist;
	poi->useTexture = FALSE;

	// Let's create VAO
	GLfloat vertices[6][3] = {
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	// Scale with width, height
	for (int i = 0; i < 6; i++) {
		vertices[i][0] *= width;
		vertices[i][1] *= height;
	}


	// Set up VAO from vertices without EBO 
	createVAO(&(poi->vaoObject), (GLfloat *)vertices, sizeof(vertices), NULL, 0, VAO_XYZ);

	// Set up the color
	poi->colorRGBA[0] = r;
	poi->colorRGBA[1] = g;
	poi->colorRGBA[2] = b;
	poi->colorRGBA[3] = a;
}

/*
 * Draw a POI with current GL program, view and projection
 * This routine sets up texture/color and model matrix
 * program is needed for uniform locations
 */
void drawPOI(Poi *poi, UniformCache *uniformCache) {

	// Set up the model matrix (rotate+scale+translate)
	ESMatrix model;
	

	// Rotate

	// Translate
	mxTran(&model, poi->x, poi->y, poi->z);

	// Set up the final model
	glUniformMatrix4fv(uniformCache->model, 1, GL_TRUE, (const GLfloat*)&model.m);

	// Set up texture or color
	if (poi->useTexture) {
		glUniform1i(uniformCache->useTexture, GL_TRUE); // Use textures
		glBindTexture(GL_TEXTURE_2D, poi->texture); // Bind the texture
	}
	else {
		glUniform1i(uniformCache->useTexture, GL_FALSE); // Use color
		glUniform4fv(uniformCache->myColor, 1, poi->colorRGBA); // Set color
	}

	// Draw the VAO object
	drawVAO(&(poi->vaoObject));
	
}

/*
 * Delete POI object
 */
void deletePOI(Poi *poi) {
	deleteVAO(&(poi->vaoObject));
	glDeleteTextures(1, &(poi->texture));
}