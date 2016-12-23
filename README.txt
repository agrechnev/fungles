Fun with OpenGL ES

Note: files 
esUtil.c, esUtil.h, esUtil_win.h, esUtil_win32.c, esTransform.c

Are taken from 

https://github.com/danginsburg/opengles3-book/

They're enough to compile for windows, for android/ios you'll need more files.

The idea here is that OpenGL ES needs a container of some sort (like GLFW for opengl)

For Visual Studio: you'll need *.h and *.lib files of the emulator.

Disable precompiled headers, SDL checks, and set project's character set to "not set" to
use non-unicode windows library for proper window title.