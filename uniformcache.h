/* By Oleksiy Grechnyev 2016
 * Set up uniform location cache
 * Includes structure UniformCache
 */

#ifndef UNIFORMCACHE_H
#define UNIFORMCACHE_H
#include "esUtil.h"

typedef struct UniformCache {
	GLuint model;
	GLuint view;
	GLuint proj;
	GLuint useTexture;
	GLuint myColor;
} UniformCache;

/* Create uniform caches */
void createUniformCache(UniformCache *uniformCache, GLuint program);

#endif