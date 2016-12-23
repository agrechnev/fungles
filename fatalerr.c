/* By Oleksiy Grechnyev 2016
Print error messages and exit */
#include <stdio.h>
#include <stdlib.h>

void fatalError(const char *text) {
	puts(text);
	exit(-1);
}

// The 2-string version
void fatalError2(const char *text1, const char *text2) {
	printf("%s", text1);
	puts(text2);
	exit(-1);
}