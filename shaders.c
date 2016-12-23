/* By Oleksiy Grechnyev 2016
Create shaders and shader program */

#include <stdio.h>
#include <stdlib.h>

#include "esUtil.h"

#include "fatalerr.h"


/* Read a file as a single malloc-allocated string, returns NULL if error */
const char *readFile(const char * fileName){
    char *buffer; // The bufer to create
    size_t fileSize; // File size
    
    FILE* file= fopen(fileName, "r"); // Open file
    if (file==NULL) return NULL;
    
    // Determine size
    if (fseek(file, 0, SEEK_END)) return NULL;
    fileSize = ftell(file);
    if (fileSize <=0) return NULL;
    rewind(file);
    
    // Allocate memory and read
    buffer = malloc(fileSize+1);
    if (buffer==NULL) return NULL;
    
    // Read data to buffer
    fread (buffer, 1, fileSize, file);
    
    buffer[fileSize]='\0'; // End of C string
    
    fclose(file);
    
    return (const char*)buffer;
}

/* Create shaders and the shader program from external files*/
GLuint createShaderProgram(const char* vertexShaderFile,  const char* fragmentShaderFile){
    
    // Read files
    const char *vertexShaderSource=readFile(vertexShaderFile);
    if (vertexShaderSource==NULL) fatalError2("Error: Cannot read shader file ", vertexShaderFile);
    
    const char *fragmentShaderSource=readFile(fragmentShaderFile);
    if (fragmentShaderSource==NULL) fatalError2("Error: Cannot read shader file ", fragmentShaderFile);

    // Build the shader program
    // Vertex shader

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for errors
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        fatalError2("Error: Cannot compile shader \n", infoLog);
    }
    
    // Fragment shader
	
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);
    // Check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        fatalError2("Error: Cannot compile shader \n", infoLog);
    }
    
    // Shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        fatalError2("Error: Cannot link shader program \n", infoLog);
    }
    
    // Delete shaders: we don't need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	// Free memory
	free((char *)vertexShaderSource);
	free((char *)fragmentShaderSource);
    
    return shaderProgram;
}
