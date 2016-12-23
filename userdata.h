/* By Oleksiy Grechnyev 2016
 * Definition of the structure UserData (header only)
 */

#ifndef USERDATA_H
#define USERDATA_H
#include "esUtil.h"

#include "uniformcache.h"

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
	Poi pois[10]; // The POIs

    // Blender-like Axes (painted by me) data XYZ=RGB
	// I could have used single vao+rotations, but it's more clear this way
	VaoObject axisVaoX, axisVaoY, axisVaoZ;

	//-------------
	// Camera settings
	// Camera origin
	GLfloat cameraX, cameraY, cameraZ;

} UserData;

#endif
