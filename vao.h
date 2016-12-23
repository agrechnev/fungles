/* Create VAO, this includes creating vertices, VBO, EBO */
#ifndef VAO_H
#define VAO_H

// Type of vertex data (ver_type)
// XYZ coords only
#define VAO_XYZ 1  
// XYZ coolrds + RGB color
#define VAO_XYZRGB 2
// XYZ coolrds +  ST texture coords
#define VAO_XYZST 3
// XYZ coolrds + RGB color + ST texture coords
#define VAO_XYZRGBST 4

// The VAO object
typedef struct VaoObject {
	int useEbo; //TRUE = use EBO
	GLuint nDrawElements; // Number of elements to draw

	// VAO, VBO, EBO
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
} VaoObject;

/* Create VAO, this includes creating vertices, VBO, EBO
 * pVAO, pVBO, pEBO = pointers to VAO, VBO, EBO
 * vertices[], indices[]  = data arrays
 * indices == 0 : don't use EBO
 * ver_size = sizeof(vertices)
 * ind_size = sizeof(indices)
 * ver_type = type of vertex data, e.g. VAO_XYZRBST
 */
void createVAO(VaoObject *pVO, GLfloat* vertices, size_t  ver_size, GLuint* indices, size_t ind_size, int ver_type);

/* Delete VAO, VBO and EBO 
 *  EBO is skipped if pEBO==NULL
 */
void deleteVAO(VaoObject *pVO);

/*
* Draw a Vao object
*/
void drawVAO(VaoObject *pVO);

#endif
