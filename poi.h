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

	GLfloat width, height; // Size of the poin in opengl coordinates
	GLfloat normDist; // Normal distance to camera (give scale 1.0 if pois are scaled)

	VaoObject vaoObject; // VAO object for this poi

	int useTexture; // Use texture or not (boolean)
	

	GLuint texture; // Texture ID if the texture is used
	GLfloat colorRGBA[4]; // RGBA Color if color is used

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
* program is needed for uniform locations
*/
void drawPOI(Poi *poi, UniformCache *uniformCache);

/*
* Delete POI object
*/
void deletePOI(Poi *poi);

#endif
