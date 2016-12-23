/* By Oleksiy Grechnyev 2016
Fun with OpenGL ES */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "esUtil.h"
#include "fatalerr.h"
#include "shaders.h"
#include "vao.h"
#include "rgbtex.h"
#include "matrix.h"
#include "poi.h"

#include "userdata.h"

// Draw axes or not ?
#define FUNGLES_DRAW_AXES TRUE


/////////////////////////////////////////////////////
// INIT SECTION
///////////////////////////////////

//-------------------------------------------
/*
Initialize graphical model (sans axes)
*/
void initModel(UserData *userData) {

	// Create EGL program + shaders
	userData->program = createShaderProgram("shaders/s.vs", "shaders/s.frag");

	// Create uniform location cache
	createUniformCache(&(userData->uniformCache), userData->program);

	// Set up POIs

	// Set up a trivial texture
	// 2 x 2 Image, 3 bytes per pixel (R, G, B)
	GLubyte pixels[4 * 3] =
	{
		255, 0, 0, // Red
		0, 255, 0, // Green
		0, 0, 255, // Blue
		255, 255, 0 // Yellow
	};

	userData->numPois = 3;
	createPOItexture(&(userData->pois[0]),
		            0.5f, 0.0f, -0.1f, // X Y Z
		            1.0f, 1.0f, 10.0f, // width height normDist
		            pixels, 2, 2);   // texture

	createPOIcolor(&(userData->pois[1]),
		-0.5f, 0.5f, 2.0f, // X Y Z
		1.0f, 1.0f, 10.0f, // width height normDist
		1.0f, 0.5f, 0.0f, 1.0f);   // RGBA orange


	createPOIcolor(&(userData->pois[2]),
		1.0f, 1.0f, -4.0f, // X Y Z
		1.0f, 1.0f, 10.0f, // width height normDist
		1.0f, 0.0f, 1.0f, 1.0f);   // RGBA purple

	
}

//-------------------------------------------
///
/* Initialize painted axes
 Note: X=red, Y=green, Z=blue like in Blender
 */
void initAxes(UserData *userData) {

	// X
	GLfloat verticesX[] = {
		// Triangle 1
	    3.0f,  0.0f,  0.0f,
	    0.0f,  0.05f,  0.0f,
	    0.0f, -0.05f,  0.0f,
		// Triangle 2
	    3.0f,  0.0f,  0.0f,
	    0.0f,  0.0f,  0.05f,
        0.0f,  0.0f, -0.05f
	};

	createVAO(&(userData->axisVaoX), verticesX, sizeof(verticesX), NULL, 0, VAO_XYZ);

	// Y
	GLfloat verticesY[] = {
		// Triangle 1
		0.0f,  3.0f,  0.0f,
		0.05f,  0.0f,  0.0f,
	   -0.05f,  0.0f,  0.0f,
		// Triangle 2
		0.0f,  3.0f,  0.0f,
		0.0f,  0.0f,  0.05f,
		0.0f,  0.0f, -0.05f
	};

	createVAO(&(userData->axisVaoY), verticesY, sizeof(verticesY), NULL, 0, VAO_XYZ);

	// Z
	GLfloat verticesZ[] = {
		// Triangle 1
		0.0f,  0.0f,  3.0f,
		0.05f,  0.0f,  0.0f,
       -0.05f,  0.0f,  0.0f,
		// Triangle 2
		0.0f,  0.0f,  3.0f,
		0.0f,  0.05f,  0.0f,
		0.0f, -0.05f,  0.0f
	};

	createVAO(&(userData->axisVaoZ), verticesZ, sizeof(verticesZ), NULL, 0, VAO_XYZ);
}

//-------------------------------------------
/*
 * Initialize userData, create our model etc.
 */
int init(ESContext *esContext) {
	UserData *userData = esContext->userData;

	// Init our model (graphical objects)
	initModel(userData);
	
	// Init axes (painted by me)
	initAxes(userData);

	// Set background color
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	return TRUE;
}

/////////////////////////////////////////////////////
//  DRAW SECTION
///////////////////////////////////


//-------------------------------------------
/*
* Draw axes with current view and proj, ignoring model
*/
void drawAxes(UserData *userData) {
	
	// Set up "model" matrix to unity, leaving view and proj untouched
	ESMatrix model;
	mxOne(&model); // Identity
	glUniformMatrix4fv(glGetUniformLocation(userData->program, "model"), 1, GL_TRUE, (const GLfloat*) &model.m);

	glUniform1i(glGetUniformLocation(userData->program, "useTexture"), GL_FALSE); // Use color, not textures
	// Draw the axes with respective color
	GLuint acLoc = glGetUniformLocation(userData->program, "myColor");

	// X = Red
	glUniform4f(acLoc, 1.0f, 0.0f, 0.0f, 1.0f);
	drawVAO(&(userData->axisVaoX));

	// Y = Green
	glUniform4f(acLoc, 0.0f, 1.0f, 0.0f, 1.0f);
	drawVAO(&(userData->axisVaoY));

	// Z = Blue
	glUniform4f(acLoc, 0.0f, 0.0f, 1.0f, 1.0f);
	drawVAO(&(userData->axisVaoZ));
}

//-------------------------------------------
/*
 * Draw POIs and axes
 */
void drawModel(ESContext *esContext) {
	UserData *userData = esContext->userData;

	// Use the program object
	glUseProgram(userData->program);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, userData->tex);


	// Set up transformations. 

	// Note: transpose in glUniformMatrix4fv can be true or false, this is important
	// My matrix.c requires true, esTransform.c from the book requires false
	// In fact normal C matrices M_ij=M[i][j] need true
	// For historical reasons many people avoid true,
	// But I don't think it matters nowadays

	// For now I use esTransform.c for lookAt and perspective (view+proj)
	// And my matrix.h for model operations
	// Change in the future

	
	// Matrix examples

	//mxRot(&tmpmat, mxRad(34.0f)*niceTime, 0.5f, 1.0f, 0.0f);

	// mxTran(&model, 0.0f, 0.0f, 0.0f);
	//mxMul(&model, &model, &tmpmat);

	// transpose = true
	// glUniformMatrix4fv(glGetUniformLocation(userData->program, "model"), 1, GL_TRUE, (const GLfloat*)&model.m);

	// View
	ESMatrix view;
	esMatrixLoadIdentity(&view);
	esMatrixLookAt(&view,
		userData->cameraX, userData->cameraY, userData->cameraZ,   // camera position
		userData->lookX, userData->lookY, userData->lookZ,    // look at position
		0.0f, 1.0f, 0.0f);     // up vector 

	// transpose = false
	glUniformMatrix4fv(glGetUniformLocation(userData->program, "view"), 1, GL_FALSE, (const GLfloat*) &view.m);

	// Projection
	ESMatrix proj;
	esMatrixLoadIdentity(&proj);
	esPerspective(&proj, 45.0f, (GLfloat)esContext->width / (GLfloat)esContext->height, 0.1f, 100.0f);

	// transpose = false
	glUniformMatrix4fv(glGetUniformLocation(userData->program, "proj"), 1, GL_FALSE, (const GLfloat*)&proj.m);


	// We don't need matrices proj and view anymore
	// Transpose them to normal (row-wise) notation
	mxTranspose(&proj, &proj);
	mxTranspose(&view, &view);

	ESMatrix projView; //proj*view, needed for occultation
	mxMul(&projView, &proj, &view);

	// Draw POIs
	for (int poiIndex = 0; poiIndex < userData->numPois; poiIndex++) {
		drawPOI(&(userData->pois[poiIndex]), &(userData->uniformCache), &projView,
			userData->cameraX, userData->cameraY, userData->cameraZ,                   // Camera pos for distance
			userData->lookX - userData->cameraX, userData->lookZ - userData->cameraZ); // Camera direction for rotation
	}

	// Check for occultation=eclipse=POI behind another
	// Sets/clears the isEclipsed flag in each POI
	occultPOI(userData->pois, userData->numPois);

	// Draw axes
	if (FUNGLES_DRAW_AXES) drawAxes(userData);
}

//-------------------------------------------
/*
 * Draw callback, aka "the game loop body"
 */
void draw(ESContext *esContext)
{
	UserData *userData = esContext->userData;

	// Set the viewport
	// Why do it every frame??? because window could be resized ?
	// Here on Windows it can't. Move it to Init ?
	glViewport(0, 0, esContext->width, esContext->height);

	// Clear the color buffer (set background)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get the time as float in seconds since an arbitrary origin
	GLfloat niceTime = (GLfloat)clock() / CLOCKS_PER_SEC;

	// Set camera movement

	GLfloat speed = 1.0f;

	// Little tilt around the Z axis in the XZ plane
	GLfloat angle = 0.4f*sinf(niceTime*speed);

	userData->cameraX = sinf(angle) * 10;
	userData->cameraZ = cosf(angle) * 10;

	// Some slow movement around Y axis
	userData->cameraY = 1.0f*(sinf(niceTime*speed*sqrtf(2.0f)/4)+1);

	// Camera looks at (0, 0, 0) for now
	userData->lookX = 0.0f;
	userData->lookY = 0.0f;
	userData->lookZ = 0.0f;

	// Draw model and axes
	drawModel(esContext);

}

/////////////////////////////////////////////////////
//  SHUTDOWN + MAIN SECTION
///////////////////////////////////

void shutdown(ESContext *esContext)
{
	UserData *userData = esContext->userData;

	// Clean up our buffers and stuff
	// Delete VAO
	deleteVAO(&(userData->vao1));

	// Delete Axes
	deleteVAO(&(userData->axisVaoX));
	deleteVAO(&(userData->axisVaoY));
	deleteVAO(&(userData->axisVaoZ));

	// Delete POIs
	for (int poiIndex = 0; poiIndex < userData->numPois; poiIndex++) {
		deletePOI(&(userData->pois[poiIndex]));
	}

	// Delete program
	glDeleteProgram(userData->program);

	// Delete user data -- does not work for some mysterious reason !
	// free(userData); 
}

/*
 * ENTRY POINT 
 * This is called by the architecture-dependent container such as esUtil_win32.c 
 */
int esMain(ESContext *esContext)
{
	// Allocate user data
	esContext->userData = malloc(sizeof(UserData));

	esCreateWindow(esContext, "Dark Elf OpenGL ES", 1000, 800, ES_WINDOW_RGB);

	// Initialize everything
	if (!init(esContext))
	{
		return GL_FALSE;
	}

	esRegisterShutdownFunc(esContext, shutdown);
	esRegisterDrawFunc(esContext, draw);

	return GL_TRUE;
}
