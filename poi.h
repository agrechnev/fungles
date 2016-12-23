/* By Oleksiy Grechnyev 2016
* Work with pois (2d objects to draw)
* Each poi object includes (basic) center coordinates, size and (texture or color)
*/

#ifndef POI_H
#define POI_H

#include "esUtil.h"
#include "vao.h"
#include "uniformcache.h"

typedef struct Poi{
	GLfloat x, y, z; // Coordinates of the center

	GLfloat origY; // Original y coordinate, doesn't changed if POI's moved because of eclipse

	GLfloat width, height; // Size of the POI in opengl coordinates
	GLfloat normDist; // Normal distance to camera (give scale 1.0 if pois are scaled)

	VaoObject vaoObject; // VAO object for this poi

	// Texture/Color
	int useTexture; // Use texture or not (boolean)

	GLuint texture; // Texture ID if the texture is used
	GLfloat colorRGBA[4]; // RGBA Color if color is used

	// Occultation (eclipse)- related stuff
	int isEclipsed; // Boolean, true = eclipsed
	GLfloat dist; // Distance to the camera on last draw


	// Data set up on Draw to test for occultation (one POI obscuring another)
	// Corners before transformation
	GLfloat corners[4][4];

    // min and max viewport x,y coordinates after all transformations
	GLfloat minX, maxX, minY, maxY;

} Poi;


/*
* Create a POI with center (x,y,z) and size (width, height)
* from an RGB texture
* Texture is not repeated at present (easy to implement if needed)
* Note: ize is in opengl coords it's better to keep size of the order 0.1-1.0
*/
void createPOItexture(Poi *poi,
	GLfloat x, GLfloat y, GLfloat z,
	GLfloat width, GLfloat height, GLfloat normDist,
	GLubyte *pixels, GLsizei texWidth, GLsizei texHeight); 

/*
* Create a POI with center (x,y,z) and size (width, height)
* with an RGBA color, no texture
* Texture is not repeated at present (easy to implement if needed)
* Note: ize is in opengl coords it's better to keep size of the order 0.1-1.0
*/
void createPOIcolor(Poi *poi,
	GLfloat x, GLfloat y, GLfloat z,
	GLfloat width, GLfloat height, GLfloat normDist,
	GLfloat r, GLfloat g, GLfloat b, GLfloat a);

/*
 * Draw a POI with current GL program, view and projection
 * This routine sets up texture/color and model matrix
 * Camera position is needed for distance (scaling)
 * camera direction (xz components only, not normalized) are needed for rotation
 */
void drawPOI(Poi *poi, UniformCache *uniformCache, ESMatrix *projView,
	GLfloat camX, GLfloat camY, GLfloat camZ,
	GLfloat dirX, GLfloat dirZ);

/*
* Delete POI object
*/
void deletePOI(Poi *poi);

/*
* Check all POIs from a single POI array for occultation(eclipse)
* Set or clear the occultation flag when appropriate
* This should be ran after ALL POIs are drawn !
*/
void occultPOI(Poi poi[], int nPois);

#endif
