#include "esUtil.h"
#include "uniformcache.h"

/* Create uniform location caches */
void createUniformCache(UniformCache *uniformCache, GLuint program){
	uniformCache->model = glGetUniformLocation(program, "model");
	uniformCache->view = glGetUniformLocation(program, "view");
	uniformCache->proj = glGetUniformLocation(program, "proj");
	uniformCache->useTexture = glGetUniformLocation(program, "useTexture");
	uniformCache->myColor = glGetUniformLocation(program, "myColor");
}