/* By Oleksiy Grechnyev 2016
 * Definition of the structure UserData (header only)
 */

#ifndef USERDATA_H
#define USERDATA_H
#include "esUtil.h"

#include "uniformcache.h"

// Maximal number of POIs
// Change to malloc if you wish
#define USERDATA_MAX_POIS  20 

typedef struct UserData
{

	GLuint program; // The EGL Program (includes shaders)

	UniformCache uniformCache; // Uniform location cache for the program

					// Model data
					// Will delete this later
	VaoObject vao1; // Vertices
	GLuint tex;  // Texture


				 // POIs, they include VAOs, textures as needed
	int numPois; // Number of pois
	
	Poi pois[USERDATA_MAX_POIS]; // The POIs

    // Blender-like Axes (painted by me) data XYZ=RGB
	// I could have used single vao+rotations, but it's more clear this way
	VaoObject axisVaoX, axisVaoY, axisVaoZ;

	//-------------
	// Camera settings
	// Camera position
	GLfloat cameraX, cameraY, cameraZ;
	// Camera: lookAt point
	GLfloat lookX, lookY, lookZ;

} UserData;

#endif
