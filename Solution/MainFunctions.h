#pragma once
#include <iostream>
#include <glew\glew.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "ObjectHandler.h"
#include <ctime>
#include "timer.h"
#include "GBuffer.h"
#include "PointLight.h"
#include "Particle.h"
#include <stdio.h>
#include "Bloom.h"
#include "Blur.h"
#include "FinalFBO.h"
#include "ShadowMap.h"
#include "InputHandler.h"
#include "Player.h"
#include "Maze.h"

#include <glm/gtc/type_ptr.hpp>

#include "MazeGeneratePNG.h"

// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#undef main

#define PI 3.1415926535

// Update functions
void updateAllObjects(double dt, ObjectHandler &OH);

// Shader pass functions
void shadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Camera *camera);
void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, GLuint cameraLocationGP, GLint texLoc, GLint normalTexLoc, int torch);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, Mesh *fullScreenQuad, GLuint *program, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, GLuint cameraLocationLP, Camera *camera);
void lightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, double counter);
void blurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle);
void finalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle);
void particlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime, glm::vec3 position);
void finalPass(FinalFBO * finalFBO, Shader * finalShader, Mesh *fullScreenTriangle);

void sendCameraLocationToGPU(GLuint cameraLocation, Camera *camera);
void prepareTexture(GLuint textureLoc, GLuint normalMapLoc);

void setStartPositions(ObjectHandler *OH);

// height and width must be odd numbers else the resulting maze will be off
void generateMazeBitmaps(int height, int width);