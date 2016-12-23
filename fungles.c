/* By Oleksiy Grechnyev 2016
Fun with OpenGL ES */

#include <time.h>
#include <math.h>

#include "esUtil.h"
#include "fatalerr.h"
#include "shaders.h"
#include "vao.h"
#include "rgbtex.h"
#include "matrix.h"

typedef struct
{
	// Model data
	GLuint program; // Program
	VaoObject vao1; // Vertices
	GLuint tex;  // Texture


	// Axes (painted by me) data

	// I could have used translation, but it's more clear this way
	VaoObject axisVaoX, axisVaoY, axisVaoZ;

} UserData;


/////////////////////////////////////////////////////
// INIT SECTION
///////////////////////////////////

//-------------------------------------------
/*
Initialize graphical model (sans axes)
*/
void initModel(UserData *userData) {

	// Create the program
	userData->program = createShaderProgram("shaders/s.vs", "shaders/s.frag");

	// Set vertices and VAO
	// Vertex XYZ + texture ST
	GLfloat vVertices[] = {
		-0.5f,  0.5f, 0.0f,  0.0f, 3.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  3.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 3.0f,
		0.5f,  0.5f, 0.0f,  3.0f, 3.0f,
		0.5f, -0.5f, 0.0f,  3.0f, 0.0f
	};

	// Vertices for a cube
	GLfloat cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Indices
	GLuint indices[] = {
		0, 1, 2,  // Triangle 1
		3, 4, 5  // Triangle 2
	};

	// Set up VAO from vertices (with EBO from indices)
	// createVAO(&(userData->vao1), vVertices, sizeof(vVertices), indices, sizeof(indices), VAO_XYZST);

	// Set up VAO from vertices without EBO 
	createVAO(&(userData->vao1), cubeVertices, sizeof(cubeVertices), NULL, 0, VAO_XYZST);

	printf("Model: nDrawElements = %d \n", userData->vao1.nDrawElements);

	// Set up a trivial texture
	// 2 x 2 Image, 3 bytes per pixel (R, G, B)
	GLubyte pixels[4 * 3] =
	{
		255, 0, 0, // Red
		0, 255, 0, // Green
		0, 0, 255, // Blue
		255, 255, 0 // Yellow
	};
	userData->tex = createRGBTexture(pixels, 2, 2);
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
	    0.0f,  0.1f,  0.0f,
	    0.0f, -0.1f,  0.0f,
		// Triangle 2
	    3.0f,  0.0f,  0.0f,
	    0.0f,  0.0f,  0.1f,
        0.0f,  0.0f, -0.1f
	};

	createVAO(&(userData->axisVaoX), verticesX, sizeof(verticesX), NULL, 0, VAO_XYZ);
	printf("Axis X: nDrawElements = %d \n", userData->axisVaoX.nDrawElements);

	// Y
	GLfloat verticesY[] = {
		// Triangle 1
		0.0f,  3.0f,  0.0f,
		0.1f,  0.0f,  0.0f,
	   -0.1f,  0.0f,  0.0f,
		// Triangle 2
		0.0f,  3.0f,  0.0f,
		0.0f,  0.0f,  0.1f,
		0.0f,  0.0f, -0.1f
	};
	createVAO(&(userData->axisVaoY), verticesY, sizeof(verticesY), NULL, 0, VAO_XYZ);

	// Z
	GLfloat verticesZ[] = {
		// Triangle 1
		0.0f,  0.0f,  3.0f,
		0.1f,  0.0f,  0.0f,
       -0.1f,  0.0f,  0.0f,
		// Triangle 2
		0.0f,  0.0f,  3.0f,
		0.0f,  0.1f,  0.0f,
		0.0f, -0.1f,  0.0f
	};

	createVAO(&(userData->axisVaoZ), verticesZ, sizeof(verticesZ), NULL, 0, VAO_XYZ);
}

//-------------------------------------------
/*
 * Initialize userData, create our model etc.
 */
int Init(ESContext *esContext) {
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
* Draw axes
*/

void drawAxes(UserData *userData) {
	
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
 * Draw model and axes
 */
void drawModel(ESContext *esContext, GLfloat niceTime) {
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


	// Model
	ESMatrix model, tmpmat;

	mxOne(&model); // Safety plug

	//mxRot(&tmpmat, mxRad(34.0f)*niceTime, 0.5f, 1.0f, 0.0f);

	// mxTran(&model, 0.0f, 0.0f, 0.0f);
	//mxMul(&model, &model, &tmpmat);

	// transpose = true
	glUniformMatrix4fv(glGetUniformLocation(userData->program, "model"), 1, GL_TRUE, &model.m);

	// View
	ESMatrix view;
	esMatrixLoadIdentity(&view);

	GLfloat camX = cos(niceTime) * 10;
	GLfloat camZ = sin(niceTime) * 10;

	// camX = 0.1;
	// camZ = 10.0f;

	esMatrixLookAt(&view,
		camX, 0.0f, camZ,   // camera position
		0.0f, 0.0f, 0.0f,    // look at position
		0.0f, 1.0f, 0.0f);     // up vector 


	// transpose = false
	glUniformMatrix4fv(glGetUniformLocation(userData->program, "view"), 1, GL_FALSE, &view.m);

	// Projection
	ESMatrix proj;
	esMatrixLoadIdentity(&proj);

	esPerspective(&proj, 45.0f, (GLfloat)esContext->width / (GLfloat)esContext->height, 0.1f, 100.0f);


	// transpose = false
	glUniformMatrix4fv(glGetUniformLocation(userData->program, "proj"), 1, GL_FALSE, &proj.m);

	// Draw the model
	glUniform1i(glGetUniformLocation(userData->program, "useTexture"), GL_TRUE); // Use textures
	drawVAO(&(userData->vao1));

	// Draw axes
	drawAxes(userData);
}



//-------------------------------------------
/*
 * Draw callback
 */
void Draw(ESContext *esContext)
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

	// Draw model and axes
	drawModel(esContext, niceTime);

	// Draw axes
	//drawAxes(esContext, niceTime);
}

/////////////////////////////////////////////////////
//  SHUTDOWN + MAIN SECTION
///////////////////////////////////

void Shutdown(ESContext *esContext)
{
	UserData *userData = esContext->userData;


	// Clean up our buffers and stuff
	// Delete VAO
	deleteVAO(&(userData->vao1));

	// Delete program
	glDeleteProgram(userData->program);
}

/*
 ENTRY POINT 
 This is called by the architecture-dependent container such as esUtil_win32.c
*/
int esMain(ESContext *esContext)
{
	esContext->userData = malloc(sizeof(UserData));

	esCreateWindow(esContext, "Dark Elf OpenGL ES", 800, 600, ES_WINDOW_RGB);

	if (!Init(esContext))
	{
		return GL_FALSE;
	}

	esRegisterShutdownFunc(esContext, Shutdown);
	esRegisterDrawFunc(esContext, Draw);

	return GL_TRUE;
}
