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
#include "SoundHandler.h"

#include <glm/gtc/type_ptr.hpp>

#include "MazeGeneratePNG.h"

// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#undef main

#define PI 3.1415926535


// Shader initiation functions
void InitMazeShader(Shader *shader, Maze * maze);
void InitShadowShader(Shader *shader);
void InitGeometryPass(Shader *shader);
void InitLightPass(Shader *shader);
void InitParticleShader(Shader *shader);
void InitPointLightPass(Shader *shader);
void InitBlurShader(Shader *shader);
void InitFinalBloomShader(Shader *shader);
void InitFinalShader(Shader *shader);

// Shader pass functions
void MazePass(Shader *mazeShader, Maze * maze);
void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Maze * maze);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Player *player, ObjectHandler *OH, Maze * maze, Texture * tempBrickTexture);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, Mesh *fullScreenQuad, GLuint *program, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera);
void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera);
void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle);
void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle);
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime, glm::vec3 position);
void FinalPass(FinalFBO * finalFBO, Shader * finalShader, Mesh *fullScreenTriangle);

// height and width must be odd numbers else the resulting maze will be off
void GenerateMazeBitmaps(int height, int width);