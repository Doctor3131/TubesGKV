#pragma once
#include <GL/glut.h>
#include <memory>
#include <vector>
#include <iostream>

#include "renderChunks.h"
#include "Player.h"

// extern Player player;

void initOpenGL();

void keyboard(unsigned char key, int x, int y);

void reshape(int w, int h);

void updateScene(int value);

void mouseMove(int x, int y);

void setPlayerSpawnPosition();

void display();

