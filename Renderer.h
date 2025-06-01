#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>     // Harus di-include duluan
#include <GL/glut.h>       // Setelah glad

#include <memory>
#include <vector>
#include <iostream>

#include "renderChunks.h"
void setupLighting();
void initOpenGL();
void display();

#endif // RENDERER_H