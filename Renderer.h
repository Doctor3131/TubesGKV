#pragma once
#include <GL/glut.h>
#include <memory>
#include <vector>
#include <iostream>

#include "renderChunks.h"
#include "Player.h"

// extern Player player;

extern bool justWarped;
extern int centerX, centerY;
extern float yaw, pitch;

void initOpenGL();
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void updateScene(int value);
void mouseMove(int x, int y);
void setPlayerSpawnPosition();
void setupLighting();
void drawText(float x, float y, const char* text);
void updateFPS();
void display();
void drawDoor(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, bool isBottomPart);
void drawFrontTextureOnly(const Vector3& pos, const Vector3& size, GLuint frontTexture);
void drawCube(const Vector3& pos, const Vector3& size, GLuint textureID);
void drawCubeWithTextures(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, const Block* block);

