
/* By Oleksiy Grechnyev 2016
 * Work with pois (2d objects to draw)
 * Each poi object includes (basic) center coordinates, size and (texture or color)
 */

#include <math.h>

#include "esUtil.h"

#include "rgbtex.h"
#include "vao.h"
#include "matrix.h"
#include "uniformcache.h"

#include "poi.h"


/*
 * Set corners in vertex coordinates before all transformation
 */
void setCornersPOI(Poi* poi) {
	// --
	poi->corners[0][0] = -poi->width / 2;
	poi->corners[0][1] = -poi->height / 2;

	// -+
	poi->corners[1][0] = -poi->width / 2;
	poi->corners[1][1] = +poi->height / 2;


	// +-
	poi->corners[2][0] = +poi->width / 2;
	poi->corners[2][1] = -poi->height / 2;


	// ++
	poi->corners[3][0] = +poi->width / 2;
	poi->corners[3][1] = +poi->height / 2;

	for (int i = 0; i < 4; i++) {
		poi->corners[i][2] = 0.0f; //  Z
		poi->corners[i][3] = 1.0f; //  W
	}
}

/* Minimal initialization ops*/
void initPOI(Poi *poi,
	GLfloat x, GLfloat y, GLfloat z,
	GLfloat width, GLfloat height, GLfloat normDist) {

	// Copy args
	poi->x = x;
	poi->y = y;
	poi->z = z;
	poi->width = width;
	poi->height = height;
	poi->normDist = normDist;

	// More data
	poi->isEclipsed = FALSE; // To start with
	poi->origY = y;

	// Set corners
	setCornersPOI(poi);
};

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
	
	initPOI(poi, x, y, z, width, height, normDist);

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

	initPOI(poi, x, y, z, width, height, normDist);

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
 * Set min and max viewport X, Y coordinates after all transformations
 * Needed for occultation test
 * tran = proj*view*model
 */
void setMinMaxXY(Poi *poi, ESMatrix *tran) {
	// One corner in viewport coords
	GLfloat viewportCorner[4];

	int i; // Loop index over corners


	GLfloat x, y;
	GLfloat x1, x2, y1, y2; // min max values

	
	for (i = 0; i < 4; i++) {
		// Apply the complete transformation matrix to this corner
		mxApply(viewportCorner, tran, poi->corners[i]); 

		// Don't forget the perspective division !!!
		// I spent a few very unhappy hours because I forgot that !!!
		x = viewportCorner[0] / viewportCorner[3];
		y = viewportCorner[1] / viewportCorner[3];

		if (i == 0 || x < x1) x1 = x; // min X
		if (i == 0 || x > x2) x2 = x; // max X
		if (i == 0 || y < y1) y1 = y; // min Y
		if (i == 0 || y > y2) y2 = y; // max Y
	}
	
	poi->minX = x1;
	poi->maxX = x2;
	poi->minY = y1;
	poi->maxY = y2;
}


/*
 * Draw a POI with current GL program, view and projection
 * This routine sets up texture/color and model matrix
 * Camera position is needed for distance (scaling)
 * camera direction (xz components only, not normalized) are needed for rotation
 */
void drawPOI(Poi *poi, UniformCache *uniformCache, ESMatrix *projView,
	GLfloat camX, GLfloat camY, GLfloat camZ,
	GLfloat dirX, GLfloat dirZ) {

	// Set up the model matrix (rotate+scale+translate)
	ESMatrix model, tmpMat;
	
	// Distance to the camera, also saved in the poi structure!
	poi->dist = mxNorm(camX - poi->x, camY - poi->y, camZ - poi->z);

	// Scale = dist/normDist
	GLfloat scale = poi->dist / poi->normDist;
	mxScale(&model, scale, scale, scale);

	// Rotate
	GLfloat angle = atan2f(dirX, dirZ); // Angle in radians
	mxRot(&tmpMat, angle, 0.0f, 1.0f, 0.0f);
	mxMul(&model, &tmpMat, &model);

	// Translate
	mxTran(&tmpMat, poi->x, poi->y, poi->z);
	mxMul(&model, &tmpMat, &model);

	// Set up the final model
	glUniformMatrix4fv(uniformCache->model, 1, GL_TRUE, (const GLfloat*)&model.m);


	// Set up min and max viewport X, Y
	ESMatrix tran;
	mxMul(&tran, projView, &model); // The total transformation matrix like in the vertex shader
	setMinMaxXY(poi, &tran);

	// Set up texture or color

	// Black eclipsed POIs ?
	// TRUE for test, FALSE for production
	const int blackEclipsed = TRUE; 

	if (blackEclipsed && poi->isEclipsed) {
		// Black eclipsed POI, overrides texture
		glUniform1i(uniformCache->useTexture, GL_FALSE); // Use color
		glUniform4f(uniformCache->myColor, 0.0f, 0.0f, 0.0f, 1.0f); // Set color black
	}
	else if (poi->useTexture) {
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

/* 
 * Check all POIs from a single POI array for occultation(eclipse)
 * Set or clear the occultation flag when appropriate
 * This should be ran after ALL POIs are drawn !
 */
void occultPOI(Poi poi[], int nPois){
	int i1, i2; // Loop indices

	// Loop over all POIs -- check for eclipse
	for (i1 = 0; i1 < nPois; i1++) {
		poi[i1].isEclipsed = FALSE; // To start with

		// Check if the POI is completely invisble -- skip the check then and continue the outer loop
		// if (poi[i1].maxX< -1.0f || poi[i1].minX > 1.0f || poi[i1].maxY< -1.0f || poi[i1].minY > 1.0f) continue;

		// Go here if POI is visible
		// Inner loop over all POIs i2 which might eclipse our POI i1
		for (i2 = 0; i2 < nPois; i2++) {
			if (i2 == i1) continue; // Skip

			if (poi[i2].dist > poi[i1].dist) continue; // Farther away from camera: not interesting

			// Check if both X and Y intervals overlap
			if (poi[i1].minX < poi[i2].maxX && poi[i2].minX < poi[i1].maxX && 
				poi[i1].minY < poi[i2].maxY && poi[i2].minY < poi[i1].maxY )
			{
				poi[i1].isEclipsed = TRUE; // i1 is eclipsed by i2
				break; // break the inner loop
			}

		}
	}
}